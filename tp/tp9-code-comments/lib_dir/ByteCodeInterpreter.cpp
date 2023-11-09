#include "ByteCodeInterpreter.h"

volatile bool timerHasElapsed = false;

ISR(TIMER2_COMPA_vect)
{
    timerHasElapsed = true;
}

/**
 * @brief Construct a new Byte Code Interpreter object
 *
 */
// Constructeur de l'interprète de bytecode, initialise le timer et les LED.
ByteCodeInterpreter::ByteCodeInterpreter() : led(&PORTC, &DDRC, PC0, PC1)
{
    TimerConfig timerConfig;     // Structure pour configurer le timer.
    timerConfig.timer = 2;       // Utilisation de TIMER2.
    timerConfig.prescaler = 256; // Définit le prescaler pour le timer.
    timerConfig.delay_ms = 1;    // Délai en millisecondes pour le timer.
    timer = Timer(timerConfig);  // Initialisation du timer avec la configuration définie.
}

/**
 * @brief Destroy the Byte Code Interpreter object
 *
 */
ByteCodeInterpreter::~ByteCodeInterpreter()
{
}

/**
 * @brief Receive the byte code from the serial communication, and save it to memory using memoire_24, while loop until 1111 1111 is received
 *
 * @return true If the byte code was received
 * @return false If the byte code was not received
 */
// Fonction pour recevoir le bytecode et le sauvegarder en mémoire.
bool ByteCodeInterpreter::receiveAndSave()
{
    uint8_t byteCode = 0x00; // Variable pour stocker le bytecode reçu.
    uint16_t address = 0x00; // Adresse initiale pour la sauvegarde du bytecode.
    bool isDBT = false;      // Flag pour détecter le début du texte (start of text).
    memory.init();           // Initialisation de la mémoire EEPROM.
    while (byteCode != FIN)  // Boucle jusqu'à la réception du code de fin.
    {
        byteCode = com.receive();       // Réception d'un byte via la communication série.
        if ((byteCode == DBT) || isDBT) // Si le byte de début est reçu ou si on est déjà en mode réception.
        {
            memory.ecriture(address, byteCode); // Écriture du byte en mémoire à l'adresse spécifiée.
            isDBT = true;                       // Mise à jour du flag indiquant qu'on a commencé la réception de données.
            address++;                          // Incrémentation de l'adresse pour le prochain byte.
        }
    }
    return true; // Retourne vrai lorsque tout le bytecode est reçu et sauvegardé.
}

/**
 * @brief Send the byte code from memory using UART using memoire_24, while loop until 1111 1111 is sent
 *
 */

// Fonction pour envoyer le bytecode sauvegardé via UART.
void ByteCodeInterpreter::sendByteCode()
{
    uint8_t byteCode = 0x00; // Variable pour lire le bytecode depuis la mémoire.
    uint16_t address = 0x00; // Adresse de départ pour l'envoi des données.
    char buffer[4];          // Buffer pour formatter les données avant envoi.
    while (byteCode != FIN)  // Boucle jusqu'à ce que le code de fin soit envoyé.
    {
        memory.lecture(address, &byteCode);  // Lecture du byte depuis la mémoire à l'adresse spécifiée.
        sprintf(buffer, "%02X\n", byteCode); // Formatage du byte en une chaîne hexadécimale.
        com.sendString(buffer);              // Envoi du byte formaté via la communication série.
        address++;                           // Incrémentation de l'adresse pour lire le prochain byte.
    }
    com.sendString("\n"); // Envoi d'une nouvelle ligne pour terminer la transmission.
}

/**
 * @brief Runs interpreteByteCode with start address 0x00
 *
 */

