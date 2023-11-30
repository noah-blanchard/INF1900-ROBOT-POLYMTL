#pragma once
#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "Wheel.h"
#include "LineMaker.h"
#include "lcm_so1602dtr_m_fw.h"
#include "Types.h"
#include "Timer.h"
#include "InfraRedSensor.h"
#include "Dijkstra.h"

enum class NavigationState
{
    NEXT_MOVE,
    FORWARD,
    FORWARD_DELAY,
    TURN_RIGHT,
    TURN_LEFT,
    MEET_POST,
    ERROR,
    GO_BACK
};

class Navigation
{
public:
    Navigation();
    Navigation(uint8_t *robotPosition, Orientation *robotOrientation);

    void go(uint16_t speed, bool backward);
    void goLeftWheel(uint16_t speed, bool backward);
    void goRightWheel(uint16_t speed, bool backward);
    void stop();
    void stopLeft();
    void stopRight();

    void adjustRight();
    void adjustLeft();
    void adjustForward();
    void turnRight();
    void turnLeft();

    // function to follow follow a trip stored as an array of moves, switch case, state machine
    Move followTrip(Move *trip);

    // CONSTANTS
    static const uint8_t _BASE_SPEED = 100;
    static const uint8_t _BACK_SPEED = 105;
    static const uint8_t _TURN_SPEED = 100;
    static const uint8_t _ADJUST_OFFSET = 30;

    static const uint8_t _MOVE_FORWARD_DELAY = 3000;
    static const uint8_t _FORWARD_ADJUST_DELAY = 2000;
    static const uint8_t _ADJUST_DELAY = 10;

private:
    Wheel _leftWheel;
    Wheel _rightWheel;

    NavigationState _tripState = NavigationState::NEXT_MOVE;

    // trip movements
    void _nextMove(Move nextMove);
    void _moveForward(uint16_t speed);      // will just follow the line until the next crossroad
    void _moveForwardDelay(uint16_t speed); /// will go forward for a specific delay (used for places with no crossroad)
    void _meetPost();
    void _goBack();
    void _turnRight();
    void _turnLeft();
    void _initTurnRight();
    void _initTurnLeft();
    void _initForward();

    void _chooseForwardMove();
    void _updateCurrentPosition();
    void _timerOn();
    void _timerOff();

    // trip variables
    Move *_trip;
    Move _nextMoveValue;
    uint8_t *_currentPosition;
    Orientation *_currentOrientation;
    uint8_t _tripIndex = 0;
    bool _firstMove = true;
    uint16_t _forwardDelayCount;
    bool _preventInitForward = false;
    // Modules
    LineMaker _lineMakerModule;
    // Timer _delayTimerModule;
    InfraRedSensor _irModule;
    LCM _display;

    // wheel controls
    void _forward();
    void _backward();
    void _leftForward();
    void _rightForward();
    void _leftBackward();
    void _rightBackward();

    static uint16_t _validateSpeed(uint16_t speed);
};
