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
    void stop();
    void turnLeftWheel(uint16_t speed);
    void turnRightWheel(uint16_t speed);
    void spin();

private:
    Wheel leftWheel;
    Wheel rightWheel;

    static uint16_t validateSpeed(uint16_t speed);
};