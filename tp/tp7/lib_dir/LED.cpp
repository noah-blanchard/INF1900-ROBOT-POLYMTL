#define F_CPU 8000000UL
#include <util/delay.h>
#include "LED.h"

constexpr uint8_t DELAY_AMBER_COLOR = 10;

LED::LED(Register port, Register mode, uint8_t greenLed, uint8_t redLed)
    : pPort(port), pMode(mode), pGreenLed(greenLed), pRedLed(redLed)
{
    *mode |= (1 << greenLed) | (1 << redLed);
}

void LED::turnOffLed()
{
    *pPort &= ~((1 << pGreenLed) | (1 << pRedLed));
}

void LED::turnLedRed()
{
    turnOffLed();
    *pPort |= (1 << pRedLed);
}

void LED::turnLedGreen()
{
    turnOffLed();
    *pPort |= (1 << pGreenLed);
}

void LED::turnLedAmber()
{
    turnLedGreen();
    _delay_ms(DELAY_AMBER_COLOR);
    turnLedRed();
    _delay_ms(DELAY_AMBER_COLOR);
}
