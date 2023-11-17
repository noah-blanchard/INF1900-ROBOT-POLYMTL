#include "Robot.h"

// custom timer volatile variable
volatile bool customDelayElapsed = false;

ISR(TIMER2_COMPA_vect)
{
    customDelayElapsed = true;
}

Robot::Robot()
{
    _validateButton = Bouton(INT1);
    _selectButton = Bouton(INT0);
    cli();
    _validateButton.enableInterrupt();
    _validateButton.setRisingEdge();

    _selectButton.enableInterrupt();
    _selectButton.setRisingEdge();
    sei();

    TimerConfig timerConfig;
    timerConfig.timer = 2;
    timerConfig.prescaler = 256;
    timerConfig.delay_ms = 1;

    _delayTimerModule = Timer(timerConfig);
    //_currentState = State::MODE_SELECTION;
    _currentState = State::FOLLOW_LINE; // pour l'instant on le met en followline, mais evidemment le initState sera le MODE_SELECTION
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
        _identifyCorner.identificationProcess(_beginning);
        break;
    }
    case State::MAKE_TRIP:
    {
        _maketrip.selectDestination(_destination);
        break;
    }
    case State::TRAVEL_POSITION_SELECTION:
    {
        //_travelPositionSelectionRoutine();
        break;
    }
    case State::CALCULATE_PATH:
    {
        //_calculatePathRoutine();
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
    Flag flag = _lineMakerModule.getDetectionFlag();
    LCM disp(&DDRC, &PORTC);
    disp.clear();

    if (_irSensorModule.isObstacleDetected())
    {
        _navModule.stop();
    }
    else
    {
        switch (flag)
        {
        case Flag::NO_ADJUSTMENT:
        {
            _navModule.go(180, false);
            disp << "hqhqhq";
            break;
        }
        case Flag::LEFT_ADJUSTMENT:
        {
            _navModule.adjustLeft();
            _pause();
            _navModule.stop();
            _pause();
            break;
        }
        case Flag::RIGHT_ADJUSTMENT:
        {
            _navModule.adjustRight();
            _pause();
            _navModule.stop();
            _pause();
            break;
        }
        case Flag::NO_LINE:
        {
            _navModule.stop();
            break;
        }
        case Flag::FULL_CROSSROAD:
        {
            disp << "(4,1) N";
            _navModule.stop();

            _currentState = State::MEET_CROSSROAD;
            break;
        }

            // Je rajoute des cases pour LEFT_CROSSROAD ET RIGHT_CROSSROAD

        case Flag::LEFT_CROSSROAD:
        {
            disp << "left";

            _navModule.stop();
            _currentState = State::MEET_CROSSROAD;
            break;
        }

        case Flag::RIGHT_CROSSROAD:
        {
            disp << "right";

            _navModule.stop();
            _currentState = State::MEET_CROSSROAD;
            break;
        }

        default:
        {
            _navModule.stop();
            break;
        }
        }
        //}
    }
}

void Robot::_meetCrossroadRoutine()
{
    // Go a bit forward during 100 ms
    _navModule.go(_BASE_SPEED, false);
    _customDelay(1000);
    _navModule.stop();
    _customDelay(300);
    // change state to turn at crossroad
    _currentState = State::TURN_AT_CROSSROAD;
}

void Robot::_turnAtCrossroadRoutine()
{
    // for the moment let's only turn to the right until the linemaker sens the "leftAdjustment" flag
    Flag flag = _lineMakerModule.getDetectionFlag();
    switch (flag)
    {
    case Flag::LEFT_ADJUSTMENT:
    {
        _navModule.stop();
        _currentState = State::FOLLOW_LINE;
        break;
    }
    default:
    {
        _navModule.goRightWheel(_TURN_SPEED, false);
        _navModule.goLeftWheel(_TURN_SPEED, false);
        break;
    }
    }
}

void Robot::_pause()
{
    _customDelay(_BASE_PAUSE_DELAY);
}

void Robot::_customDelay(uint16_t delay)
{
    _delayTimerModule.reset();
    cli();
    _delayTimerModule.enable();
    sei();
    // use timer0 to wait for the delay
    for (int i = 0; i < delay; ++i)
    {

        while (!customDelayElapsed)
            ;
        customDelayElapsed = false;

        _delayTimerModule.reset();
    }
}