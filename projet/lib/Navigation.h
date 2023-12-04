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
#include "Consts.h"
#include "Sound.h"

enum class NavigationState
{
    NEXT_MOVE,
    FORWARD,
    FORWARD_DELAY,
    TURN_RIGHT,
    TURN_LEFT,
    CHOOSE_RIGHT_TURN,
    MEET_POST,
    ERROR,
    GO_BACK,
    TURN_RIGHT_180,
    TURN_LEFT_180
};

class Navigation
{
public:
    Navigation();
    Navigation(uint8_t *robotPosition, Orientation *robotOrientation);

    void go(bool backward);
    void goLeftWheel(uint8_t speed, bool backward);
    void goRightWheel(uint8_t speed, bool backward);
    void stop();
    void stopLeft();
    void stopRight();

    void adjustRight();
    void adjustLeft();
    void adjustForward();
    void adjustForward(uint16_t amount);
    void turnRight();
    void turnLeft();

    // function to follow follow a trip stored as an array of moves, switch case, state machine
    Move followTrip(Move *trip);
    void parking();

private:
    // CONSTANTS
    static const uint8_t _BASE_SPEED = BASE_WHEEL_SPEED;
    static const uint8_t _BACK_SPEED = BACK_WHEEL_SPEED;
    static const uint8_t _TURN_SPEED = TURN_WHEEL_SPEED;
    static const uint8_t _ADJUST_OFFSET = LINE_ADJUST_OFFSET;
    static const uint8_t _OFFSET = OFFSET;
    static const uint8_t _ADJUST_DELAY = ADJUST_DELAY;

    Wheel _leftWheel;
    Wheel _rightWheel;

    NavigationState _tripState = NavigationState::NEXT_MOVE;

    // trip movements
    void _nextMove();
    void _moveForward();      // will just follow the line until the next crossroad
    void _moveForwardDelay(); /// will go forward for a specific delay (used for places with no crossroad)
    void _meetPost();
    void _goBack();
    void _turnRight();
    void _turnLeft();
    void _initTurnRight();
    void _initTurnLeft();
    void _initForward();
    void _chooseRightTurn();
    void _turnRight180();
    void _turnLeft180();

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
    LineMakerFlag _lastCrossroad;
    uint8_t _turn180Count = 0;

    // Modules
    LineMaker _lineMakerModule;
    InfraRedSensor _irModule;
    LCM _display;
    Sound _soundModule;

    // wheel controls
    void _forward();
    void _backward();
    void _leftForward();
    void _rightForward();
    void _leftBackward();
    void _rightBackward();

    static uint16_t _validateSpeed(uint16_t speed);
};
