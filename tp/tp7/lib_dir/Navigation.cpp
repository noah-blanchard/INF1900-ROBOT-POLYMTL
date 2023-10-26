#include "Navigation.h"

// validate speed
uint16_t Navigation::validateSpeed(uint16_t speed)
{
    if (speed > Wheel::MAX_COMPARE_VALUE)
        speed = Wheel::MAX_COMPARE_VALUE;
    else if (speed < 0)
        speed = 0;

    return speed;
}

Navigation::Navigation() : leftWheel(0), rightWheel(1)
{
    
}

void Navigation::forward()
{
    PORTD &= ~(1 << PD6) | (1<< PD7);

}

void Navigation::backward()
{
    PORTD |= (1 << PD6)| (1<< PD7);
}

void Navigation::go(uint16_t speed)
{
    speed = validateSpeed(speed);

    leftWheel.setCompareValue(speed);
    rightWheel.setCompareValue(speed);
}

void Navigation::stop()
{
    leftWheel.setCompareValue(0);
    rightWheel.setCompareValue(0);
}

void Navigation::turnLeftWheel(uint16_t speed)
{
    speed = validateSpeed(speed);
    leftWheel.setCompareValue(Wheel::MAX_COMPARE_VALUE);
}

void Navigation::turnRightWheel(uint16_t speed)
{
    speed = validateSpeed(speed);
    rightWheel.setCompareValue(Wheel::MAX_COMPARE_VALUE);
}

void Navigation::spin()
{
    PORTD |= (1 << PD6);
   PORTD &= ~(1 << PD7);
}