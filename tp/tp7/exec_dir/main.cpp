#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "LED.h"

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
    LED a0a1LED(&PORTA, &DDRA, PA0, PA1);

    while (true)
    {
        a0a1LED.turnLedRed();
    }

    return 0;
}
