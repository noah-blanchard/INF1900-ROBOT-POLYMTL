#include <avr/io.h>
#include <util/delay.h>
#include <Communication.h>
#include <memoire_24.h>

class ByteCodeInterpreter
{
public:
    ByteCode();
    ~ByteCode();
    bool receiveByteCode();

private:
    Communication com;
    Memoire24CXXX memoire;
};