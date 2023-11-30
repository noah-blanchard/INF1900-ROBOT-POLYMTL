#include "IdentifyCorner.h"

IdentifyCorner::IdentifyCorner() : _display(&DDRC, &PORTC), _led(&PORTB, &DDRB, PB0, PB1)
{
}

IdentifyCorner::~IdentifyCorner()
{
}

void IdentifyCorner::identificationProcess(uint8_t *_beginning)
{
    _display.clear();
    while (!_found)
    {
        //_led.flashGreen();
        switch (_state)
        {
        case IdentifyCornerState::GO_FORWARD_FIRST_LINE:
            _goForwardFirstLine();
            break;

        case IdentifyCornerState::TURN_AROUND:
            _turnAround();
            break;

        case IdentifyCornerState::TURN_AROUND_SECOND_LINE:
            _turnAroundSecondLine();
            break;

        case IdentifyCornerState::GO_BACK_FIRST_LINE:
            _goBackFirstLine();
            _navModule.stop();
            break;

        case IdentifyCornerState::GO_BACK_SECOND_LINE:
            _goBackSecondLine();
            _navModule.stop();
            break;

        case IdentifyCornerState::TURN_SECOND_LINE:
            _turnSecondLine();
            break;

        case IdentifyCornerState::TURN_BACK_FIRST_LINE:
            _turnBackFirstLine();
            break;

        case IdentifyCornerState::TURN_BACK_SECOND_LINE:
            _turnBackSecondLine();
            break;

        case IdentifyCornerState::GO_FORWARD_SECOND_LINE:
            _goForwardSecondLine();
            break;

        case IdentifyCornerState::GO_INIT_POS:
            _goInitPos();
            break;

        case IdentifyCornerState::TURN_INIT_POS:
            _turnInitPos();
            break;
        }
    }

    _display.clear();
    _displayInitPos();
    //_led.turnLedGreen();

    return;
}

// go forward on the first line met by the robot
void IdentifyCorner::_goForwardFirstLine()
{
    _display.clear();
    _display << "GO FORWARD";
    LineMakerFlag flag = _lineMakerModule.getDetectionFlag();
    _navModule.go(Navigation::_BASE_SPEED, false);
    switch (flag)
    {

    case LineMakerFlag::RIGHT_ADJUSTMENT:
        _blockIncrementation = false;
        _navModule.adjustLeft();
        break;

    case LineMakerFlag::LEFT_ADJUSTMENT:
        _blockIncrementation = false;
        _navModule.adjustRight();
        break;

    case LineMakerFlag::OUTER_RIGHT_DETECTION:
    {
        if (!_blockIncrementation)
        {
            _firstLineCount++;
            _sidefirst = true;
            _displayCurrentIntersectionCount();
            _blockIncrementation = true;
        }
        // _navModule.adjustRight();
        isRight = true;
        break;
    }

    case LineMakerFlag::OUTER_LEFT_DETECTION:
        if (!_blockIncrementation)
        {
            _firstLineCount++;
            _displayCurrentIntersectionCount();
            _blockIncrementation = true;
        }
        isRight = false;
        break;

    case LineMakerFlag::FULL_CROSSROAD:
        if (!_blockIncrementation)
        {
            _firstLineCount++;
            _displayCurrentIntersectionCount();
            _blockIncrementation = true;
        }
        break;
    case LineMakerFlag::NO_LINE:
    {
        _navModule.adjustForward();
        if (_simpleCompareMAtch())
        {
            makeSound();
            _displayCurrentIntersectionCount();
            _state = IdentifyCornerState::TURN_BACK_FIRST_LINE;
        }
        else
        {
            _state = IdentifyCornerState::TURN_AROUND;
        }

        break;
    }
    }
}

void IdentifyCorner::makeSound()
{
    _sound.chooseFrequency(81);
    _delay_ms(1000);
    _sound.stopSound();
}
void IdentifyCorner::_displayCurrentIntersectionCount()
{
    char displayString[16];
    sprintf(displayString, "%d %d", _firstLineCount, _secondLineCount);
    _display.clear();
    _display << displayString;
}

