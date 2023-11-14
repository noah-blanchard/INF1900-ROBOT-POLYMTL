#include <avr/libc.h>
#include "Navigation.h"

#define OUTER_LEFT_SENSOR PA2;
#define INNER_LEFT_SENSOR PA4;
#define MIDDLE_SENSOR PA5;
#define INNER_RIGHT_SENSOR PA6;
#define OUTER_RIGHT_SENSOR PA7;
#define DDR_SENSOR DDRA;
#define PORT_SENSOR PORTA;
#define PIN_SENSOR PINA;

class LineMaker
{

public:
    LineMaker() {}
    ~LineMaker() {}

    void adjustTrajectory();

    const uint8_t NONE = 0b00000;
    const uint8_t OUTER_LEFT = 0b00001;
    const uint8_t INNER_LEFT = 0b00010;
    const uint8_t MIDDLE = 0b00100;
    const uint8_t INNER_RIGHT = 0b01000;
    const uint8_t OUTER_RIGHT = 0b10000;
    const uint8_t ALL = OUTER_LEFT | INNER_LEFT | MIDDLE | INNER_RIGHT | OUTER_RIGHT;

private:

    uint8_t retrieveSensorData();
    Navigation _nav;

    const uint8_t baseSpeed = 200;
    const uint8_t adjustSpeed = 255;

}
