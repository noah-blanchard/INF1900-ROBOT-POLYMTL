#include <stdint.h>
#include <avr/io.h>

typedef volatile uint16_t *Reg;

class Wheel
{
public:
    Wheel(uint8_t wheelNumber);

    void setCompareValue(uint16_t value);
    void setForward();
    void setBackward();

    static const uint16_t MAX_COMPARE_VALUE = 0xFF;

private:
    Reg _output;
    uint8_t _wheelNumber;
};
