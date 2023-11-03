#include "ByteCodeInterpreter.h"

volatile bool timerHasElapsed = false;

ISR(TIMER0_COMPA_vect)
{
    timerHasElapsed = true;
}

/**
 * @brief Construct a new Byte Code Interpreter object
 *
 */

ByteCodeInterpreter::ByteCodeInterpreter() : led(&PORTA, &DDRA, PA0, PA1)
{
    TimerConfig timerConfig;
    timerConfig.timer = 0;
    timerConfig.prescaler = 256;
    timerConfig.delay_ms = 1;
    timer = Timer(timerConfig);
}

/**
 * @brief Destroy the Byte Code Interpreter object
 *
 */
ByteCodeInterpreter::~ByteCodeInterpreter()
{
}

/** BYTECODES
 * dbt: 0000 0001
att: 0000 0010
dal: 0100 0100
det: 0100 0101
sgo: 0100 1000
sar: 0000 1001
mar: 0110 0000 (Note : Il semble y avoir une répétition pour "arrêter les moteurs", je suppose que c'est une erreur et je ne liste qu'une seule version ici.)
mav: 0110 0010
mre: 0110 0011
trd: 0110 0100
trg: 0110 0101
dbc: 1100 0000
fbc: 1100 0001
fin: 1111 1111
*/

/**
 * @brief Receive the byte code from the serial communication, and save it to memory using memoire_24, while loop until 1111 1111 is received
 *
 * @return true If the byte code was received
 * @return false If the byte code was not received
 */
bool ByteCodeInterpreter::receiveAndSave()
{
    uint8_t byteCode = 0x00;
    uint16_t address = 0x00;
    bool isDBT = false;
    while (byteCode != FIN)
    {
        byteCode = com.receive();
        if ((byteCode == DBT) | isDBT)
        {
            memory.ecriture(address, byteCode);
            isDBT = true;
            address++;
        }
    }
    return true;
}

/**
 * @brief Send the byte code from memory using memoire_24, while loop until 1111 1111 is sent
 *
 */
void ByteCodeInterpreter::sendByteCode()
{
    // read the memory and send each byte to com.sendString, use an string fstring
    uint8_t byteCode = 0x00;
    uint16_t address = 0x00;
    char buffer[4];
    while (byteCode != FIN)
    {
        memory.lecture(address, &byteCode);
        sprintf(buffer, "%02X\n", byteCode);
        com.sendString(buffer);
        address++;
    }
    com.sendString("\n");
}

/**
 * @brief Runs interpreteByteCode with start address 0x00
 *
 */

void ByteCodeInterpreter::run()
{
    currentAddress = 0x00;
    uint8_t byteCode = 0x00;
    while (byteCode != FIN)
    {
        memory.lecture(currentAddress, &byteCode);
        this->interpreteByteCode(byteCode);
        ++currentAddress;
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
        break;
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
    case MAR:
        break;
    case MAV:
        break;
    case MRE:
        break;
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
 * @brief Routine for executing a DBC instruction
 *
 */
void ByteCodeInterpreter::executeDBC(uint16_t iterationAddress, uint16_t startAdress)
{

    uint8_t iterations = 0;
    memory.lecture(iterationAddress, &iterations);

    // go back to current address when FBC is met, go back to interpreteByteCode when fbc is met and iterations is 0
    uint8_t byteCode = 0x00;
    while (iterations != 0)
    {
        memory.lecture(currentAddress, &byteCode);
        this->interpreteByteCode(byteCode);
        ++currentAddress;

        if (byteCode == FBC)
        {
            --iterations;
            currentAddress = startAdress;
        }
    }
}

void ByteCodeInterpreter::executeATT(uint16_t delayAddress)
{
    // ten times the default delay value, stored in a uint16_t
    uint8_t delay = 0x00;
    memory.lecture(delayAddress, &delay);
    uint16_t delayValue = delay * defaultDelayValue;
    // send this value via RS232
    char buffer[4];
    sprintf(buffer, "%d\n", delayValue);
    com.sendString(buffer);

    // delay for this value
    this->customDelay(delayValue);
}

void ByteCodeInterpreter::executeDAL(uint16_t colorAddress)
{
    uint8_t color = 0x00;
    memory.lecture(colorAddress, &color);
    switch (color)
    {
    case 0x01:
        led.turnLedGreen();
        break;
    case 0x02:
        led.turnLedRed();
        break;
    default:
        break;
    }
}

void ByteCodeInterpreter::executeTRD()
{
    const uint16_t speed = 255;
    com.sendString("TRD\n");
    nav.goLeftWheel(speed, false); // left wheel forward
    nav.goRightWheel(speed, true); // right wheel backward
    this->customDelay(turnDelayValue);
    nav.stop();
    ++currentAddress;
}

void ByteCodeInterpreter::executeTRG()
{
    const uint16_t speed = 255;
    com.sendString("TRG\n");
    nav.goLeftWheel(speed, true);   // left wheel backward
    nav.goRightWheel(speed, false); // right wheel forward
    this->customDelay(turnDelayValue);
    nav.stop();
    ++currentAddress;
}

void ByteCodeInterpreter::executeSGO(uint16_t noteAddress)
{
    uint8_t note = 0x00;
    // memory.lecture(noteAddress, &note);
    // sound.chooseFrequency(note);
    uint8_t noteDuration = 200; // 200 ms par note
    
    for (int i = 0; i < 30 * 1000 / noteDuration; ++i) // Jouer pendant 30 secondes
    {
        // Séquence de notes de votre choix, par exemple, C, D, E, F, G, A, B
        for (int note = 60; note <= 71; ++note)
        {
            sound.chooseFrequency(note);
            _delay_ms(noteDuration);
            sound.stopSound();
            _delay_ms(10); // Pause entre les notes
        }
    }
}

void ByteCodeInterpreter::executeSAR()
{
    sound.stopSound();
    ++currentAddress;
}

// do the same but using 5 instead of 1 because possible errors cpu delay, but adapt the value in the for loop delay / 5
void ByteCodeInterpreter::customDelay(uint16_t delay)
{
    timer.reset();
    // use timer0 to wait for the delay
    for (int i = 0; i < delay; ++i)
    {
        timer.enable();
        sei();
        while (!timerHasElapsed)
            ;
        timerHasElapsed = false;
        cli();
        timer.disable();
        timer.reset();
    }
}