bool IdentifyCorner::_simpleCompareMAtch()
{
    _displayCurrentIntersectionCount();
    if ((_firstLineCount == 1) && (_secondLineCount == 0) && (_blockIncrementation == false))
    {
        _initCorner = Corner::RCTH;
        return true;
    }
    else if ((_firstLineCount == 2) && (_secondLineCount == 1) && (_blockIncrementation == false))
    {
        _initCorner = Corner::RCTV;
        return true;
    }
    else if ((_firstLineCount == 3) && (_secondLineCount == 0))
    {
        _initCorner = Corner::RCBH;
        return true;
    }
    else if ((_firstLineCount == 2) && (_secondLineCount == 3))
    {

        _initCorner = Corner::RCBV;
        return true;
    }
    else if ((_firstLineCount == 1) && (_secondLineCount == 1) && _sidefirst)
    {
        // LCBV
        _initCorner = Corner::LCBV;
        return true;
    }
    else if ((_firstLineCount == 1) && (_secondLineCount == 1 && !_sidefirst))
    {
        // LCBH
        _initCorner = Corner::LCBH;
        return true;
    }
    else if ((_firstLineCount == 2) && (_secondLineCount == 1) && _blockIncrementation)
    {
        // LCTH
        _initCorner = Corner::LCTH;
        return true;
    }
    else if ((_firstLineCount == 1) && (_secondLineCount == 2) && _blockIncrementation)
    {
        // LCTV
        _initCorner = Corner::LCTV;
        return true;
    }
    return false;
}

// turn around should turn around
void IdentifyCorner::_turnAround()
{
    _display.clear();
    _display << "turn around";
    if (isRight)
    {
        _navModule.turnLeft();
    }
    else
    {
        _navModule.turnRight();
    }

    LineMakerFlag sensor = _lineMakerModule.getDetectionFlag();

    if (sensor == LineMakerFlag::LEFT_ADJUSTMENT || sensor == LineMakerFlag::RIGHT_ADJUSTMENT || sensor == LineMakerFlag::NO_ADJUSTMENT)
    {
        _navModule.stop();
        _delay_ms(1000);
        _displayCurrentIntersectionCount();
        _delay_ms(2000);
        _state = IdentifyCornerState::GO_BACK_FIRST_LINE;
    }
}

void IdentifyCorner::_turnAroundSecondLine()
{
    //_displayCurrentIntersectionCount();
    _display.clear();
    _display << "SEC turn around";
    if (isRight)
    {
        _navModule.turnLeft();
    }
    else
    {
        _navModule.turnRight();
    }

    LineMakerFlag sensor = _lineMakerModule.getDetectionFlag();

    if (sensor == LineMakerFlag::LEFT_ADJUSTMENT || sensor == LineMakerFlag::RIGHT_ADJUSTMENT || sensor == LineMakerFlag::NO_ADJUSTMENT)
    {
        _navModule.stop();
        _delay_ms(1000);
        _displayCurrentIntersectionCount();
        _delay_ms(2000);
        _state = IdentifyCornerState::GO_INIT_POS;
    }
}

void IdentifyCorner::_goBackFirstLine()
{
    // do the same but don't increment, just go back until no line is detected (start position)
    LineMakerFlag flag = _lineMakerModule.getDetectionFlag();

    _navModule.go(Navigation::_BASE_SPEED, false);
    switch (flag)
    {
    case LineMakerFlag::NO_LINE:
        _navModule.adjustForward();
        _state = IdentifyCornerState::TURN_SECOND_LINE;
        break;
    case LineMakerFlag::LEFT_ADJUSTMENT:
        _navModule.adjustRight();
        break;
    case LineMakerFlag::RIGHT_ADJUSTMENT:
        _navModule.adjustLeft();
        break;
    }
}

