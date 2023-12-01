/**
 * @file LED.cpp
 * @brief Implementation of the LED class.
 */

#define F_CPU 8000000UL
#include <util/delay.h>
#include "LED.h"

#define CPU_FREQ 8000000UL // 8 MHz
#define PRESCALER 1024
#define OVERFLOW_FREQ (CPU_FREQ / (PRESCALER * 256))

volatile uint8_t counter = 0;
const uint8_t countsRequired = OVERFLOW_FREQ / 4;

ISR(TIMER2_OVF_vect)
{
    counter++;
    if (counter >= countsRequired)
    {
        counter = 0;
    }
}

constexpr uint8_t DELAY_AMBER_COLOR = 10;
constexpr uint8_t DELAY_FOR_FOUR_HZ = 125;

/**
 * @brief Constructor for the LED class.
 *
 * @param port The register for the LED port.
 * @param mode The register for the LED mode.
 * @param greenLed The pin number for the green LED.
 * @param redLed The pin number for the red LED.
 */
LED::LED(Register port, Register mode, uint8_t greenLed, uint8_t redLed)
    : _port(port), _mode(mode), _greenLed(greenLed), _redLed(redLed)
{
    *mode |= (1 << greenLed) | (1 << redLed);
}

/**
 * @brief Turns off both the green and red LEDs.
 */
void LED::turnOffLed()
{
    *_port &= ~((1 << _greenLed) | (1 << _redLed));
}

/**
 * @brief Turns on the red LED and turns off the green LED.
 */
void LED::turnLedRed()
{
    turnOffLed();
    *_port |= (1 << _redLed);
}

/**
 * @brief Turns on the green LED and turns off the red LED.
 */
void LED::turnLedGreen()
{
    turnOffLed();
    *_port |= (1 << _greenLed);
}

/**
 * @brief Turns on the green LED for a short period of time, then turns on the red LED for the same period of time.
 */
void LED::turnLedAmber()
{
    turnLedGreen();
    _delay_ms(DELAY_AMBER_COLOR);
    turnLedRed();
    _delay_ms(DELAY_AMBER_COLOR);
}

void LED::flashGreen()
{
    if (counter > countsRequired)
    {
        if (_on)
        {
            turnOffLed();
            _on = false;
        }
        else
        {
            turnLedGreen();
            _on = true;
        }
    }
}

void LED::setupBlink()
{
    cli();
    TCCR2A = 0;

    // Set prescaler to 1024
    TCCR2B = (1 << CS22) | (1 << CS21) | (1 << CS20);

    // Enable Timer 2 overflow interrupt
    TIMSK2 = (1 << TOIE2);
    sei();
}