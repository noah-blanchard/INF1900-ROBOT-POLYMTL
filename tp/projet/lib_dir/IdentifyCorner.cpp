#include "IdentifyCorner.h"

IdentifyCorner::IdentifyCorner() : _display(&DDRC, &PORTC)
{
}

IdentifyCorner::~IdentifyCorner()
{
}

void IdentifyCorner::identificationProcess(uint8_t *_beginning)
{
    _display.clear();
    _display << "Searching for corner...";
    while (!_found)
    {
        switch (_state)
        {
        case IdentifyCornerState::GO_FORWARD_FIRST_LINE:
            _goForward();
            break;

        case IdentifyCornerState::TURN_AROUND:
            _turnAround();
            break;

        case IdentifyCornerState::TURN_AROUND_SECOND_LINE:
            _turnAroundSecondLine();
            break;

        case IdentifyCornerState::TURN_AROUND_THIRD_LINE:
            _turnAroundThirdLine();
            break;

        case IdentifyCornerState::GO_BACK:
            _goBack();
            _display = "GO_BACK";
            _navModule.stop();
            break;

        case IdentifyCornerState::GO_BACK_SECOND_LINE:
            _goBackSecondLine();
            _display = "GO_BACK sec line";
            _navModule.stop();
            break;

        case IdentifyCornerState::GO_BACK_THIRD_LINE:
            _goBackThirdLine();
            _display = "GO_BACK third line";
            _navModule.stop();
            break;

        case IdentifyCornerState::TURN_SECOND_LINE:
            _turnSecondLine();
            _display = "TURN_SECOND_LINE";
            //_navModule.stop();
            break;

        case IdentifyCornerState::TURN_BACK_FIRST_LINE:
            _turnBackFirstLine();
            _display = "TURN BACK 1";
            //_navModule.stop();
            break;

        case IdentifyCornerState::TURN_BACK_SECOND_LINE:
            _turnBackSecondLine();
            _display = "TURN BACK 2";
            //_navModule.stop();
            break;

        case IdentifyCornerState::GO_FORWARD_SECOND_LINE:
            _goForwardSecondLine();
            // _display = "sec line";
            break;

        case IdentifyCornerState::TURN_THIRD_LINE:
            _turnThirdLine();
            _display = "TURN_THIRD_LINE";
            break;

        case IdentifyCornerState::GO_FORWARD_THIRD_LINE:
            _goForwardThirdLine();
            _display = "third line";
            break;
        }
    }
    // It found the right intersection so call function compare match

    // complete here what's supposed to do when it's found

    // write function switch case recognization corner

    //            FirstLine       SecondLine
    // RCBH ==>   32
    // RCBV ==>  23

    // RCTV ===> 2 intersections right -------  1 left
    // LCTH ====> 2 intersections right  ------ 1 left *

    // LCBV ==> 1 First intersection is left --- 1*
    // LCBH ===> 1 first intersection  is right --- 1* and

    // LCTV ===> 1 first intersection is left    --- 2*
    // RCTH ===> 1 first intersection is left ------ 2*

    return;
}

// go forward should follow the line using the line maker module until no line is detected
void IdentifyCorner::_goForward()
{
    _display.clear();
    _display << "GO FORWARD";
    LineMakerFlag flag = _lineMakerModule.getDetectionFlag();
    _navModule.go(140, false);
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
        _navModule.adjustLeft();
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
        _navModule.adjustRight();
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
        _navModule.stop();
        _state = IdentifyCornerState::TURN_AROUND;
        break;
    }
    //_displayCurrentIntersectionCount();
}

void IdentifyCorner::makeSound()
{
    _sound.chooseFrequency(81);
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
    // add case 1-1 (right left and left right)
    _displayCurrentIntersectionCount();
    _delay_ms(5000);
    if ((_firstLineCount == 3) and (_secondLineCount == 2))
    {
        makeSound();
        _delay_ms(2000);
        _displayCurrentIntersectionCount();
        return true;
    }
    else if ((_firstLineCount == 2) and (_secondLineCount == 3))
    {

        _displayCurrentIntersectionCount();
        makeSound();
        return true;
    }
    else if ((_firstLineCount == 1) and (_secondLineCount == 1) and _sidefirst)
    {
        // LCBV
        _displayCurrentIntersectionCount();
        makeSound();
        return true;
    }
    else if ((_firstLineCount == 1) and (_secondLineCount == 1 and !_sidefirst))
    {
        // LCBH
        _displayCurrentIntersectionCount();
        makeSound();
        return true;
    }
    return false;
}

