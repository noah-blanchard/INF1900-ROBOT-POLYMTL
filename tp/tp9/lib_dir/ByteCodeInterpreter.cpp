#include "ByteCodeInterpreter.h"

/**
 * @brief Construct a new Byte Code Interpreter object
 *
 */

ByteCodeInterpreter::ByteCodeInterpreter() : led(&PORTA, &DDRA, PA0, PA1)
{
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
    while (byteCode != FIN)
    {
        byteCode = com.receive();
        memory.ecriture(address, byteCode);
        address++;
    }
    return true;
}

/**
 * @brief Runs interpreteByteCode with start address 0x00
 *
 */

void ByteCodeInterpreter::run()
{
    uint8_t byteCode = 0x00;
    while (byteCode != FIN)
    {
        byteCode = memory.lecture(currentAddress);
        this->interpreteByteCode(byteCode, currentAddress);
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
        break;
    case ATT:
        break;
    case DAL:
        break;
    case DET:
        break;
    case SGO:
        break;
    case SAR:
        break;
    case MAR:
        break;
    case MAV:
        break;
    case MRE:
        break;
    case TRD:
        break;
    case TRG:
        break;
    case DBC:
        this->executeDBC(memory.lecture(++currentAddress), ++currentAddress);
        break;
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
void ByteCodeInterpreter::executeDBC(uint8_t iterations, uint16_t startAdress)
{
    // go back to current address when FBC is met, go back to interpreteByteCode when fbc is met and iterations is 0
    uint8_t byteCode = 0x00;
    while (iterations != 0)
    {
        byteCode = memory.lecture(currentAddress);
        this->interpreteByteCode(byteCode);
        ++currentAddress;

        if (byteCode == FBC)
        {
            --iterations;
            currentAddress = startAdress;
        }
    }
}

void ByteCodeInterpreter::executeATT(uint8_t delay)
{
    // ten times the default delay value, stored in a uint16_t
    uint16_t delayValue = delay * defaultDelayValue;
    this->customDelay(delayValue);
}

void ByteCodeInterpreter::executeDAL(uint8_t color)
{
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

// do the same but using 5 instead of 1 because possible errors cpu delay, but adapt the value in the for loop delay / 5
void ByteCodeInterpreter::customDelay(uint16_t delay)
{
    for (uint16_t i = 0; i < delay / 5; i++)
    {
        _delay_ms(5);
    }
}
