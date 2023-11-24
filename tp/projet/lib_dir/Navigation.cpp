/**
 * @file Navigation.cpp
 * @brief Implementation of the Navigation class.
 */
#include "Navigation.h"

volatile bool delayElapsed = false;
ISR(TIMER2_COMPA_vect)
{
    delayElapsed = true;
}

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
Navigation::Navigation() : _leftWheel(0), _rightWheel(1), _display(&DDRC, &PORTC)
{
    DDRD = (1 << PD4) | (1 << PD5) | (1 << PD6) | (1 << PD7);
    PORTD |= (1 << PD5);
    PORTD |= (1 << PD4);
    PORTD &= ~(1 << PD6);
    PORTD &= ~(1 << PD7);

    _nextMoveValue.x = 0;
    _nextMoveValue.y = 0;
    _nextMoveValue.orientation = Orientation::SOUTH;

    TimerConfig timerConfig;
    timerConfig.timer = 2;
    timerConfig.prescaler = 256;
    timerConfig.delay_ms = 6000;

    _delayTimerModule = Timer(timerConfig);
}

/**
 * @brief Sets the left wheel to move forward.
 *
 */
void Navigation::_leftForward()
{
    // PORTD |= (1 << PD4);
    PORTD &= ~(1 << PD6);
}

/**
 * @brief Sets the right wheel to move forward.
 *
 */
void Navigation::_rightForward()
{
    // PORTD |= (1 << PD5);
    PORTD &= ~(1 << PD7);
}

/**
 * @brief Sets the left wheel to move backward.
 *
 */
void Navigation::_leftBackward()
{
    PORTD |= (1 << PD6);
    // PORTD &= ~(1 << PD4);
}

/**
 * @brief Sets the right wheel to move backward.
 *
 */
void Navigation::_rightBackward()
{
    PORTD |= (1 << PD7);
    // PORTD &= ~(1 << PD5);
}

/**
 * @brief Sets both wheels to move forward.
 *
 */
void Navigation::_forward()
{
    this->_leftForward();
    this->_rightForward();
}

/**
 * @brief Sets both wheels to move backward.
 *
 */