bool IdentifyCorner::_furtherCompareMatch()
{
    if (_firstLineCount == 3 and _secondLineCount == 1)
    {
        // LCTH -- good
        _displayCurrentIntersectionCount();
        makeSound();
        return true;
    }
    else if (_firstLineCount == 1 and _secondLineCount == 3)
    {
        // LCTV
        _displayCurrentIntersectionCount();
        makeSound();
        return true;
    }
    else if (_firstLineCount == 4 and _secondLineCount == 2)
    {
        // RCTH
        _displayCurrentIntersectionCount();
        makeSound();
        return true;
    }
    else if (_firstLineCount == 4 and _secondLineCount == 1)
    {
        // RCTV this case has to be covered;

        //_displayCurrentIntersectionCount();
        // makeSound();
        _display.clear();
        _display << "not found";
        return false;
    }
}

// turn around should turn around
void IdentifyCorner::_turnAround()
{
    //_displayCurrentIntersectionCount();
    _display.clear();
    _display << "turn around";
    // if isRight, turn around from left wait for NO_ADJUSTMENT or LEFT_ADJUSTMENT or RIGHT_ADJUSTMENT
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
        _state = IdentifyCornerState::GO_BACK;
    }
}

// turn around should turn around
void IdentifyCorner::_turnAroundSecondLine()
{
    //_displayCurrentIntersectionCount();
    _display.clear();
    _display << "turn around";
    // if isRight, turn around from left wait for NO_ADJUSTMENT or LEFT_ADJUSTMENT or RIGHT_ADJUSTMENT
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
        _state = IdentifyCornerState::GO_BACK_SECOND_LINE;
    }
}

void IdentifyCorner::_turnAroundThirdLine()
{
    //_displayCurrentIntersectionCount();
    _display.clear();
    _display << "turn around";
    // if isRight, turn around from left wait for NO_ADJUSTMENT or LEFT_ADJUSTMENT or RIGHT_ADJUSTMENT
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
        _navModule.stop();
        _delay_ms(1000);
        _displayCurrentIntersectionCount();
        _delay_ms(2000);
        _state = IdentifyCornerState::GO_BACK_THIRD_LINE;
    }
}

