#include <avr/io.h>
#include <util/delay.h>
#include "Communication.h"
#include "memoire_24.h"
#include "LED.h"

class ByteCodeInterpreter
{
public:
    ByteCodeInterpreter();
    ~ByteCodeInterpreter();
    bool receiveAndSave();
    void run();

    // Instructions const
    // Déclaration des constantes d'instructions
    static const uint8_t DBT = 0b00000001;  // début
    static const uint8_t ATT = 0b00000010;  // attendre
    static const uint8_t DAL = 0b01000100;  // allumer la DEL
    static const uint8_t DET = 0b01000101;  // éteindre la DEL
    static const uint8_t SGO = 0b01001000;  // jouer une sonorité
    static const uint8_t SAR = 0b00001001;  // arrêter de jouer la sonorité
    static const uint8_t MAR = 0b01100000;  // arrêter les moteurs
    static const uint8_t MAR2 = 0b01100001; // arrêter les moteurs
    static const uint8_t MAV = 0b01100010;  // avancer
    static const uint8_t MRE = 0b01100011;  // reculer
    static const uint8_t TRD = 0b01100100;  // tourner à droite
    static const uint8_t TRG = 0b01100101;  // tourner à gauche
    static const uint8_t DBC = 0b11000000;  // début de boucle
    static const uint8_t FBC = 0b11000001;  // fin de boucle
    static const uint8_t FIN = 0b11111111;  // fin

    static const uint16_t defaultDelayValue = 25; // default to 25 ms

private:
    void interpreteByteCode(uint8_t byteCode);
    void executeDBC(uint8_t iterations, uint16_t startAdress);
    void executeATT(uint8_t delay);
    void executeDAL(uint8_t color);
    void customDelay(uint16_t delay);

    Communication com;
    Memoire24CXXX memory;
    LED led;
    uint8_t currentAddress = 0x00;
};