// Exécute l'interprétation du bytecode à partir de l'adresse de départ 0x00.
void ByteCodeInterpreter::run()
{
    currentAddress = 0x00;   // Adresse courante mise à 0x00.
    uint8_t byteCode = 0x00; // Variable pour stocker le bytecode actuel.
    while (byteCode != FIN)  // Boucle d'interprétation des bytecodes.
    {
        memory.lecture(currentAddress, &byteCode); // Lecture du bytecode à l'adresse courante.
        this->interpreteByteCode(byteCode);        // Appel de la fonction d'interprétation du bytecode.
        ++currentAddress;                          // Incrémentation de l'adresse courante pour passer au bytecode suivant.
    }
}

/**
 * @brief Execute the byte code from memory using memoire_24
 *
 */
void ByteCodeInterpreter::interpreteByteCode(uint8_t byteCode)
{
    switch (byteCode)
    {
    case DBT:
    {
        ++currentAddress;
        break;
    }
    case ATT:
    {
        this->executeATT(++currentAddress);
        break;
    }
    case DAL:
    {
        this->executeDAL(++currentAddress);
        break;
    }
    case DET:
    {
        this->executeDET();
        break;
    }
    case SGO:
    {
        this->executeSGO(++currentAddress);
        break;
    }
    case SAR:
    {
        this->executeSAR();
        break;
    }
    case (MAR | MAR2):
    {
        this->executeMAR();
        break;
    }
    case MAV:
    {
        this->executeMAV(++currentAddress);
        break;
    }
    case MRE:
    {
        this->executeMRE(++currentAddress);
        break;
    }
    case TRD:
    {
        this->executeTRD();
        break;
    }
    case TRG:
    {
        this->executeTRG();
        break;
    }
    case DBC:
    {
        uint16_t iterationAddress = ++currentAddress;
        uint16_t loopStartAddress = ++currentAddress;
        this->executeDBC(iterationAddress, loopStartAddress);
        break;
    }
    case FBC:
        break;
    case FIN:
        break;
    default:
        break;
    }
}

/**
 * @brief Exécute une instruction DBC (Début de Bloc de Commande)
 *
 * @param iterationAddress Adresse en mémoire où le nombre d'itérations est stocké.
 * @param startAdress Adresse de départ pour l'exécution du bloc de commandes.
 */
void ByteCodeInterpreter::executeDBC(uint16_t iterationAddress, uint16_t startAdress)
{
    uint8_t iterations = 0;
    memory.lecture(iterationAddress, &iterations); // Lecture du nombre d'itérations à partir de la mémoire.

    // Boucle d'exécution du bloc de commandes.
    uint8_t byteCode = 0x00;
    while (iterations != 0)
    {
        memory.lecture(currentAddress, &byteCode); // Lecture du bytecode courant.
        this->interpreteByteCode(byteCode);        // Interprétation du bytecode lu.
        ++currentAddress;

        // Si on rencontre FBC (Fin de Bloc de Commande), décrémenter le nombre d'itérations et revenir au début.
        if (byteCode == FBC)
        {
            --iterations;
            currentAddress = startAdress;
        }
    }
}

/**
 * @brief Exécute une instruction ATT (Attente)
 *
 * @param delayAddress Adresse en mémoire où la valeur de délai est stockée.
 */
void ByteCodeInterpreter::executeATT(uint16_t delayAddress)
{
    uint8_t delay = 0x00;
    memory.lecture(delayAddress, &delay);            // Lecture de la valeur de délai à partir de la mémoire.
    uint16_t delayValue = delay * defaultDelayValue; // Calcul du délai réel.

    // Attente active pendant le délai calculé.
    this->customDelay(delayValue);
}

/**
 * @brief Exécute une instruction DAL (Allumer DEL)
 *
 * @param colorAddress Adresse en mémoire où la couleur de la DEL est stockée.
 */
void ByteCodeInterpreter::executeDAL(uint16_t colorAddress)
{
    uint8_t color = 0x00;
    memory.lecture(colorAddress, &color); // Lecture de la valeur de couleur.

    // Allumage de la DEL en fonction de la couleur spécifiée.
    switch (color)
    {
    case 0x01:
        led.turnLedGreen(); // Allume la DEL en vert.
        break;
    case 0x02:
        led.turnLedRed(); // Allume la DEL en rouge.
        break;
    default:
        // Si la couleur n'est pas reconnue, ne rien faire.
        break;
    }
}

