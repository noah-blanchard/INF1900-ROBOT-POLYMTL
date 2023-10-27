#define F_CPU 8000000UL
#include <avr/io.h>
#include "Debug.h"


int main()
{

    Communication comm;
    const char *message = "hello";
    //Communication com;
    while (true)
    {
      DEBUG("debug\n", comm);
    }

    return 0;
}
