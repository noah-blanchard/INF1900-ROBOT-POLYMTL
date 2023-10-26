#include "Wheel.h"

Wheel::Wheel(uint8_t wheelNp) : wheelN(wheelNp)
{
    switch (wheelN)
    {
    case 0:
        pOutput = &OCR1B;
        break;
    case 1:
        pOutput = &OCR1A;
        break;
    }

    TCCR1A = (1 << WGM10) | (1 << COM1A1) | (1 << COM1B1);

    TCCR1B = (1 << CS11);

    TCCR1C = 0;
}

void Wheel::setCompareValue(uint16_t value)
{
    *pOutput = value;
}