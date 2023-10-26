#include <stdint.h>
#include <avr/io.h>

typedef volatile uint16_t *Register;

class Wheel
{
public:
    Wheel(uint8_t wheelNp);

    void setCompareValue(uint16_t value);
    void setForward();
    void setBackward();

    static const uint16_t MAX_COMPARE_VALUE = 0xFF;

private:
    Register pOutput;
    uint8_t wheelN;
};
