#define F_CPU 8000000UL
#include <util/delay.h>
#include "LED.h"

constexpr uint8_t DELAY_AMBER_COLOR = 10;

Led::Led(Register port, Register mode, uint8_t greenLed, uint8_t redLed)
    :pPort(port), pMode(mode), pGreenLed(greenLed), pRedLed(redLed){
        *mode |= (1<< greenLed) | (1 << redLed);
    }


void Led::turnOffLed()
{
    *pPort &= ~ ((1<< pGreenLed) | (1 << pRedLed));
}

void Led::turnLedRed()
{
    turnOffLed();
    *pPort |=  (1<< pRedLed);
}

void Led::turnLedGreen()
{
    turnOffLed();
    *pPort |= (1<< pGreenLed);
}

void Led::turnLedAmber()
{
    turnLedGreen();
    _delay_ms(DELAY_AMBER_COLOR); 
   turnLedRed();
    _delay_ms(DELAY_AMBER_COLOR); 

}





