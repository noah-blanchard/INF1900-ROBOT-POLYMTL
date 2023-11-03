#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h> 
#include "led.h"

constexpr uint8_t DEBOUNCE_TIME = 10;

bool isButtonPressed() {return (PIND & (1 << PD2));}

bool isDebounce()
{
    if(isButtonPressed())
    {
        _delay_ms(DEBOUNCE_TIME);
        return isButtonPressed();
    }
    return false;
}

int main()
{
    Led led(&PORTA, &DDRA, PA0, PA1);

    while(true)
    {
        //  led.turnLedGreen();
        // _delay_ms(3000);
        led.turnLedAmber();
        // _delay_ms(3000);
        // led.turnLedRed();
        // _delay_ms(6000);
    }
   
    return 0;
}
 
