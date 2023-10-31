#include "ByteCodeInterpreter.h"

/**
 * @brief Construct a new Byte Code Interpreter object
 *
 */

ByteCodeInterpreter::ByteCodeInterpreter()
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
bool ByteCodeInterpreter::receiveByteCode()
{
    uint8_t byteCode = 0x00;
    uint16_t address = 0x00;
    while (byteCode != 0xFF)
    {
        byteCode = com.receive();
        memoire.ecriture(address, byteCode);
        address++;
    }
    return true;
}