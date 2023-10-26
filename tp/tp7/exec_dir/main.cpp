#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "Communication.h"

// constexpr uint8_t DEBOUNCE_TIME = 10;

// bool isButtonPressed() {return (PIND & (1 << PD2));}

// bool isDebounce()
// {
//     if(isButtonPressed())
//     {
//         _delay_ms(DEBOUNCE_TIME);
//         return isButtonPressed();
//     }
//     return false;
// }

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
