#include "IdentifyCorner.h"

IdentifyCorner::IdentifyCorner() : _display(&DDRC, &PORTC)
{
}

IdentifyCorner::~IdentifyCorner()
{
}

void IdentifyCorner::identificationProcess(uint8_t *_beginning)
{
            _display = "Searching for corner...";
    while (!_found)
    {
        _currentSequence = 0;
        switch (_state)
        {
        case IdentifyCornerState::GO_FORWARD:
            _goForward();
            break;

        case IdentifyCornerState::DETECT_INTERSECTION:
            _detectIntersection();
            break;

        case IdentifyCornerState::DETECT_FORWARD:
            _detectForward();
            break;

        case IdentifyCornerState::TURN:
            _turn();
            break;

        case IdentifyCornerState::STOP:
            //_stop();
            break;
        }
    }
    return;
}

// go forward should follow the line using the line maker module
void IdentifyCorner::_goForward()
{
    _display = "GO FORWARD";
    LineMakerFlag flag = _lineMakerModule.getDetectionFlag();
    switch (flag)
    {
    case LineMakerFlag::NO_ADJUSTMENT:
        _navModule.go(120, false);
        break;

    case LineMakerFlag::RIGHT_ADJUSTMENT:
        _navModule.adjustRight();
        break;

    case LineMakerFlag::LEFT_ADJUSTMENT:
        _navModule.adjustLeft();
        break;

    case LineMakerFlag::RIGHT_CROSSROAD:
        _navModule.stop();
        _state = IdentifyCornerState::DETECT_INTERSECTION;
        break;

    case LineMakerFlag::LEFT_CROSSROAD:
        _navModule.stop();
        _state = IdentifyCornerState::DETECT_INTERSECTION;
        break;

    case LineMakerFlag::FULL_CROSSROAD:
        _navModule.stop();
        _state = IdentifyCornerState::DETECT_INTERSECTION;
        break;
    }
}

// detect intersection should detect the intersection
void IdentifyCorner::_detectIntersection()
{
    _display = "DETECT INTERSECT";
    _delay_ms(2500);
    LineMakerFlag flag = _lineMakerModule.getDetectionFlag();
    uint8_t sequenceToAdd = 0;
    // use bitshift to add at the right place
    switch (flag)
    {
    case LineMakerFlag::RIGHT_CROSSROAD:
        sequenceToAdd |= (RIGHT);
        _display = "RIGHT DETECT";
        _delay_ms(2500);
        break;

    case LineMakerFlag::LEFT_CROSSROAD:
        sequenceToAdd |= (LEFT);
         _display = "LEFT DETECT";
        _delay_ms(2500);
        break;

    case LineMakerFlag::FULL_CROSSROAD:
        sequenceToAdd |= (LEFT | RIGHT);
         _display = "FULL DETECT";
        _delay_ms(2500);
        break;
    }
    _displayCurrentSequence();
    _delay_ms(2500);
    _currentSequence |= (sequenceToAdd);
    _state = IdentifyCornerState::DETECT_FORWARD;
}

// detect forward should detect the forward
void IdentifyCorner::_detectForward()
{
    // go a bit forward, then wait
    _navModule.go(120, false);
    _delay_ms(1000);
    _navModule.stop();
    _display = "LOOKING FORWARD";
    _delay_ms(2500);
    LineMakerFlag flag = _lineMakerModule.getDetectionFlag();
    uint8_t sequenceToAdd = 0;
    // if flag == no line then there's no forward
    if (!(flag == LineMakerFlag::NO_LINE))
    {
        sequenceToAdd |= (FORWARD);
        _display = "FORWARD DETECTED";
        _delay_ms(2500);
    }
    _currentSequence |= (sequenceToAdd);
    // add the current sequence into th _sequence with tteh bitshift than increment
    _sequence |= (_currentSequence << _bitshift);
    _bitshift += 3;

    _display = "SEQUENCE:";
    _delay_ms(2500);
    _displayCurrentSequence();
    _delay_ms(4000);

    // if there's either left or right in the current sequence, then we turn
    if ((_currentSequence & (LEFT | RIGHT)) != 0)
    {
        _state = IdentifyCornerState::TURN;
    }
    else
    {
        _state = IdentifyCornerState::GO_FORWARD;
    }
}

// turn should turn
void IdentifyCorner::_turn()
{

    // determine if left or right using the current sequence
    bool isLeft = false;
    if ((_currentSequence & LEFT) != 0)
    {
        isLeft = true;
    }
    else if ((_currentSequence & RIGHT) != 0)
    {
        isLeft = false;
    }

    // if (isLeft)
    // {
    //     _navModule.goRightWheel(120, false);
    //     _navModule.goLeftWheel(120, true);
    // }
    // else
    // {
    //     _navModule.goRightWheel(120, true);
    //     _navModule.goLeftWheel(120, false);
    // }

    // turn for 250 ms in the right direction
    if (isLeft)
    {
        _navModule.goRightWheel(120, false);
        _navModule.goLeftWheel(120, true);
        _delay_ms(250);
    }
    else
    {
        _navModule.goRightWheel(120, true);
        _navModule.goLeftWheel(120, false);
        _delay_ms(250);
    }

    // stop
    _navModule.stop();

    _display = "TURNING";

    // now turn until the line is found (left adjustment for right turn, right adjustment for left turn)
    if (isLeft)
    {
        _navModule.goRightWheel(120, false);
        _navModule.goLeftWheel(120, true);
        while (_lineMakerModule.getDetectionFlag() != LineMakerFlag::RIGHT_ADJUSTMENT)
        {
        }
    }
    else
    {
        _navModule.goRightWheel(120, true);
        _navModule.goLeftWheel(120, false);
        while (_lineMakerModule.getDetectionFlag() != LineMakerFlag::LEFT_ADJUSTMENT)
        {
        }
    }

    // stop
    _navModule.stop();

    _state = IdentifyCornerState::GO_FORWARD;
}

void IdentifyCorner::_displayCurrentSequence()
{
    // display the current sequence
    char displayString[16];
    //sprintf(displayString, "%d", _currentSequence);
    // use sprint f to format as bits display not decim

    // display in base 2 (binary) not decimal
    sprintf(displayString, "%d", _currentSequence);

    _display = displayString;
}