void IdentifyCorner::_goInitPos()
{
    // do the same but don't increment, just go back until no line is detected (start position)
    LineMakerFlag flag = _lineMakerModule.getDetectionFlag();
    // switch (flag)
    // {
    // case LineMakerFlag::NO_LINE:
    //     _navModule.adjustForward();
    //     _state = IdentifyCornerState::TURN_INIT_POS;
    //     break;
    // case LineMakerFlag::NO_ADJUSTMENT:
    //     _navModule.go(Navigation::_BASE_SPEED, false);
    //     break;
    // case LineMakerFlag::LEFT_ADJUSTMENT:
    //     _navModule.adjustRight();
    //     break;
    // case LineMakerFlag::RIGHT_ADJUSTMENT:
    //     _navModule.adjustLeft();
    //     break;
    // }
    _navModule.go(Navigation::_BASE_SPEED, false);
    switch (flag)
    {

    case LineMakerFlag::RIGHT_ADJUSTMENT:
        _blockIncrementation = false;
        _navModule.adjustLeft();
        break;

    case LineMakerFlag::LEFT_ADJUSTMENT:
        _blockIncrementation = false;
        _navModule.adjustRight();
        break;

    // case LineMakerFlag::OUTER_RIGHT_DETECTION:
    // {
    //     if (!_blockIncrementation)
    //     {
    //         _firstLineCount++;
    //         _sidefirst = true;
    //         _displayCurrentIntersectionCount();
    //         _blockIncrementation = true;
    //     }
    //     // _navModule.adjustRight();
    //     isRight = true;
    //     break;
    // }

    // case LineMakerFlag::OUTER_LEFT_DETECTION:
    //     if (!_blockIncrementation)
    //     {
    //         _firstLineCount++;
    //         _displayCurrentIntersectionCount();
    //         _blockIncrementation = true;
    //     }
    //     isRight = false;
    //     break;

    // case LineMakerFlag::FULL_CROSSROAD:
    //     if (!_blockIncrementation)
    //     {
    //         _firstLineCount++;
    //         _displayCurrentIntersectionCount();
    //         _blockIncrementation = true;
    //     }
    //     break;
    case LineMakerFlag::NO_LINE:
    {
        _navModule.adjustForward();
        _state = IdentifyCornerState::TURN_INIT_POS;
        break;
    }
    }
}

void IdentifyCorner::_turnInitPos()
{
    bool turnRight = (_initCorner == Corner::RCBH) ||
                     (_initCorner == Corner::LCTH) ||
                     (_initCorner == Corner::LCBV) ||
                     (_initCorner == Corner::RCTV);

    if (turnRight)
    {
        _navModule.turnRight();
    }
    else
    {
        _navModule.turnLeft();
    }

    LineMakerFlag sensor = _lineMakerModule.getDetectionFlag();
    if (sensor == LineMakerFlag::LEFT_ADJUSTMENT || sensor == LineMakerFlag::RIGHT_ADJUSTMENT || sensor == LineMakerFlag::NO_ADJUSTMENT)
    {
        _navModule.stop();
        _delay_ms(2000);
        _found = true;
        _state = IdentifyCornerState::FINISH;
    }
}

void IdentifyCorner::_goBackSecondLine()
{
    // do the same but don't increment, just go back until no line is detected (start position)
    LineMakerFlag flag = _lineMakerModule.getDetectionFlag();

    _navModule.go(Navigation::_BASE_SPEED, false);
    switch (flag)
    {
    case LineMakerFlag::NO_LINE:
        _navModule.adjustForward();
        _state = IdentifyCornerState::TURN_BACK_FIRST_LINE;
        break;
    case LineMakerFlag::LEFT_ADJUSTMENT:
        break;
    case LineMakerFlag::RIGHT_ADJUSTMENT:
        _navModule.adjustLeft();
        break;
    }
}

void IdentifyCorner::_turnSecondLine()
{

    if (isRight)
    {
        _navModule.turnLeft();
    }
    else
    {
        _navModule.turnRight();
    }

    LineMakerFlag sensor = _lineMakerModule.getDetectionFlag();

    if (sensor == LineMakerFlag::LEFT_ADJUSTMENT || sensor == LineMakerFlag::RIGHT_ADJUSTMENT || sensor == LineMakerFlag::NO_ADJUSTMENT)
    {
        _navModule.stop();
        _delay_ms(1000);
        _state = IdentifyCornerState::GO_FORWARD_SECOND_LINE;
    }
}

