/**
 * @file Navigation.cpp
 * @brief Implementation of the Navigation class methods.
 * 
 * This file contains the implementation of the Navigation class methods, which control the movement of a robot.
 * The Navigation class uses two Wheel objects to control the speed and direction of the robot's movement.
 * The class provides methods to move the robot forward, backward, turn left or right, spin in place, and stop.
 * The class also includes a private method to validate the speed input to ensure it is within the acceptable range.
 */

#include "Navigation.h"

/**
 * @brief Validates the given speed value to ensure it is within the acceptable range.
 * 
 * If the speed is greater than the maximum compare value, it is set to the maximum compare value.
 * If the speed is less than 0, it is set to 0.
 *
 * @param speed The speed value to validate.
 * @return The validated speed value.
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
 * @brief Constructs a new Navigation object with default values.
 * 
 * The Navigation object uses two Wheel objects to control the speed and direction of the robot's movement.
 * The left wheel is connected to pin 0 and the right wheel is connected to pin 1.
 */
Navigation::Navigation() : _leftWheel(0), _rightWheel(1)
{
    
}

/**
 * @brief Moves the robot forward.
 * 
 * This method sets the direction of both wheels to forward, causing the robot to move forward.
 */
void Navigation::forward()
{
    PORTD &= ~(1 << PD6) | (1<< PD7);
}

/**
 * @brief Moves the robot backward.
 * 
 * This method sets the direction of both wheels to backward, causing the robot to move backward.
 */
void Navigation::backward()
{
    PORTD |= (1 << PD6)| (1<< PD7);
}

/**
 * @brief Moves the robot at the given speed.
 * 
 * This method sets the compare value of both wheels to the given speed, causing the robot to move at the specified speed.
 * The speed value is validated to ensure it is within the acceptable range.
 *
 * @param speed The speed at which to move the robot.
 */
void Navigation::go(uint16_t speed)
{
    speed = _validateSpeed(speed);

    _leftWheel.setCompareValue(speed);
    _rightWheel.setCompareValue(speed);
}

/**
 * @brief Stops the robot.
 * 
 * This method sets the compare value of both wheels to 0, causing the robot to stop moving.
 */
void Navigation::stop()
{
    _leftWheel.setCompareValue(0);
    _rightWheel.setCompareValue(0);
}

/**
 * @brief Turns the left wheel of the robot at the given speed.
 * 
 * This method sets the compare value of the left wheel to the given speed, causing the left wheel to turn at the specified speed.
 * The speed value is validated to ensure it is within the acceptable range.
 *
 * @param speed The speed at which to turn the left wheel.
 */
void Navigation::turnLeftWheel(uint16_t speed)
{
    speed = _validateSpeed(speed);
    _leftWheel.setCompareValue(Wheel::MAX_COMPARE_VALUE);
}

/**
 * @brief Turns the right wheel of the robot at the given speed.
 * 
 * This method sets the compare value of the right wheel to the given speed, causing the right wheel to turn at the specified speed.
 * The speed value is validated to ensure it is within the acceptable range.
 *
 * @param speed The speed at which to turn the right wheel.
 */
void Navigation::turnRightWheel(uint16_t speed)
{
    speed = _validateSpeed(speed);
    _rightWheel.setCompareValue(Wheel::MAX_COMPARE_VALUE);
}

/**
 * @brief Spins the robot in place.
 * 
 * This method sets the direction of the left wheel to backward and the direction of the right wheel to forward, causing the robot to spin in place.
 */
void Navigation::spin()
{
    PORTD |= (1 << PD6);
    PORTD &= ~(1 << PD7);
}