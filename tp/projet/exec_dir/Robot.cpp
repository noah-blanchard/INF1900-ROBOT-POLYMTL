#include "Robot.h"

ISR(TIMER2_COMPA_vect)
{
    customDelayElapsed = true;
}

Robot::Robot()
{
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
        _modeSelectionRoutine();
        break;
    }
    case State::IDENTIFY_CORNER:
    {
        _identifyCornerRoutine();
        break;
    }
    case State::TRAVEL_POSITION_SELECTION:
    {
        _travelPositionSelectionRoutine();
        break;
    }
    case State::CALCULATE_PATH:
    {
        _calculatePathRoutine();
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
    LineMaker::Flag flag = _lineMakerModule.getDetectionFlag();

    if(_irSensorModule.isObstacleDetected()){
        _navModule.stop();
        _currentState = State::MEET_OBSTACLE;
        return;
    } else
    {
        switch (flag)
    {
    case LineMaker::Flag::NO_ADJUSTMENT:
    {
        _navModule.go(180, false);
        break;
    }
    case LineMaker::Flag::LEFT_ADJUSTMENT:
    {
        _pause();
        _navModule.adjustLeft();
        _pause();
        break;
    }
    case LineMaker::Flag::RIGHT_ADJUSTMENT:
    {
        _pause();
        _navModule.adjustRight();
        _pause();
        break;
    }
    case LineMaker::Flag::NO_LINE:
    {
        _navModule.stop();
        break;
    }
    case LineMaker::Flag::FULL_CROSSROAD:
    {
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
    }
    
    _navModule.stop();
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

        while (!timerHasElapsed)
            ;
        timerHasElapsed = false;

        _delayTimerModule.reset();
    }
}