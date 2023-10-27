/**
 * @file Wheel.cpp
 * @brief Implementation of the Wheel class
*/
#include "Wheel.h"

/**
 * @brief Constructor for Wheel class
 * 
 * @param wheelN The wheel number (0 or 1)
 */
Wheel::Wheel(uint8_t wheelN) : _wheelNumber(wheelN)
{
    switch (_wheelNumber)
    {
    case 0:
        _output = &OCR1B;
        break;
    case 1:
        _output = &OCR1A;
        break;
    }

    TCCR1A = (1 << WGM10) | (1 << COM1A1) | (1 << COM1B1);

    TCCR1B = (1 << CS11);

    TCCR1C = 0;
}

/**
 * @brief Set the compare value for the wheel
 * 
 * @param value The compare value to set
 */
void Wheel::setCompareValue(uint16_t value)
{
    *_output = value;
}