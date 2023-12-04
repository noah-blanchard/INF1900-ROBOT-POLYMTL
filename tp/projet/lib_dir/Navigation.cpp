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

Navigation::Navigation() : _leftWheel(0), _rightWheel(1), _display(&DDRC, &PORTC)
{
    DDRD = (1 << PD4) | (1 << PD5) | (1 << PD6) | (1 << PD7);
    PORTD |= (1 << PD5);
    PORTD |= (1 << PD4);
    PORTD &= ~(1 << PD6);
    PORTD &= ~(1 << PD7);
}

/**
 * @brief Constructs a new Navigation object and initializes the DDRD register.
 *
 */
Navigation::Navigation(uint8_t *robotPosition, Orientation *robotOrientation) : _leftWheel(0), _rightWheel(1), _display(&DDRC, &PORTC)
{
    DDRD = (1 << PD4) | (1 << PD5) | (1 << PD6) | (1 << PD7);
    PORTD |= (1 << PD5);
    PORTD |= (1 << PD4);
    PORTD &= ~(1 << PD6);
    PORTD &= ~(1 << PD7);

    _nextMoveValue.x = robotPosition[0];
    _nextMoveValue.y = robotPosition[1];
    _nextMoveValue.orientation = *robotOrientation;
    _currentOrientation = robotOrientation;
    _currentPosition = robotPosition;
}

/**
 * @brief Sets the left wheel to move forward.
 *
 */
void Navigation::_leftForward()
{
    PORTD &= ~(1 << PD6);
}

/**
 * @brief Sets the right wheel to move forward.
 *
 */
void Navigation::_rightForward()
{

    PORTD &= ~(1 << PD7);
}

/**
 * @brief Sets the left wheel to move backward.
 *
 */
void Navigation::_leftBackward()
{
    PORTD |= (1 << PD6);
}

/**
 * @brief Sets the right wheel to move backward.
 *
 */
