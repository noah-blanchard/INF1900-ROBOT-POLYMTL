#define F_CPU 8000000UL
#include <avr/io.h>
#include "Debug.h"

int main()
{
    Communication comm;
    const char *message = "Hello World! THIS IS DEBUG MESSAGE !\n";

    while (true)
    {
        DEBUG(message);
    }

    return 0;
}
