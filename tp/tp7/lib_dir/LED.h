#include <stdint.h>
#define FLASHRED 0
#define FLASHGREEN 1
#define FLASHAMBER 2

typedef volatile uint8_t *Register;

class LED
{
public:
    LED(Register port, Register mode, uint8_t greenLed, uint8_t redLed);

    void turnOffLed();
    void turnLedRed();
    void turnLedGreen();
    void turnLedAmber();
    
private:
    Register _port;
    Register _mode;
    uint8_t _greenLed;
    uint8_t _redLed;
};
