#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>
#include "Wheel.h"

class Navigation
{
public:
    Navigation();

    void go(uint16_t speed, bool backward);
    void goLeftWheel(uint16_t speed, bool backward);
    void goRightWheel(uint16_t speed, bool backward);
    void stop();
    void stopLeft();
    void stopRight();

    void adjustRight();
    void adjustLeft();

private:
    Wheel _leftWheel;
    Wheel _rightWheel;

    static const uint8_t _BASE_SPEED = 180;
    static const uint8_t _ADJUST_OFFSET = 50;

    void
    forward();
    void backward();
    void leftForward();
    void rightForward();
    void leftBackward();
    void rightBackward();

    static uint16_t _validateSpeed(uint16_t speed);
};