void Navigation::_rightBackward()
{
    PORTD |= (1 << PD7);
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
void Navigation::go(bool backward)
{
    if (backward)
    {
        this->_backward();
    }
    else
    {
        this->_forward();
    }

    goLeftWheel(_BASE_SPEED, backward);
    goRightWheel(_BASE_SPEED, backward);
}

/**
 * @brief Sets the left wheel to move at the given speed and direction.
 *
 * @param speed The speed value to set the left wheel to move at.
 * @param backward A boolean value indicating whether the left wheel should move backward or forward.
 */
void Navigation::goLeftWheel(uint8_t speed, bool backward)
{
    if (backward)
    {
        this->_leftBackward();
    }
    else
    {
        this->_leftForward();
    }

    _leftWheel.setCompareValue(speed + _OFFSET);
}

/**
 * @brief Sets the right wheel to move at the given speed and direction.
 *
 * @param speed The speed value to set the right wheel to move at.
 * @param backward A boolean value indicating whether the right wheel should move backward or forward.
 */
void Navigation::goRightWheel(uint8_t speed, bool backward)
{
    if (backward)
    {
        this->_rightBackward();
    }
    else
    {
        this->_rightForward();
    }

    _rightWheel.setCompareValue(speed - _OFFSET);
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
    _delay_ms(_ADJUST_DELAY);
}

/**
 * @brief Send more power in left wheel
 *
 */
void Navigation::adjustLeft()
{
    goRightWheel(_BASE_SPEED, false);
    goLeftWheel(_BASE_SPEED + _ADJUST_OFFSET, false);
    _delay_ms(_ADJUST_DELAY);
}

void Navigation::adjustForward(uint16_t amount)
{
    for (uint16_t delayCounter = 0; delayCounter < amount; delayCounter++)
    {
        LineMakerFlag lineMakerFlag = _lineMakerModule.getDetectionFlag();

        switch (lineMakerFlag)
        {
        case LineMakerFlag::NO_ADJUSTMENT:
        {
            go(false);
            _delay_ms(_ADJUST_DELAY);
            break;
        }
        case LineMakerFlag::RIGHT_ADJUSTMENT:
        {
            adjustLeft();
            break;
        }
        case LineMakerFlag::OUTER_RIGHT_DETECTION:
        {
            go(false);
            _delay_ms(_ADJUST_DELAY);
            break;
        }
        case LineMakerFlag::FULL_CROSSROAD:
        {
            go(false);
            _delay_ms(_ADJUST_DELAY);
            break;
        }
        case LineMakerFlag::NO_LINE:
        {
            go(false);
            _delay_ms(_ADJUST_DELAY);
        }
        break;
        case LineMakerFlag::OUTER_LEFT_DETECTION:
        {
            go(false);
            _delay_ms(_ADJUST_DELAY);
        }
        case LineMakerFlag::LEFT_ADJUSTMENT:
        {
            adjustRight();
            break;
        }
        }
    }
}

void Navigation::adjustForward()
{
    adjustForward(BASE_ADJ_FWD_AMT);
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

Move Navigation::followTrip(Move *trip)
{
    _tripIndex = 0;
    _trip = trip;
    _firstMove = true;
    _preventInitForward = true;
    _tripState = NavigationState::NEXT_MOVE;
    _nextMoveValue.x = _currentPosition[0];
    _nextMoveValue.y = _currentPosition[1];
    _nextMoveValue.orientation = *_currentOrientation;

    // uint8_t tripIndex = 0;

    while (_trip[_tripIndex].orientation != Orientation::FINISHED)
    {
        switch (_tripState)
        {
        case NavigationState::NEXT_MOVE:
        {
            _nextMove();
            break;
        }
        case NavigationState::FORWARD:
        {
            _moveForward();
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
        case NavigationState::MEET_POST:
        {
            _meetPost();
            break;
        }
        case NavigationState::FORWARD_DELAY:
        {
            _moveForwardDelay();
            break;
        }
        case NavigationState::ERROR:
        {
            stop();
            _currentPosition[0] = _nextMoveValue.x;
            _currentPosition[1] = _nextMoveValue.y;
            _delay_ms(3000);
            return _trip[_tripIndex];
            break;
        }
        case NavigationState::TURN_RIGHT_180:
        {
            _turnRight180();
            break;
        }
        case NavigationState::TURN_LEFT_180:
        {
            _turnLeft180();
            break;
        }
        case NavigationState::CHOOSE_RIGHT_TURN:
        {
            _chooseRightTurn();
            break;
        }
        }
    }

    stop();
    _display = "FINISHED";
    _updateCurrentPosition();
    _delay_ms(NAV_STOP_DELAY);

    return _trip[_tripIndex];
}

void Navigation::_chooseForwardMove()
{
    if ((_nextMoveValue.x == 1 && _nextMoveValue.y == 0) ||
        (_nextMoveValue.x == 1 && _nextMoveValue.y == 3) ||
        (_nextMoveValue.x == 5 && _nextMoveValue.y == 2) ||
        (_nextMoveValue.x == 6 && _nextMoveValue.y == 2))
    {
        _forwardDelayCount = 0;
        _tripState = NavigationState::FORWARD_DELAY;
        if (!_preventInitForward)
        {
            _initForward();
        }
    }
    else
    {
        _tripState = NavigationState::FORWARD;
        if (!_preventInitForward)
        {
            _initForward();
        }
        _preventInitForward = false;
    }
}

void Navigation::_updateCurrentPosition()
{
    _currentPosition[0] = _nextMoveValue.x;
    _currentPosition[1] = _nextMoveValue.y;
    *_currentOrientation = _nextMoveValue.orientation;
}

void Navigation::_nextMove()
{
    // so here we need to compare the current position and orientation with next move
    // if it's the same, state go forward
    // if it's not the same, state turn right or left depending on the orientation
    // for example, if current is south and next is east, turn left
    // if current is south and next is west, turn right
    stop();
    _delay_ms(NAV_STOP_DELAY);
    _updateCurrentPosition();
    _nextMoveValue = _trip[_tripIndex];

    if (*_currentOrientation == _nextMoveValue.orientation)
    {
        _chooseForwardMove();
    }
    else
    {
        switch (*_currentOrientation)
        {
        case Orientation::NORTH:
        {
            if (_nextMoveValue.orientation == Orientation::EAST)
            {
                _initTurnRight();
                _tripState = NavigationState::TURN_RIGHT;
                _display = "TURN RIGHT";
            }
            else if (_nextMoveValue.orientation == Orientation::WEST)
            {
                _initTurnLeft();
                _tripState = NavigationState::TURN_LEFT;
                _display = "TURN LEFT";
            }
            else if (_nextMoveValue.orientation == Orientation::SOUTH)
            {
                _tripState = NavigationState::CHOOSE_RIGHT_TURN;
            }
            break;
        }
        case Orientation::EAST:
        {
            if (_nextMoveValue.orientation == Orientation::SOUTH)
            {
                _initTurnRight();
                _tripState = NavigationState::TURN_RIGHT;
                _display = "TURN RIGHT";
            }
            else if (_nextMoveValue.orientation == Orientation::NORTH)
            {
                _initTurnLeft();
                _tripState = NavigationState::TURN_LEFT;
                _display = "TURN LEFT";
            }
            else if (_nextMoveValue.orientation == Orientation::WEST)
            {
                _tripState = NavigationState::CHOOSE_RIGHT_TURN;
            }
            break;
        }
        case Orientation::SOUTH:
        {
            if (_nextMoveValue.orientation == Orientation::WEST)
            {
                _initTurnRight();
                _tripState = NavigationState::TURN_RIGHT;
            }
            else if (_nextMoveValue.orientation == Orientation::EAST)
            {
                _initTurnLeft();
                _tripState = NavigationState::TURN_LEFT;
                _display = "TURN LEFT";
            }
            else if (_nextMoveValue.orientation == Orientation::NORTH)
            {
                _tripState = NavigationState::CHOOSE_RIGHT_TURN;
            }
            break;
        }
        case Orientation::WEST:
        {
            if (_nextMoveValue.orientation == Orientation::NORTH)
            {
                _initTurnRight();
                _tripState = NavigationState::TURN_RIGHT;
                _display = "TURN RIGHT";
            }
            else if (_nextMoveValue.orientation == Orientation::SOUTH)
            {
                _initTurnLeft();
                _tripState = NavigationState::TURN_LEFT;
                _display = "TURN LEFT";
            }
            else if (_nextMoveValue.orientation == Orientation::EAST)
            {
                _tripState = NavigationState::CHOOSE_RIGHT_TURN;
            }
            break;
        }
        }
    }

    _preventInitForward = false;
}

void Navigation::_moveForward()
{
    // so here we need to follow the line until we reach the next crossroad
    // so we need to check if we are at a crossroad
    // if we are, we need to update the current position and orientation
    // if we are not, we need to keep following the line

    // check if ir module detects
    if (_irModule.isObstacleDetected())
    {
        stop();
        _display = "POTITO";
        _soundModule.chooseFrequency(LOW_MIDI_NOTE);
        _delay_ms(MEET_POST_DELAY);
        _soundModule.stopSound();
        _tripState = NavigationState::MEET_POST;
    }
    else
    {

        LineMakerFlag lineMakerFlag = _lineMakerModule.getDetectionFlag();

        switch (lineMakerFlag)
        {
        case LineMakerFlag::NO_ADJUSTMENT:
        {
            go(false);
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
        case LineMakerFlag::FULL_CROSSROAD:
        {
            _lastCrossroad = LineMakerFlag::FULL_CROSSROAD;
            _tripIndex++;
            _tripState = NavigationState::NEXT_MOVE;
            break;
        }
        case LineMakerFlag::OUTER_LEFT_DETECTION:
        {
            _lastCrossroad = LineMakerFlag::OUTER_LEFT_DETECTION;
            _tripIndex++;
            _tripState = NavigationState::NEXT_MOVE;
            break;
        }
        case LineMakerFlag::OUTER_RIGHT_DETECTION:
        {
            _lastCrossroad = LineMakerFlag::OUTER_RIGHT_DETECTION;
            _tripIndex++;
            _tripState = NavigationState::NEXT_MOVE;
            break;
        }
        }
    }
}

void Navigation::_moveForwardDelay()
{
    if (_irModule.isObstacleDetected())
    {
        stop();
        _display = "POTITO";
        _delay_ms(NAV_STOP_DELAY);
        _tripState = NavigationState::MEET_POST;
    }
    else
    {
        _forwardDelayCount++;

        LineMakerFlag lineMakerFlag = _lineMakerModule.getDetectionFlag();

        switch (lineMakerFlag)
        {
        case LineMakerFlag::NO_ADJUSTMENT:
        {
            go(false);
            _delay_ms(_ADJUST_DELAY);
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
        }
    }

    if (_forwardDelayCount >= BASE_FWD_DELAY_AMT)
    {
        _display = "stop";
        stop();
        _delay_ms(NAV_STOP_DELAY);
        _forwardDelayCount = 0;
        _tripIndex++;
        _tripState = NavigationState::NEXT_MOVE;
    }
}

void Navigation::_initForward()
{
    adjustForward(LESS_ADJ_FWD_AMT);
}

void Navigation::_initTurnRight()
{
    // if it is not the first move, go forward for a bit, then turn
    if (!_firstMove || !_preventInitForward)
    {
        // go(_BASE_SPEED, false);
        // _delay_ms(1650);
        adjustForward(NAV_ADJ_FWD_AMT);
        _preventInitForward = false;
    }
    else
    {
        _firstMove = false;
    }
    stop();
    _delay_ms(INIT_TURN_STOP_DELAY);
    turnRight();
    _delay_ms(INIT_TURN_DELAY);
    stop();
}

void Navigation::_initTurnLeft()
{
    // if it is not the first move, go forward for a bit, then turn
    if (!_firstMove || !_preventInitForward)
    {
        // go(_BASE_SPEED + _ADJUST_OFFSET, false);
        // _delay_ms(1650);
        adjustForward(NAV_ADJ_FWD_AMT);
        _preventInitForward = false;
    }
    else
    {
        _firstMove = false;
    }
    stop();
    _delay_ms(INIT_TURN_STOP_DELAY);
    turnLeft();
    _delay_ms(INIT_TURN_DELAY);
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

    if (_irModule.isObstacleDetected())
    {
        _tripState = NavigationState::MEET_POST;
    }

    else
    {

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
            _delay_ms(NAV_STOP_DELAY);
            _preventInitForward = true;
            _chooseForwardMove();

            break;
        }
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

    if (_irModule.isObstacleDetected())
    {
        _updateCurrentPosition();
        _tripState = NavigationState::MEET_POST;
    }

    else
    {

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
            _delay_ms(NAV_STOP_DELAY);
            _preventInitForward = true;
            _chooseForwardMove();

            break;
        }
        }
    }
}

void Navigation::_meetPost()
{
    // turn around 180 degrees until a line is detected
    *_currentOrientation = _nextMoveValue.orientation;
    _nextMoveValue = _trip[_tripIndex - 1];
    stop();
    _delay_ms(NAV_STOP_DELAY);
    _tripState = NavigationState::ERROR;
}

void Navigation::_chooseRightTurn()
{

    // turn left pour (1,1) et nextOrientation west
    /**
     * 1, 0,
     * 1, 1
     * 2, 1
     * 1, 2
     * 2, 2
     */

    uint8_t col = _currentPosition[0];
    uint8_t row = _currentPosition[1];
    _preventInitForward = true;

    if ((col == 1) && (row == 1))
    {
        switch (_nextMoveValue.orientation)
        {
        case Orientation::EAST:
        {
            _initTurnRight();
            _tripState = NavigationState::TURN_RIGHT;
            _display = "TURN RIGHT";
            break;
        }
        case Orientation::WEST:
        {
            _initTurnLeft();
            _tripState = NavigationState::TURN_LEFT;
            _display = "TURN LEFT";
            break;
        }
        }
    }
    // really ???? col 1 raw 0 ????
    else if ((col == 1 && row == 0))
    {
        switch (_nextMoveValue.orientation)
        {
        case Orientation::EAST:
        {
            _initTurnLeft();
            _tripState = NavigationState::TURN_LEFT;
            _display = "TURN LEFT";
            break;
        }
        case Orientation::WEST:
        {
            _initTurnRight();
            _tripState = NavigationState::TURN_RIGHT;
            _display = "TURN RIGHT";
            break;
        }
        }
    }
    // 2 0
    else if ((col == 2) && (row == 0))
    {
        switch (_nextMoveValue.orientation)
        {
        case Orientation::EAST:
        {
            _initTurnRight();
            _tripState = NavigationState::TURN_RIGHT;
            _display = "TURN LEFT";
            break;
        }
        case Orientation::WEST:
        {
            _initTurnLeft();
            _tripState = NavigationState::TURN_LEFT;
            _display = "TURN RIGHT";
            break;
        }
        }
    }
    // 4 1
    else if ((col == 4) && (row == 1))
    {
        switch (_nextMoveValue.orientation)
        {
        case Orientation::EAST:
        {
            _initTurnRight();
            _tripState = NavigationState::TURN_RIGHT;
            _display = "TURN RIGHT";
            break;
        }
        case Orientation::WEST:
        {
            _initTurnLeft();
            _tripState = NavigationState::TURN_LEFT;
            _display = "TURN LEFT";
            break;
        }
        }
    }
    // 5 1
    // 2 1
    else if ((col == 2) && (row == 1))
    {
        switch (_nextMoveValue.orientation)
        {
        case Orientation::NORTH:
        {
            _initTurnLeft();
            _tripState = NavigationState::TURN_LEFT;
            _display = "TURN RIGHT";
            break;
        }
        case Orientation::SOUTH:
        {
            _initTurnRight();
            _tripState = NavigationState::TURN_RIGHT;
            _display = "TURN LEFT";
            break;
        }
        }
    }
    // 4 1 // repeated !!!!!!!!
    else if ((col == 4) && (row == 1))
    {
        switch (_nextMoveValue.orientation)
        {
        case Orientation::EAST:
        {
            _initTurnRight();
            _tripState = NavigationState::TURN_RIGHT;
            _display = "TURN LEFT";
            break;
        }
        case Orientation::WEST:
        {
            _initTurnLeft();
            _tripState = NavigationState::TURN_LEFT;
            _display = "TURN RIGHT";
            break;
        }
        }
    }
    // 6 1
    else if ((col == 6) && (row == 1))
    {
        switch (_nextMoveValue.orientation)
        {
        case Orientation::SOUTH:
        {
            _initTurnRight();
            _tripState = NavigationState::TURN_RIGHT;
            _display = "TURN RIGHT";
            break;
        }
        case Orientation::NORTH:
        {
            _initTurnLeft();
            _tripState = NavigationState::TURN_LEFT;
            _display = "TURN LEFT";
            break;
        }
        }
    }
    // 1 2
    else if ((col == 1) && (row == 2))
    {
        switch (_nextMoveValue.orientation)
        {
        case Orientation::WEST:
        {
            _initTurnRight();
            _tripState = NavigationState::TURN_RIGHT;
            _display = "TURN RIGHT";
            break;
        }
        case Orientation::EAST:
        {
            _initTurnLeft();
            _tripState = NavigationState::TURN_LEFT;
            _display = "TURN LEFT";
            break;
        }
        }
    }
    // 3 2
    else if ((col == 3) && (row == 2))
    {
        switch (_nextMoveValue.orientation)
        {
        case Orientation::WEST:
        {
            _initTurnLeft();
            _tripState = NavigationState::TURN_LEFT;
            _display = "TURN LEFT";
            break;
        }
        case Orientation::EAST:
        {
            _initTurnRight();
            _tripState = NavigationState::TURN_RIGHT;
            _display = "TURN RIGHT";
            break;
        }
        }
    }
    // 4 2
    else if ((col == 4) && (row == 2))
    {
        switch (_nextMoveValue.orientation)
        {
        case Orientation::NORTH:
        {
            _initTurnLeft();
            _tripState = NavigationState::TURN_LEFT;
            _display = "TURN RIGHT";
            break;
        }
        case Orientation::SOUTH:
        {
            _initTurnRight();
            _tripState = NavigationState::TURN_RIGHT;
            _display = "TURN LEFT";
            break;
        }
        }
    }
    else if ((col == 1) && (row == 3))
    {
        switch (_nextMoveValue.orientation)
        {
        case Orientation::EAST:
        {
            _initTurnLeft();
            _tripState = NavigationState::TURN_LEFT;
            _display = "TURN LEFT";
            break;
        }
        case Orientation::WEST:
        {
            _initTurnRight();
            _tripState = NavigationState::TURN_RIGHT;
            _display = "TURN RIGHT";
            break;
        }
        }
    }
    // 4 3
    else if ((col == 4) && (row == 3))
    {
        switch (_nextMoveValue.orientation)
        {
        case Orientation::EAST:
        {
            _initTurnLeft();
            _tripState = NavigationState::TURN_LEFT;
            _display = "TURN LEFT";
            break;
        }
        case Orientation::WEST:
        {
            _initTurnRight();
            _tripState = NavigationState::TURN_RIGHT;
            _display = "TURN RIGHT";
            break;
        }
        }
    }
    // 5 3
    else if ((col == 5) && (row == 3))
    {
        switch (_nextMoveValue.orientation)
        {
        case Orientation::EAST:
        {
            _initTurnLeft();
            _tripState = NavigationState::TURN_LEFT;
            _display = "TURN LEFT";
            break;
        }
        case Orientation::WEST:
        {
            _initTurnRight();
            _tripState = NavigationState::TURN_RIGHT;
            _display = "TURN RIGHT";
            break;
        }
        }
    }
    else if (col == 2 && row == 2)
    {
        switch (_nextMoveValue.orientation)
        {
        case Orientation::EAST:
        {
            _initTurnLeft();
            _tripState = NavigationState::TURN_LEFT_180;
            _display = "TURN LEFT";
            break;
        }
        case Orientation::WEST:
        {
            _initTurnRight();
            _tripState = NavigationState::TURN_RIGHT_180;
            _display = "TURN RIGHT";
            break;
        }
        case Orientation::NORTH:
        {
            _initTurnLeft();
            _tripState = NavigationState::TURN_LEFT_180;
            _display = "TURN LEFT";
            break;
        }
        case Orientation::SOUTH:
        {
            _initTurnRight();
            _tripState = NavigationState::TURN_RIGHT_180;
            _display = "TURN RIGHT";
            break;
        }
        }
    }
    else if (col == 5 && row == 1)
    {
        switch (_nextMoveValue.orientation)
        {
        case Orientation::EAST:
        {
            _initTurnLeft();
            _tripState = NavigationState::TURN_LEFT_180;
            _display = "TURN LEFT";
            break;
        }
        case Orientation::WEST:
        {
            _initTurnRight();
            _tripState = NavigationState::TURN_RIGHT_180;
            _display = "TURN RIGHT";
            break;
        }
        case Orientation::NORTH:
        {
            _initTurnLeft();
            _tripState = NavigationState::TURN_LEFT_180;
            _display = "TURN LEFT";
            break;
        }
        case Orientation::SOUTH:
        {
            _initTurnRight();
            _tripState = NavigationState::TURN_RIGHT_180;
            _display = "TURN RIGHT";
            break;
        }
        }
    }
}

void Navigation::parking()
{

    adjustForward(NAV_ADJ_FWD_AMT);
    stop();
    _delay_ms(NAV_STOP_DELAY);

    if (_lineMakerModule.getDetectionFlag() == LineMakerFlag::NO_LINE)
    {
        while (_lineMakerModule.getDetectionFlag() != LineMakerFlag::LEFT_ADJUSTMENT || _lineMakerModule.getDetectionFlag() == LineMakerFlag::RIGHT_ADJUSTMENT || _lineMakerModule.getDetectionFlag() == LineMakerFlag::NO_ADJUSTMENT)
        {

            switch (_lastCrossroad)
            {
            case LineMakerFlag::OUTER_LEFT_DETECTION:
            {
                turnLeft();
                break;
            }
            case LineMakerFlag::OUTER_RIGHT_DETECTION:
            {
                turnRight();
                break;
            }
            case LineMakerFlag::FULL_CROSSROAD:
            {
                turnRight();
                break;
            }
            }
        }

        // update _currentOrientation

        if (*_currentOrientation == Orientation::NORTH && _lastCrossroad == LineMakerFlag::OUTER_LEFT_DETECTION)
        {
            *_currentOrientation = Orientation::WEST;
        }
        else if (*_currentOrientation == Orientation::NORTH && _lastCrossroad == LineMakerFlag::OUTER_RIGHT_DETECTION)
        {
            *_currentOrientation = Orientation::EAST;
        }
        else if (*_currentOrientation == Orientation::SOUTH && _lastCrossroad == LineMakerFlag::OUTER_LEFT_DETECTION)
        {
            *_currentOrientation = Orientation::EAST;
        }
        else if (*_currentOrientation == Orientation::SOUTH && _lastCrossroad == LineMakerFlag::OUTER_RIGHT_DETECTION)
        {
            *_currentOrientation = Orientation::WEST;
        }
        else if (*_currentOrientation == Orientation::WEST && _lastCrossroad == LineMakerFlag::OUTER_LEFT_DETECTION)
        {
            *_currentOrientation = Orientation::SOUTH;
        }
        else if (*_currentOrientation == Orientation::WEST && _lastCrossroad == LineMakerFlag::OUTER_RIGHT_DETECTION)
        {
            *_currentOrientation = Orientation::NORTH;
        }
        else if (*_currentOrientation == Orientation::EAST && _lastCrossroad == LineMakerFlag::OUTER_LEFT_DETECTION)
        {
            *_currentOrientation = Orientation::NORTH;
        }
        else if (*_currentOrientation == Orientation::EAST && _lastCrossroad == LineMakerFlag::OUTER_RIGHT_DETECTION)
        {
            *_currentOrientation = Orientation::SOUTH;
        }
        else if (*_currentOrientation == Orientation::NORTH && _lastCrossroad == LineMakerFlag::FULL_CROSSROAD)
        {
            *_currentOrientation = Orientation::EAST;
        }
        else if (*_currentOrientation == Orientation::SOUTH && _lastCrossroad == LineMakerFlag::FULL_CROSSROAD)
        {
            *_currentOrientation = Orientation::WEST;
        }
        else if (*_currentOrientation == Orientation::WEST && _lastCrossroad == LineMakerFlag::FULL_CROSSROAD)
        {
            *_currentOrientation = Orientation::NORTH;
        }
        else if (*_currentOrientation == Orientation::EAST && _lastCrossroad == LineMakerFlag::FULL_CROSSROAD)
        {
            *_currentOrientation = Orientation::SOUTH;
        }
    }

    stop();
    _delay_ms(NAV_STOP_DELAY);
}

void Navigation::_turnLeft180()
{
    LineMakerFlag lineMakerFlag = _lineMakerModule.getDetectionFlag();

    if (_irModule.isObstacleDetected() && _turn180Count == 2)
    {
        _tripState = NavigationState::MEET_POST;
    }

    else
    {

        switch (lineMakerFlag)
        {
        case LineMakerFlag::NO_LINE:
        {
            // if we don't detect the line, we need to turn right until we detect it
            turnLeft();
            if (_turn180Count == 1)
            {
                _turn180Count = 2;
            }
            break;
        }
        case LineMakerFlag::LEFT_ADJUSTMENT:
        {
            // if we detect the line on the left, it means we met the line
            // so stop moving and go to forward state
            if (_turn180Count == 0)
            {
                _turn180Count = 1;
            }
            else if (_turn180Count == 2)
            {
                stop();
                _delay_ms(NAV_STOP_DELAY);
                _preventInitForward = true;
                _firstMove = false;
                _turn180Count = 0;
                _chooseForwardMove();
            }

            break;
        }
        }
    }
}

void Navigation::_turnRight180()
{

    LineMakerFlag lineMakerFlag = _lineMakerModule.getDetectionFlag();

    if (_irModule.isObstacleDetected() && _turn180Count == 2)
    {
        _tripState = NavigationState::MEET_POST;
    }

    else

    {
        switch (lineMakerFlag)
        {
        case LineMakerFlag::NO_LINE:
        {
            // if we don't detect the line, we need to turn right until we detect it
            turnRight();
            if (_turn180Count == 1)
            {
                _turn180Count = 2;
            }
            break;
        }
        case LineMakerFlag::RIGHT_ADJUSTMENT:
        {
            // if we detect the line on the right, it means we met the line
            // so stop moving and go to forward state
            if (_turn180Count == 0)
            {
                _turn180Count = 1;
            }
            else if (_turn180Count == 2)
            {
                stop();
                _delay_ms(NAV_STOP_DELAY);
                _preventInitForward = true;
                _firstMove = false;
                _turn180Count = 0;
                _chooseForwardMove();
            }

            break;
        }
        }
    }
}