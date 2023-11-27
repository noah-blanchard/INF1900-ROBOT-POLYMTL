#include "Robot.h"

// custom timer volatile variable
// volatile bool customDelayElapsed = false;

// ISR(TIMER2_COMPA_vect)
// {
//     customDelayElapsed = true;
// }

Robot::Robot() : _display(&DDRC, &PORTC), _navModule(_currentPosition, _currentOrientation)
{
    _validateButton = Bouton(INT1);
    _selectButton = Bouton(INT0);
    cli();
    _validateButton.enableInterrupt();
    _validateButton.setRisingEdge();

    _selectButton.enableInterrupt();
    _selectButton.setRisingEdge();
    sei();

    _beginning[0] = 0;
    _beginning[1] = 0;

    _currentOrientation = Orientation::SOUTH;
    _currentPosition[0] = 0;
    _currentPosition[1] = 0;

    // _beginning[0] = 0;
    // _beginning[1] = 0;
    //_currentState = State::MODE_SELECTION;
    //_currentState = State::NAVIGATE_TRIP; // pour l'instant on le met en followline, mais evidemment le initState sera le MODE_SELECTION
    //_currentState = State::NAVIGATE_TRIP;
    _currentState = State::CALCULATE_PATH;

    //_currentState = State::MAKE_TRIP;
    // _moveArray init for test

    // _moveArray[0].orientation = Orientation::EAST;
    // _moveArray[0].x = 1;
    // _moveArray[0].y = 0;

    // _moveArray[1].orientation = Orientation::EAST;
    // _moveArray[1].x = 2;
    // _moveArray[1].y = 0;

    // _moveArray[2].orientation = Orientation::SOUTH;
    // _moveArray[2].x = 2;
    // _moveArray[2].y = 1;

    // _moveArray[3].orientation = Orientation::WEST;
    // _moveArray[3].x = 1;
    // _moveArray[3].y = 1;
    // _moveArray[4].orientation = Orientation::SOUTH;
    // _moveArray[4].x = 1;
    // _moveArray[4].y = 2;
    // _moveArray[5].orientation = Orientation::FINISHED;
    // _moveArray[5].x = 1;
    // _moveArray[5].y = 2;
}

Robot::~Robot()
{
}

void Robot::runRoutine()
{
    switch (_currentState)
    {
    case State::MODE_SELECTION:
    {
        //_modeSelectionRoutine();
        break;
    }
    case State::IDENTIFY_CORNER:
    {
        //_identifyCornerRoutine();
        _identifyCornerModule.identificationProcess(_beginning);
        break;
    }
    case State::MAKE_TRIP:
    {
        _maketripModule.selectDestination(_destination);
        break;
    }
    case State::TRAVEL_POSITION_SELECTION:
    {
        //_travelPositionSelectionRoutine();
        break;
    }
    case State::CALCULATE_PATH:
    {
        _calculatePathRoutine();
        break;
    }
    case State::NAVIGATE_TRIP:
    {
        _navigateTripRoutine();
        break;
    }
    case State::FOLLOW_LINE:
    {
        _followLineRoutine();
        break;
    }
    case State::MEET_CROSSROAD:
    {
        _meetCrossroadRoutine();
        break;
    }
    case State::TURN_AT_CROSSROAD:
    {
        _turnAtCrossroadRoutine();
        break;
    }
    default:
    {
        break;
    }
    }
}

void Robot::_followLineRoutine()
{
    LineMakerFlag flag = _lineMakerModule.getDetectionFlag();
    switch (flag)
    {
    case LineMakerFlag::LEFT_ADJUSTMENT:
    {
        _navModule.adjustRight();
        break;
    }
    case LineMakerFlag::NO_ADJUSTMENT:
    {
        _navModule.go(_BASE_SPEED, false);
        break;
    }
    case LineMakerFlag::RIGHT_ADJUSTMENT:
    {
        _navModule.adjustLeft();
        break;
    }
    }
}

void Robot::_meetCrossroadRoutine()
{
    // // Go a bit forward during 100 ms
    // _navModule.go(_BASE_SPEED, false);
    // _customDelay(1000);
    // _navModule.stop();
    // _customDelay(300);
    // _navModule.goRightWheel(_TURN_SPEED, true);
    // _navModule.goLeftWheel(_TURN_SPEED, false);
    // _customDelay(400);
    // _navModule.stop();
    // _customDelay(300);
    // // change state to turn at crossroad
    // _currentState = State::TURN_AT_CROSSROAD;
}

void Robot::_turnAtCrossroadRoutine()
{
    // for the moment let's only turn to the right until the linemaker sens the "leftAdjustment" flag
    LineMakerFlag flag = _lineMakerModule.getDetectionFlag();
    switch (flag)
    {
    case LineMakerFlag::LEFT_ADJUSTMENT:
    {
        _navModule.stop();
        _currentState = State::FOLLOW_LINE;
        break;
    }
    default:
    {
        _navModule.goRightWheel(_TURN_SPEED, true);
        _navModule.goLeftWheel(_TURN_SPEED, false);
        break;
    }
    }
}

void Robot::_calculatePathRoutine()
{
    _display = "I WILL CALCULATE";
    _delay_ms(1500);

    /// chaneg this with the result of Make Trip selection
    _destination[0] = 2;
    _destination[1] = 2;

    _dijkstraModule.run(_beginning, _destination, _moveArray);
    _display = "FINISHED";
    _delay_ms(2000);
    _currentState = State::NAVIGATE_TRIP;
}

void Robot::_navigateTripRoutine()
{
    Move tripResult = _navModule.followTrip(_moveArray); // returns the last move of the trip ! will return the one with a post if invalid
    if (_currentOrientation != Orientation::FINISHED)
    {
        // means we havent reached thend end => recalculate the path !
        _beginning[0] = _currentPosition[0];
        _beginning[1] = _currentPosition[1];
        _dijkstraModule.removeNode(_currentPosition[0], _currentPosition[1]);
        _currentState = State::CALCULATE_PATH;
    }
}

// void Robot::_pause()
// {
//     _customDelay(_BASE_PAUSE_DELAY);
// }

// void Robot::_customDelay(uint16_t delay)
// {
//     _delayTimerModule.reset();
//     cli();
//     _delayTimerModule.enable();
//     sei();
//     // use timer0 to wait for the delay
//     for (int i = 0; i < delay; ++i)
//     {

//         while (!customDelayElapsed)
//             ;
//         customDelayElapsed = false;

//         _delayTimerModule.reset();
//     }
// }