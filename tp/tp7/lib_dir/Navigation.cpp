#include "Navigation.h"

uint16_t Navigation::_validateSpeed(uint16_t speed)
{
    if (speed > Wheel::MAX_COMPARE_VALUE)
        speed = Wheel::MAX_COMPARE_VALUE;
    else if (speed < 0)
        speed = 0;

    return speed;
}

Navigation::Navigation() : _leftWheel(0), _rightWheel(1)
{
    DDRD = (1 << PD4) | (1 << PD5) | (1 << PD6) | (1 << PD7);
}

void Navigation::leftForward()
{
    PORTD |= (1 << PD4);
    PORTD &= ~(1 << PD6);
}

void Navigation::rightForward()
{
    PORTD |= (1 << PD5);
    PORTD &= ~(1 << PD7);
}

void Navigation::leftBackward()
{
    PORTD |= (1 << PD6);
    PORTD &= ~(1 << PD4);
}

void Navigation::rightBackward()
{
    PORTD |= (1 << PD7);
    PORTD &= ~(1 << PD5);
}

void Navigation::forward()
{
    this->leftForward();
    this->rightForward();
}

void Navigation::backward()
{
    this->leftBackward();
    this->rightBackward();
}

void Navigation::go(uint16_t speed, bool backward)
{
    speed = _validateSpeed(speed);

    if (backward)
    {
        this->backward();
    }
    else
    {
        this->forward();
    }

    _leftWheel.setCompareValue(speed);
    _rightWheel.setCompareValue(speed);
}

void Navigation::goLeftWheel(uint16_t speed, bool backward)
{
    speed = _validateSpeed(speed);

    if (backward)
    {
        this->leftBackward();
    }
    else
    {
        this->leftForward();
    }

    _leftWheel.setCompareValue(speed);
}

void Navigation::goRightWheel(uint16_t speed, bool backward)
{
    speed = _validateSpeed(speed);

    if (backward)
    {
        this->rightBackward();
    }
    else
    {
        this->rightForward();
    }

    _rightWheel.setCompareValue(speed);
}

void Navigation::stop()
{
    this->stopLeft();
    this->stopRight();
}

void Navigation::stopLeft()
{
    _leftWheel.setCompareValue(0);
    PORTD &= ~(1 << PD4);
    PORTD &= ~(1 << PD6);
}

void Navigation::stopRight()
{
    _rightWheel.setCompareValue(0);
    PORTD &= ~(1 << PD5);
    PORTD &= ~(1 << PD7);
}