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
    // void alternateLed(uint16_t delay);
    // void flashLed(uint16_t time, uint16_t delay,int light);
    // void flashLedGreen(uint8_t duration, uint8_t delay);
    // void flashLedRed(uint8_t duration, uint8_t delay);
    // void flashLedAmber(uint8_t duration, uint8_t delay);

private:
    Register pPort;
    Register pMode;
    uint8_t pGreenLed;
    uint8_t pRedLed;
};
