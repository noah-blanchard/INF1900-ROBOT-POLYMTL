#pragma once
#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "Wheel.h"
#include "LineMaker.h"
#include "Types.h"
#include "Timer.h"

enum class NavigationState
{
    NEXT_MOVE,
    FORWARD,
    FORWARD_DELAY,
    TURN_RIGHT,
    TURN_LEFT,
};

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

    // function to follow follow a trip stored as an array of moves, switch case, state machine
    void followTrip(Move *trip);

private:
    Wheel _leftWheel;
    Wheel _rightWheel;

    NavigationState _tripState = NavigationState::NEXT_MOVE;

    static const uint8_t _BASE_SPEED = 180;
    static const uint8_t _ADJUST_OFFSET = 50;

    // trip movements
    void _nextMove(Move nextMove);
    void _moveForward(uint16_t speed); // will just follow the line until the next crossroad
    void _moveForwardDelay(uint16_t speed);
    void _turnRight();
    void _turnLeft();

    void _chooseForwardMove();
    void _updateCurrentPosition();
    void _timerOn();
    void _timerOff();

    // trip variables
    uint8_t _currentPosition[2];
    Orientation _currentOrientation;
    LineMaker _lineMakerModule;
    Move _nextMove;
    Timer _delayTimerModule;

    // wheel setups
    void _forward();
    void _backward();
    void _leftForward();
    void _rightForward();
    void _leftBackward();
    void _rightBackward();

    static uint16_t _validateSpeed(uint16_t speed);
};
