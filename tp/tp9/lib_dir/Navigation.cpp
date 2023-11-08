/**
 * @file Navigation.cpp
 * @brief Implementation of the Navigation class.
 */
#include "Navigation.h"

/**
 * @brief Validates the speed value to ensure it is within the range of 0 to Wheel::MAX_COMPARE_VALUE.
 * @param speed The speed value to validate.
 * @return uint16_t The validated speed value.
 */
uint16_t Navigation::_validateSpeed(uint16_t speed)
{
    if (speed > Wheel::MAX_COMPARE_VALUE)
        speed = Wheel::MAX_COMPARE_VALUE;
    else if (speed < 0)
        speed = 0;

    return speed;
}

/**
 * @brief Constructs a new Navigation object and initializes the DDRD register.
 *
 */
Navigation::Navigation() : _leftWheel(0), _rightWheel(1)
{
    DDRD = (1 << PD4) | (1 << PD5) | (1 << PD6) | (1 << PD7);
    PORTD |= (1 << PD5);
    PORTD |= (1 << PD4);
    PORTD &= ~(1 << PD6);
    PORTD &= ~(1 << PD7);
}

/**
 * @brief Sets the left wheel to move forward.
 *
 */
void Navigation::leftForward()
{
    // PORTD |= (1 << PD4);
    PORTD &= ~(1 << PD6);
}

/**
 * @brief Sets the right wheel to move forward.
 *
 */
void Navigation::rightForward()
{
    // PORTD |= (1 << PD5);
    PORTD &= ~(1 << PD7);
}

/**
 * @brief Sets the left wheel to move backward.
 *
 */
void Navigation::leftBackward()
{
    PORTD |= (1 << PD6);
    // PORTD &= ~(1 << PD4);
}

/**
 * @brief Sets the right wheel to move backward.
 *
 */
void Navigation::rightBackward()
{
    PORTD |= (1 << PD7);
    // PORTD &= ~(1 << PD5);
}

/**
 * @brief Sets both wheels to move forward.
 *
 */
void Navigation::forward()
{
    this->leftForward();
    this->rightForward();
}

/**
 * @brief Sets both wheels to move backward.
 *
 */
void Navigation::backward()
{
    this->leftBackward();
    this->rightBackward();
}

/**
 * @brief Sets both wheels to move at the given speed and direction.
 *
 * @param speed The speed value to set the wheels to move at.
 * @param backward A boolean value indicating whether the wheels should move backward or forward.
 */
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

/**
 * @brief Sets the left wheel to move at the given speed and direction.
 *
 * @param speed The speed value to set the left wheel to move at.
 * @param backward A boolean value indicating whether the left wheel should move backward or forward.
 */
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

/**
 * @brief Sets the right wheel to move at the given speed and direction.
 *
 * @param speed The speed value to set the right wheel to move at.
 * @param backward A boolean value indicating whether the right wheel should move backward or forward.
 */
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

/**
 * @brief Stops both wheels from moving.
 *
 */
void Navigation::stop()
{
    this->stopLeft();
    this->stopRight();
}

/**
 * @brief Stops the left wheel from moving.
 *
 */
void Navigation::stopLeft()
{
    _leftWheel.setCompareValue(0);
    PORTD &= ~(1 << PD4);
    PORTD &= ~(1 << PD6);
}

/**
 * @brief Stops the right wheel from moving.
 *
 */
void Navigation::stopRight()
{
    _rightWheel.setCompareValue(0);
    PORTD &= ~(1 << PD5);
    PORTD &= ~(1 << PD7);
}