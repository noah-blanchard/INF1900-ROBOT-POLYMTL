#define F_CPU 8000000UL
#include <avr/io.h>

int main()
{
    Communication comm;
    const char *message = "Hello World!";

    while (true)
    {
        comm.sendString(message);
        _delay_ms(1000);
    }

    return 0;
}