void IdentifyCorner::_goBack()
{
    // do the same but don't increment, just go back until no line is detected (start position)
    LineMakerFlag flag = _lineMakerModule.getDetectionFlag();

    _navModule.go(140, false);
    switch (flag)
    {
    case LineMakerFlag::NO_LINE:
        _navModule.stop();
        _navModule.go(140, false);
        _delay_ms(1000);
        _navModule.stop();
        _delay_ms(1000);
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

void IdentifyCorner::_goBackSecondLine()
{
    // do the same but don't increment, just go back until no line is detected (start position)
    LineMakerFlag flag = _lineMakerModule.getDetectionFlag();

    _navModule.go(140, false);
    switch (flag)
    {
    case LineMakerFlag::NO_LINE:
        _navModule.stop();
        _navModule.go(140, false);
        _delay_ms(1000);
        _navModule.stop();
        _delay_ms(1000);
        _state = IdentifyCornerState::TURN_BACK_FIRST_LINE;
        break;
    case LineMakerFlag::LEFT_ADJUSTMENT:
        break;
    case LineMakerFlag::RIGHT_ADJUSTMENT:
        _navModule.adjustLeft();
        break;
    }
}

void IdentifyCorner::_goBackThirdLine()
{
    // do the same but don't increment, just go back until no line is detected (start position)
    LineMakerFlag flag = _lineMakerModule.getDetectionFlag();

    _navModule.go(140, false);
    switch (flag)
    {
    case LineMakerFlag::NO_LINE:
        _navModule.stop();
        _navModule.go(140, false);
        _delay_ms(1000);
        _navModule.stop();
        _delay_ms(1000);
        _state = IdentifyCornerState::TURN_BACK_SECOND_LINE;
        break;
    case LineMakerFlag::LEFT_ADJUSTMENT:
        _navModule.adjustRight();
        break;
    case LineMakerFlag::RIGHT_ADJUSTMENT:
        _navModule.adjustLeft();
        break;
    }
}

void IdentifyCorner::_turnSecondLine()
{

    // go forward for 1 second

    _navModule.go(140, false);
    _delay_ms(1000);
    _navModule.stop();
    _delay_ms(1000);

    // turn around from left
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

    // if (sensor == LineMaker::INNER_LEFT || sensor == LineMaker::INNER_RIGHT || sensor == LineMaker::NO_ADJUSTMENT)
    // {
    //     _delay_ms(1000);
    //     _navModule.stop();
    //     _display.clear();
    //     _display << "i am back";
    // }

    LineMakerFlag sensor = _lineMakerModule.getDetectionFlag();
    if (sensor == LineMakerFlag::LEFT_ADJUSTMENT || sensor == LineMakerFlag::RIGHT_ADJUSTMENT || sensor == LineMakerFlag::NO_ADJUSTMENT)
    {
        _delay_ms(1000);
        _navModule.stop();
        _delay_ms(1000);
        _state = IdentifyCornerState::TURN_BACK_FIRST_LINE;
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
    _navModule.go(140, false);
    _display = "sec line";
    switch (flag)
    {
    case LineMakerFlag::NO_LINE:
        _navModule.stop();
        _display << "simple";
        //_state = IdentifyCornerState::TURN_AROUND;
        if (_simpleCompareMAtch())
        // it founds the right spot since the second line
        {
            _display.clear();
            _display << "simple";
            //_delay_ms(5000);
            _state = IdentifyCornerState::TURN_AROUND_SECOND_LINE;
        }
        else
        {
            _display.clear();
            _display << "no simple";
            _state = IdentifyCornerState::TURN_THIRD_LINE;
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
            // Correct this in code instead of _firstLineCount, it's _secondLineCount
            _secondLineCount++;

            _displayCurrentIntersectionCount();
            _blockIncrementation = true;
        }
        _navModule.adjustLeft();
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
        _navModule.adjustRight();
        isRight = false;
        break;
    }
}

void IdentifyCorner::_turnThirdLine()
{

    // go forward for 1 second

    // turn around from left
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
        _state = IdentifyCornerState::GO_FORWARD_THIRD_LINE;
    }
}

void IdentifyCorner::_goForwardThirdLine()
{
    // go forward until no line is detected
    LineMakerFlag flag = _lineMakerModule.getDetectionFlag();
    _navModule.go(140, false);
    switch (flag)
    {
    case LineMakerFlag::NO_LINE:
        _navModule.stop();
        if (_furtherCompareMatch())
        {
            makeSound();
            _delay_ms(2000);
            _displayCurrentIntersectionCount();
        }
        // Later start process go back to initial place here
        _state = IdentifyCornerState::TURN_AROUND;
        break;
    case LineMakerFlag::LEFT_ADJUSTMENT:
        _navModule.adjustRight();
        break;
    case LineMakerFlag::RIGHT_ADJUSTMENT:
        _navModule.adjustLeft();
        break;

    case LineMakerFlag::OUTER_RIGHT_DETECTION:
    {
        if (!_blockIncrementation)
        {
            _firstLineCount++;
            _blockIncrementation = true;

            if (_furtherCompareMatch())
            {
                makeSound();

                _displayCurrentIntersectionCount();
                _state = IdentifyCornerState::TURN_AROUND;
            }
        }
        _navModule.adjustLeft();
        isRight = true;
        break;
    }

    case LineMakerFlag::OUTER_LEFT_DETECTION:
        if (!_blockIncrementation)
        {
            _secondLineCount++;
            _blockIncrementation = true;
            if (_furtherCompareMatch())
            {
                makeSound();

                _displayCurrentIntersectionCount();
                _state = IdentifyCornerState::TURN_AROUND;
            }
        }
        _navModule.adjustRight();
        isRight = false;
        break;
    }
}

// void IdentifyCorner::identificationProcess(uint8_t *_beginning)
// {
//     _display.clear();
//     _display << "Searching for corner...";
//     while (!_found)
//     {
//         switch (_state)
//         {
//         case IdentifyCornerState::GO_FORWARD_FIRST_LINE:
//             _goForward();
//             break;

//         case IdentifyCornerState::TURN_AROUND:
//             _turnAround();
//             break;

//         case IdentifyCornerState::GO_BACK:
//             _goBack();
//             _display = "GO_BACK";
//             _navModule.stop();
//             break;

//         case IdentifyCornerState::TURN_SECOND_LINE:
//             _turnSecondLine();
//             _display = "TURN_SECOND_LINE";
//             //_navModule.stop();
//             break;

//         case IdentifyCornerState::GO_FORWARD_SECOND_LINE:
//             _goForwardSecondLine();
//            // _display = "sec line";
//            break;

//         case IdentifyCornerState::TURN_THIRD_LINE:
//             _turnThirdLine();
//             _display = "TURN_THIRD_LINE";
//             break;

//         case IdentifyCornerState::GO_FORWARD_THIRD_LINE:
//             _goForwardThirdLine();
//             _display = "third line";
//             break;
//         }
//     }
//     // It found the right intersection so call function compare match

//     // complete here what's supposed to do when it's found

//     // write function switch case recognization corner

//     //            FirstLine       SecondLine
//     // RCBH ==>   32
//     // RCBV ==>  23

//     // RCTV ===> 2 intersections right -------  1 left
//     // LCTH ====> 2 intersections right  ------ 1 left *

//     // LCBV ==> 1 First intersection is left --- 1*
//     // LCBH ===> 1 first intersection  is right --- 1* and

//     // LCTV ===> 1 first intersection is left    --- 2*
//     // RCTH ===> 1 first intersection is left ------ 2*

//     return;
// }

// // go forward should follow the line using the line maker module until no line is detected
// void IdentifyCorner::_goForward()
// {
//     _display.clear();
//     _display << "GO FORWARD";
//     LineMakerFlag flag = _lineMakerModule.getDetectionFlag();
//     _navModule.go(140, false);
//     switch (flag)
//     {

//     case LineMakerFlag::RIGHT_ADJUSTMENT:
//         _blockIncrementation = false;
//         _navModule.adjustRight();
//         _delay_ms(100);
//         _navModule.stop();
//         _delay_ms(30);
//         break;

//     case LineMakerFlag::LEFT_ADJUSTMENT:
//         _blockIncrementation = false;
//         _navModule.adjustLeft();
//         _delay_ms(100);
//         _navModule.stop();
//         _delay_ms(30);
//         break;

//     case LineMakerFlag::OUTER_RIGHT_DETECTION:
//     {
//         if (!_blockIncrementation)
//         {
//             _firstLineCount++;
//             _sidefirst = true;
//             _displayCurrentIntersectionCount();
//             _blockIncrementation = true;
//         }
//         isRight = true;
//         break;
//     }

//     case LineMakerFlag::OUTER_LEFT_DETECTION:
//         if (!_blockIncrementation)
//         {
//             _firstLineCount++;
//             _displayCurrentIntersectionCount();
//             _blockIncrementation = true;
//         }
//         isRight = false;
//         break;

//     case LineMakerFlag::FULL_CROSSROAD:
//         if (!_blockIncrementation)
//         {
//             _firstLineCount++;
//             _displayCurrentIntersectionCount();
//             _blockIncrementation = true;
//         }
//         break;
//     case LineMakerFlag::NO_LINE:
//         _navModule.stop();
//         _state = IdentifyCornerState::TURN_AROUND;
//         break;
//     }
//     //_displayCurrentIntersectionCount();
// }

// void IdentifyCorner::makeSound()
// {
//     _sound.chooseFrequency(81);
// }
// void IdentifyCorner::_displayCurrentIntersectionCount()
// {
//     char displayString[16];
//     sprintf(displayString, "%d %d", _firstLineCount, _secondLineCount);
//     _display.clear();
//     _display << displayString;
// }

// bool IdentifyCorner::_simpleCompareMAtch()
// {
//     // add case 1-1 (right left and left right)
//     _displayCurrentIntersectionCount();
//     _delay_ms(5000);
//     if((_firstLineCount == 3) and (_secondLineCount == 2))
//     {
//         makeSound();
//         _delay_ms(2000);
//         _displayCurrentIntersectionCount();
//         return true;
//     }
//     else if((_firstLineCount == 2) and (_secondLineCount == 3))
//     {

//         _displayCurrentIntersectionCount();
//         makeSound();
//         return true;
//     }
//     else if( (_firstLineCount == 1) and (_secondLineCount == 1) and _sidefirst)
//     {
//          // LCBV
//         _displayCurrentIntersectionCount();
//         makeSound();
//         return true;
//     }
//     else if( (_firstLineCount == 1) and (_secondLineCount == 1 and !_sidefirst))
//     {
//          // LCBH
//         _displayCurrentIntersectionCount();
//         makeSound();
//         return true;
//     }
//     return false;
// }

// bool IdentifyCorner::_furtherCompareMatch()
// {
//     if(_firstLineCount == 3 and _secondLineCount == 1)
//     {
//         // LCTH -- good
//         _displayCurrentIntersectionCount();
//         makeSound();
//         return true;
//     }
//     else if(_firstLineCount == 1 and _secondLineCount == 3)
//     {
//         // LCTV
//          _displayCurrentIntersectionCount();
//         makeSound();
//         return true;
//     }
//     else if(_firstLineCount == 4 and _secondLineCount == 2)
//     {
//         // RCTH
//         _displayCurrentIntersectionCount();
//         makeSound();
//         return true;
//     }
//     else if(_firstLineCount == 4 and _secondLineCount == 1)
//     {
//          // RCTV this case has to be covered;

//         //_displayCurrentIntersectionCount();
//         //makeSound();
//         _display.clear();
//         _display << "not found";
//         return false;
//     }

// }

// // turn around should turn around
// void IdentifyCorner::_turnAround()
// {
//     //_displayCurrentIntersectionCount();
//      _display.clear();
//     _display << "turn around";
//     // if isRight, turn around from left wait for NO_ADJUSTMENT or LEFT_ADJUSTMENT or RIGHT_ADJUSTMENT
//     if (isRight)
//     {
//         _navModule.goRightWheel(120, false);
//         _navModule.goLeftWheel(140, true);
//     }
//     else
//     {
//         _navModule.goRightWheel(140, true);
//         _navModule.goLeftWheel(120, false);
//     }

//     uint8_t sensor = _lineMakerModule._retrieveSensorData();

//     if (sensor == LineMaker::INNER_LEFT || sensor == LineMaker::INNER_RIGHT || sensor == LineMaker::NO_ADJUSTMENT)
//     {
//         _navModule.stop();
//         _delay_ms(1000);
//         _displayCurrentIntersectionCount();
//         _delay_ms(2000);
//         _state = IdentifyCornerState::GO_BACK;
//     }
// }

// void IdentifyCorner::_goBack()
// {
//     // do the same but don't increment, just go back until no line is detected (start position)
//     LineMakerFlag flag = _lineMakerModule.getDetectionFlag();

//     _navModule.go(140, false);
//     switch (flag)
//     {
//     case LineMakerFlag::NO_LINE:
//         _navModule.stop();
//         _navModule.go(140, false);
//         _delay_ms(1000);
//         _navModule.stop();
//         _delay_ms(1000);
//         _state = IdentifyCornerState::TURN_SECOND_LINE;
//         break;
//     case LineMakerFlag::LEFT_ADJUSTMENT:
//         _navModule.adjustLeft();
//         _delay_ms(200);
//         _navModule.stop();
//         _delay_ms(70);
//         break;
//     case LineMakerFlag::RIGHT_ADJUSTMENT:
//         _navModule.adjustRight();
//         _delay_ms(200);
//         _navModule.stop();
//         _delay_ms(70);
//         break;
//     }
// }

// void IdentifyCorner::_turnSecondLine()
// {

//     // go forward for 1 second

//     // turn around from left
//     if (isRight)
//     {
//         _navModule.goRightWheel(120, false);
//         _navModule.goLeftWheel(140, true);
//     }
//     else
//     {
//         _navModule.goRightWheel(140, true);
//         _navModule.goLeftWheel(120, false);
//     }

//     uint8_t sensor = _lineMakerModule._retrieveSensorData();

//     if (sensor == LineMaker::INNER_LEFT || sensor == LineMaker::INNER_RIGHT || sensor == LineMaker::NO_ADJUSTMENT)
//     {
//         _navModule.stop();
//         _delay_ms(1000);
//         _state = IdentifyCornerState::GO_FORWARD_SECOND_LINE;
//     }
// }

// void IdentifyCorner::_goForwardSecondLine()
// {
//     // go forward until no line is detected
//     LineMakerFlag flag = _lineMakerModule.getDetectionFlag();
//     _navModule.go(140, false);
//     _display = "sec line";
//     switch (flag)
//     {
//     case LineMakerFlag::NO_LINE:
//         _navModule.stop();
//         _display << "simple";
//        //_state = IdentifyCornerState::TURN_AROUND;
//        if(_simpleCompareMAtch())
//        // it founds the right spot since the second line
//        {
//             _display.clear();
//             _display << "simple";
//             //_delay_ms(5000);
//             _state = IdentifyCornerState::TURN_AROUND;
//        }
//        else
//        {
//             _display.clear();
//            _display << "no simple";
//            _state = IdentifyCornerState::TURN_THIRD_LINE;
//       }
//         break;
//    case LineMakerFlag::RIGHT_ADJUSTMENT:
//         _blockIncrementation = false;
//         _navModule.adjustRight();
//         _delay_ms(100);
//         _navModule.stop();
//         _delay_ms(30);
//         break;

//     case LineMakerFlag::LEFT_ADJUSTMENT:
//         _blockIncrementation = false;
//         _navModule.adjustLeft();
//         _delay_ms(100);
//         _navModule.stop();
//         _delay_ms(30);
//         break;

//     case LineMakerFlag::OUTER_RIGHT_DETECTION:
//     {
//         if (!_blockIncrementation)
//         {
//             // Correct this in code instead of _firstLineCount, it's _secondLineCount
//             _secondLineCount++;

//             _displayCurrentIntersectionCount();
//             _blockIncrementation = true;
//         }
//         isRight = true;
//         break;
//     }

//     case LineMakerFlag::OUTER_LEFT_DETECTION:
//         if (!_blockIncrementation)
//         {
//             _secondLineCount++;
//             _displayCurrentIntersectionCount();
//             _blockIncrementation = true;
//         }
//         isRight = false;
//         break;
//     }

// }

// void IdentifyCorner::_turnThirdLine()
// {

//     // go forward for 1 second

//     // turn around from left
//     if (isRight)
//     {
//         _navModule.goRightWheel(120, false);
//         _navModule.goLeftWheel(140, true);
//     }
//     else
//     {
//         _navModule.goRightWheel(140, true);
//         _navModule.goLeftWheel(120, false);
//     }

//     uint8_t sensor = _lineMakerModule._retrieveSensorData();

//     if (sensor == LineMaker::INNER_LEFT || sensor == LineMaker::INNER_RIGHT || sensor == LineMaker::NO_ADJUSTMENT)
//     {
//         _navModule.stop();
//         _delay_ms(1000);
//         _state = IdentifyCornerState::GO_FORWARD_THIRD_LINE;
//     }
// }

// void IdentifyCorner::_goForwardThirdLine()
// {
//     // go forward until no line is detected
//     LineMakerFlag flag = _lineMakerModule.getDetectionFlag();
//     _navModule.go(140, false);
//     switch (flag)
//     {
//     case LineMakerFlag::NO_LINE:
//         _navModule.stop();
//         if(_furtherCompareMatch())
//         {
//             makeSound();
//             _delay_ms(2000);
//             _displayCurrentIntersectionCount();
//         }
//         // Later start process go back to initial place here
//          _state = IdentifyCornerState::TURN_AROUND;
//         break;
//     case LineMakerFlag::LEFT_ADJUSTMENT:
//         _navModule.adjustLeft();
//         _delay_ms(200);
//         _navModule.stop();
//         _delay_ms(70);
//         break;
//     case LineMakerFlag::RIGHT_ADJUSTMENT:
//         _navModule.adjustRight();
//         _delay_ms(200);
//         _navModule.stop();
//         _delay_ms(70);
//         break;

//     case LineMakerFlag::OUTER_RIGHT_DETECTION:
//     {
//         if (!_blockIncrementation)
//         {
//             _firstLineCount++;
//             _blockIncrementation = true;

//             if(_furtherCompareMatch())
//             {
//                 makeSound();

//                 _displayCurrentIntersectionCount();
//                  _state = IdentifyCornerState::TURN_AROUND;
//             }
//         }
//         isRight = true;
//         break;
//     }

//     case LineMakerFlag::OUTER_LEFT_DETECTION:
//         if (!_blockIncrementation)
//         {
//             _secondLineCount++;
//             _blockIncrementation = true;
//              if(_furtherCompareMatch())
//             {
//                 makeSound();

//                 _displayCurrentIntersectionCount();
//                  _state = IdentifyCornerState::TURN_AROUND;
//             }
//         }
//         isRight = false;
//         break;
//     }

// }