/**
 * @brief Exécute une instruction DET (Éteindre DEL).
 */
void ByteCodeInterpreter::executeDET()
{
    led.turnOffLed(); // Éteint la DEL.
    ++currentAddress;
}

/**
 * @brief Exécute une instruction TRD (Tourner à Droite).
 */
void ByteCodeInterpreter::executeTRD()
{
    const uint16_t speed = 255;
    com.sendString("TRD\n");           // Envoi de la commande de rotation à droite.
    nav.goLeftWheel(speed, false);     // Roue gauche avance.
    nav.goRightWheel(speed, true);     // Roue droite recule.
    this->customDelay(turnDelayValue); // Attente pendant la rotation.
    nav.stop();                        // Arrêt des moteurs.
    ++currentAddress;
}

/**
 * @brief Exécute une instruction TRG (Tourner à Gauche).
 */
void ByteCodeInterpreter::executeTRG()
{
    const uint16_t speed = 255;
    com.sendString("TRG\n");           // Envoi de la commande de rotation à gauche.
    nav.goLeftWheel(speed, true);      // Roue gauche recule.
    nav.goRightWheel(speed, false);    // Roue droite avance.
    this->customDelay(turnDelayValue); // Attente pendant la rotation.
    nav.stop();                        // Arrêt des moteurs.
    ++currentAddress;
}

/**
 * @brief Exécute une instruction SGO (Jouer un Son).
 *
 * @param noteAddress Adresse en mémoire où la note est stockée.
 */
void ByteCodeInterpreter::executeSGO(uint16_t noteAddress)
{
    uint8_t note = 0x00;
    memory.lecture(noteAddress, &note); // Lecture de la note de musique à partir de la mémoire.
    sound.chooseFrequency(note);        // Réglage de la fréquence du son à jouer.
}

/**
 * @brief Exécute une instruction SAR (Arrêter le Son).
 */
void ByteCodeInterpreter::executeSAR()
{
    sound.stopSound(); // Arrêt du son.
    ++currentAddress;
}

/**
 * @brief Exécute une instruction MAR (Arrêter les Moteurs).
 */
void ByteCodeInterpreter::executeMAR()
{
    nav.stop(); // Arrêt des moteurs.
    ++currentAddress;
}

/**
 * @brief Exécute une instruction MAV (Mouvement Avant).
 *
 * @param speedAddress Adresse en mémoire où la vitesse est stockée.
 */
void ByteCodeInterpreter::executeMAV(uint16_t speedAddress)
{
    uint8_t speed = 0x00;
    memory.lecture(speedAddress, &speed); // Lecture de la vitesse à partir de la mémoire.
    nav.go(speed, false);                 // Moteurs en mouvement vers l'avant.
}

/**
 * @brief Exécute une instruction MRE (Mouvement Arrière).
 *
 * @param speedAddress Adresse en mémoire où la vitesse est stockée.
 */
void ByteCodeInterpreter::executeMRE(uint16_t speedAddress)
{
    uint8_t speed = 0x00;
    memory.lecture(speedAddress, &speed); // Lecture de la vitesse à partir de la mémoire.
    nav.go(speed, true);                  // Moteurs en mouvement vers l'arrière.
}

void ByteCodeInterpreter::customDelay(uint16_t delay)
{
    timer.reset();                  // Réinitialisation du timer.
    cli();                          // Désactivation des interruptions globales.
    timer.enable();                 // Activation du timer.
    sei();                          // Réactivation des interruptions globales.
    for (int i = 0; i < delay; ++i) // Boucle pour créer le délai spécifié.
    {
        while (!timerHasElapsed)
            ;                    // Attente active jusqu'à ce que le timer ait expiré.
        timerHasElapsed = false; // Réinitialisation de l'état du timer.
        timer.reset();           // Réinitialisation du timer pour le prochain délai.
    }
}