void IdentifyCorner::_turnBackFirstLine()
{
    _display.clear();
    _display << "turn around FIRST";
    if (isRight)
    {
        _navModule.turnLeft();
    }
    else
    {
        _navModule.turnRight();
    }

    LineMakerFlag sensor = _lineMakerModule.getDetectionFlag();

    if (sensor == LineMakerFlag::LEFT_ADJUSTMENT || sensor == LineMakerFlag::RIGHT_ADJUSTMENT || sensor == LineMakerFlag::NO_ADJUSTMENT)
    {
        _navModule.stop();
        _delay_ms(1000);
        _state = IdentifyCornerState::GO_INIT_POS;
    }
}

void IdentifyCorner::_turnBackSecondLine()
{

    // turn around from left
    if (isRight)
    {
        _navModule.turnLeft();
    }
    else
    {
        _navModule.turnRight();
    }

    // uint8_t sensor = _lineMakerModule._retrieveSensorData();
    LineMakerFlag sensor = _lineMakerModule.getDetectionFlag();

    if (sensor == LineMakerFlag::LEFT_ADJUSTMENT || sensor == LineMakerFlag::RIGHT_ADJUSTMENT || sensor == LineMakerFlag::NO_ADJUSTMENT)
    {
        _delay_ms(1000);
        _navModule.stop();
        _display.clear();
        _display << "back on sec line";
        _state = IdentifyCornerState::TURN_BACK_SECOND_LINE;
    }
}

void IdentifyCorner::_goForwardSecondLine()
{
    // go forward until no line is detected
    LineMakerFlag flag = _lineMakerModule.getDetectionFlag();
    _navModule.go(Navigation::_BASE_SPEED, false);
    _display = "sec line";
    switch (flag)
    {
    case LineMakerFlag::NO_LINE:
        _navModule.adjustForward();
        _display << "simple";
        if (_simpleCompareMAtch())
        {
            _display.clear();
            _display << "simple";
            makeSound();
            _delay_ms(2000);
            _state = IdentifyCornerState::TURN_AROUND_SECOND_LINE;
        }
        else
        {
            _display.clear();
            _display << "no simple";
            // _state = IdentifyCornerState::TURN_THIRD_LINE;
        }
        break;
    case LineMakerFlag::RIGHT_ADJUSTMENT:
        _blockIncrementation = false;
        _navModule.adjustLeft();

        break;

    case LineMakerFlag::LEFT_ADJUSTMENT:
        _blockIncrementation = false;
        _navModule.adjustRight();

        break;

    case LineMakerFlag::OUTER_RIGHT_DETECTION:
    {
        if (!_blockIncrementation)
        {
            _secondLineCount++;
            _displayCurrentIntersectionCount();
            _blockIncrementation = true;
        }
        isRight = true;
        break;
    }

    case LineMakerFlag::OUTER_LEFT_DETECTION:
        if (!_blockIncrementation)
        {
            _secondLineCount++;
            _displayCurrentIntersectionCount();
            _blockIncrementation = true;
        }
        isRight = false;
        break;
    }
}

void IdentifyCorner::_displayInitPos()
{
    _display.clear();

    switch (_initCorner)
    {
    case Corner::LCTH:
    {
        _display.write("(1, 1)");
        _display.write("EST", LCM_FW_HALF_CH);
        break;
    }
    case Corner::LCTV:
    {
        _display.write("(1, 1)");
        _display.write("SUD", LCM_FW_HALF_CH);
        break;
    }
    case Corner::RCTH:
    {
        _display.write("(1, 7)");
        _display.write("OUEST", LCM_FW_HALF_CH);
        break;
    }
    case Corner::RCTV:
    {
        _display.write("(1, 7)");
        _display.write("SUD", LCM_FW_HALF_CH);
        break;
    }
    case Corner::LCBH:
    {
        _display.write("(4, 1)");
        _display.write("EST", LCM_FW_HALF_CH);
        break;
    }
    case Corner::LCBV:
    {
        _display.write("(4, 1)");
        _display.write("NORD", LCM_FW_HALF_CH);
        break;
    }
    case Corner::RCBH:
    {
        _display.write("(4, 7)");
        _display.write("OUEST", LCM_FW_HALF_CH);
        break;
    }
    case Corner::RCBV:
    {
        _display.write("(4, 7)");
        _display.write("NORD", LCM_FW_HALF_CH);
        break;
    }
    }
}