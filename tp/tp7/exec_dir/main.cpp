#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "Navigation.h"

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
    // Led led(&PORTA, &DDRA, PA0, PA1);

    DDRD |= (1 << PD4) | (1 << PD5) | (1 << PD6) | (1 << PD7);

    DDRA = (1 << DDA0) | (1 << DDA1);
    PORTA |= (1 << PA0); // Green quand on avance
    // while(true)
    // {
    //     //  led.turnLedGreen();
    //     // _delay_ms(3000);
    //     led.turnLedAmber();
    //     // _delay_ms(3000);
    //     // led.turnLedRed();
    //     // _delay_ms(6000);
    // }

    Navigation nav;
    uint16_t speed = 255;

    while (true)
    {
            nav.spin();
        nav.go(speed);
    }

    return 0;
}
