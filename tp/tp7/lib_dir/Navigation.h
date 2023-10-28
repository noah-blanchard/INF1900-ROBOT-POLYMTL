#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>
#include "Wheel.h"

class Navigation
{
public:
    Navigation();

    void forward();
    void backward();
    void go(uint16_t speed);
    void goLeftWheel()
    void stop();
    void spin();
    void leftForward();
    void rightForward();
    void leftBackward();
    void rightBackward();
    void stopLeft();
    void stopRight();

private:
    Wheel _leftWheel;
    Wheel _rightWheel;

    static uint16_t _validateSpeed(uint16_t speed);
};
