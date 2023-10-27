#define F_CPU 8000000UL
#include <util/delay.h>
#include "LED.h"

constexpr uint8_t DELAY_AMBER_COLOR = 10;

LED::LED(Register port, Register mode, uint8_t greenLed, uint8_t redLed)
    : _port(port), _mode(mode), _greenLed(greenLed), _redLed(redLed)
{
    *mode |= (1 << greenLed) | (1 << redLed);
}

void LED::turnOffLed()
{
    *_port &= ~((1 << _greenLed) | (1 << _redLed));
}

void LED::turnLedRed()
{
    turnOffLed();
    *_port |= (1 << _redLed);
}

void LED::turnLedGreen()
{
    turnOffLed();
    *_port |= (1 << _greenLed);
}

void LED::turnLedAmber()
{
    turnLedGreen();
    _delay_ms(DELAY_AMBER_COLOR);
    turnLedRed();
    _delay_ms(DELAY_AMBER_COLOR);
}