void Navigation::_backward()
{
    this->_leftBackward();
    this->_rightBackward();
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
        this->_backward();
    }
    else
    {
        this->_forward();
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
        this->_leftBackward();
    }
    else
    {
        this->_leftForward();
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
        this->_rightBackward();
    }
    else
    {
        this->_rightForward();
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

/**
 * @brief Send more power in right wheel
 *
 */
void Navigation::adjustRight()
{
    goRightWheel(_BASE_SPEED + _ADJUST_OFFSET, false);
    goLeftWheel(_BASE_SPEED, false);
    _delay_ms(10);
}

/**
 * @brief Send more power in left wheel
 *
 */
void Navigation::adjustLeft()
{
    goRightWheel(_BASE_SPEED, false);
    goLeftWheel(_BASE_SPEED + _ADJUST_OFFSET, false);
    _delay_ms(10);
}

void Navigation::adjustForward()
{
    go(_BASE_SPEED, false);
    _delay_ms(2000);
    stop();
    _delay_ms(200);
}

void Navigation::turnLeft()
{
    goRightWheel(_TURN_SPEED, false);
    goLeftWheel(_BACK_SPEED, true);
}

void Navigation::turnRight()
{
    goRightWheel(_BACK_SPEED, true);
    goLeftWheel(_TURN_SPEED, false);
}

// FOLLOW TRIP IMPLEMENTATION

void Navigation::followTrip(Move *trip)
{
    // uint8_t tripIndex = 0;
    _currentOrientation = Orientation::EAST;
    _currentPosition[0] = 0; // 0 being x
    _currentPosition[1] = 0; // 1 being y

    while (trip[_tripIndex].orientation != Orientation::FINISHED)
    {
        switch (_tripState)
        {
        case NavigationState::NEXT_MOVE:
        {
            _nextMove(trip[_tripIndex]);
            break;
        }
        case NavigationState::FORWARD:
        {
            _moveForward(_BASE_SPEED);
            break;
        }
        case NavigationState::TURN_RIGHT:
        {
            _turnRight();
            break;
        }
        case NavigationState::TURN_LEFT:
        {
            _turnLeft();
            break;
        }
        case NavigationState::FORWARD_DELAY:
        {
            _moveForwardDelay(_BASE_SPEED);
            break;
        }
        }
    }
}

void Navigation::_timerOn()
{
    cli();
    _delayTimerModule.reset();
    _delayTimerModule.enable();
    sei();
}

void Navigation::_timerOff()
{
    cli();
    _delayTimerModule.reset();
    _delayTimerModule.disable();
    sei();
}

void Navigation::_chooseForwardMove()
{
    //_tripState = NavigationState::FORWARD;
    // some place have no crossroad and need to use FORWARD_DELAY
    // x = 1, y = 0, 1 3, 5 2, 6, 2
    // so put state to forward delay if we have one of these positions
    _initForward();

    if ((_nextMoveValue.x == 1 && _nextMoveValue.y == 0) ||
        (_nextMoveValue.x == 1 && _nextMoveValue.y == 3) ||
        (_nextMoveValue.x == 5 && _nextMoveValue.y == 2) ||
        (_nextMoveValue.x == 6 && _nextMoveValue.y == 2))
    {
        _display = "FORWARD DELAY";
        _tripState = NavigationState::FORWARD_DELAY;
    }
    else
    {
        _display = "FORWARD NORMAL";
        _tripState = NavigationState::FORWARD;
    }
}

void Navigation::_updateCurrentPosition()
{
    _currentPosition[0] = _nextMoveValue.x;
    _currentPosition[1] = _nextMoveValue.y;
    _currentOrientation = _nextMoveValue.orientation;
}

void Navigation::_nextMove(Move nextMove)
{
    // so here we need to compare the current position and orientation with next move
    // if it's the same, state go forward
    // if it's not the same, state turn right or left depending on the orientation
    // for example, if current is south and next is east, turn left
    // if current is south and next is west, turn right
    stop();
    _delay_ms(1000);
    _updateCurrentPosition();
    _nextMoveValue = nextMove;
    _display = "NEXT MOVE";

    if (_currentOrientation == nextMove.orientation)
    {
        _chooseForwardMove();
    }
    else
    {
        _display = "TURN";
        switch (_currentOrientation)
        {
        case Orientation::NORTH:
        {
            if (nextMove.orientation == Orientation::EAST)
            {
                _initTurnRight();
                _tripState = NavigationState::TURN_RIGHT;
                _display = "TURN RIGHT";
            }
            else if (nextMove.orientation == Orientation::WEST)
            {
                _initTurnLeft();
                _tripState = NavigationState::TURN_LEFT;
                _display = "TURN LEFT";
            }
            break;
        }
        case Orientation::EAST:
        {
            if (nextMove.orientation == Orientation::SOUTH)
            {
                _initTurnRight();
                _tripState = NavigationState::TURN_RIGHT;
                _display = "TURN RIGHT";
            }
            else if (nextMove.orientation == Orientation::NORTH)
            {
                _initTurnLeft();
                _tripState = NavigationState::TURN_LEFT;
                _display = "TURN LEFT";
            }
            break;
        }
        case Orientation::SOUTH:
        {
            if (nextMove.orientation == Orientation::WEST)
            {
                _initTurnRight();
                _tripState = NavigationState::TURN_RIGHT;
            }
            else if (nextMove.orientation == Orientation::EAST)
            {
                _initTurnLeft();
                _tripState = NavigationState::TURN_LEFT;
                _display = "TURN LEFT";
            }
            break;
        }
        case Orientation::WEST:
        {
            if (nextMove.orientation == Orientation::NORTH)
            {
                _initTurnRight();
                _tripState = NavigationState::TURN_RIGHT;
                _display = "TURN RIGHT";
            }
            else if (nextMove.orientation == Orientation::SOUTH)
            {
                _initTurnLeft();
                _tripState = NavigationState::TURN_LEFT;
                _display = "TURN LEFT";
            }
            break;
        }
        }
    }
}

void Navigation::_moveForward(uint16_t speed)
{
    // so here we need to follow the line until we reach the next crossroad
    // so we need to check if we are at a crossroad
    // if we are, we need to update the current position and orientation
    // if we are not, we need to keep following the line

    LineMakerFlag lineMakerFlag = _lineMakerModule.getDetectionFlag();
    _display = "MOVE FORWARD NOW";

    switch (lineMakerFlag)
    {
    case LineMakerFlag::NO_ADJUSTMENT:
    {
        go(speed, false);
        break;
    }
    case LineMakerFlag::RIGHT_ADJUSTMENT:
    {
        adjustLeft();
        break;
    }
    case LineMakerFlag::LEFT_ADJUSTMENT:
    {
        adjustRight();
        break;
    }
    case LineMakerFlag::OUTER_LEFT_DETECTION:
    {
        _tripIndex++;
        _tripState = NavigationState::NEXT_MOVE;
        break;
    }
    case LineMakerFlag::OUTER_RIGHT_DETECTION:
    {
        _tripIndex++;
        _tripState = NavigationState::NEXT_MOVE;
        break;
    }
    }
}

void Navigation::_moveForwardDelay(uint16_t speed)
{
    // so here we will user timer 2 to trigger interrupt and stop moving forward in this case

    LineMakerFlag lineMakerFlag = _lineMakerModule.getDetectionFlag();

    // make a for loop to make that it does the following during 8000 ms

    for (uint16_t i = 0; i < 8000 / 220; i++)
    {

        switch (lineMakerFlag)
        {
        case LineMakerFlag::NO_ADJUSTMENT:
        {
            go(speed, false);
            _delay_ms(110);
            stop();
            _delay_ms(110);
            break;
        }
        case LineMakerFlag::RIGHT_ADJUSTMENT:
        {
            adjustLeft();
            _delay_ms(110);
            stop();
            _delay_ms(110);
            break;
        }
        case LineMakerFlag::LEFT_ADJUSTMENT:
        {
            adjustRight();
            _delay_ms(110);
            stop();
            _delay_ms(110);
            break;
        }
        }
    }
    stop();
    _tripIndex++;
    _tripState = NavigationState::NEXT_MOVE;

    // if (delayElapsed)
    // {
    //     stop();
    //     _timerOff();
    //     _tripIndex++;
    //     delayElapsed = false;
    //     _tripState = NavigationState::NEXT_MOVE;
    // }
}

void Navigation::_initForward()
{
    // adjustForward();
}

void Navigation::_initTurnRight()
{
    go(_BASE_SPEED + _ADJUST_OFFSET, false);
    _delay_ms(1650);
    stop();
    _delay_ms(200);
    // then turn a bit right for 1 second
    turnRight();
    _delay_ms(800);
    stop();
}

void Navigation::_initTurnLeft()
{
    go(_BASE_SPEED + _ADJUST_OFFSET, false);
    _delay_ms(1650);
    stop();
    _delay_ms(200);
    // then turn a bit left for 1 second
    turnLeft();
    _delay_ms(800);
    stop();
}
void Navigation::_turnRight()
{
    // so here we need to turn right
    // so we need to update the current position and orientation
    // and then we need to go forward
    // turn right until we detect the line

    // the first steps are for adjusting when arriving at the crossroad
    // first go forward for 1 and a half second

    LineMakerFlag lineMakerFlag = _lineMakerModule.getDetectionFlag();

    switch (lineMakerFlag)
    {
    case LineMakerFlag::NO_LINE:
    {
        // if we don't detect the line, we need to turn right until we detect it
        turnRight();
        break;
    }
    case LineMakerFlag::RIGHT_ADJUSTMENT:
    {
        // if we detect the line on the left, it means we met the line
        // so stop moving and go to forward state
        stop();
        _chooseForwardMove();
        break;
    }
    }
}

void Navigation::_turnLeft()
{
    // so here we need to turn left
    // so we need to update the current position and orientation
    // and then we need to go forward
    // turn left until we detect the line

    // the first steps are for adjusting when arriving at the crossroad
    // first go forward for 1 and a half second;

    LineMakerFlag lineMakerFlag = _lineMakerModule.getDetectionFlag();

    switch (lineMakerFlag)
    {
    case LineMakerFlag::NO_LINE:
    {
        // if we don't detect the line, we need to turn left until we detect it
        turnLeft();
        break;
    }
    case LineMakerFlag::LEFT_ADJUSTMENT:
    {
        // if we detect the line on the left, it means we met the line
        // so stop moving and go to forward state
        stop();
        _chooseForwardMove();
        break;
    }
    }
}