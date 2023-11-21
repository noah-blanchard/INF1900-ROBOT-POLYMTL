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

        case IdentifyCornerState::GO_BACK:
            _goBack();
            _display = "GO_BACK";
            _navModule.stop();
            break;

        case IdentifyCornerState::TURN_SECOND_LINE:
            _turnSecondLine();
            _display = "TURN_SECOND_LINE";
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
        _navModule.adjustRight();
        _delay_ms(100);
        _navModule.stop();
        _delay_ms(30);
        break;

    case LineMakerFlag::LEFT_ADJUSTMENT:
        _blockIncrementation = false;
        _navModule.adjustLeft();
        _delay_ms(100);
        _navModule.stop();
        _delay_ms(30);
        break;

    case LineMakerFlag::RIGHT_CROSSROAD:
    {
        if (!_blockIncrementation)
        {
            _firstLineCount++;
            _sidefirst = true;
            _displayCurrentIntersectionCount();
            _blockIncrementation = true;
        }
        isRight = true;
        break;
    }

    case LineMakerFlag::LEFT_CROSSROAD:
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
    if(((_firstLineCount == 3) and (_secondLineCount == 2)) ||((_firstLineCount == 2) and (_secondLineCount == 3)) )
    {
        makeSound();
        _delay_ms(2000);
        _displayCurrentIntersectionCount();
        return true;
    } else if( (_firstLineCount == 1) and (_secondLineCount == 1) and _sidefirst)
    {
         // LCBV
        _displayCurrentIntersectionCount();
        makeSound();
        return true;
    }
    else if( (_firstLineCount == 1) and (_secondLineCount == 1 and !_sidefirst))
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
    if(_firstLineCount == 3 and _secondLineCount == 1)
    {
        // LCTH -- good
        _displayCurrentIntersectionCount();
        makeSound();
        return true;
    }
    else if(_firstLineCount == 1 and _secondLineCount == 3)
    {
        // LCTV
         _displayCurrentIntersectionCount();
        makeSound();
        return true;
    }
    else if(_firstLineCount == 4 and _secondLineCount == 2)
    {
        // RCTH
        _displayCurrentIntersectionCount();
        makeSound();
        return true;
    }
    else if(_firstLineCount == 4 and _secondLineCount == 1)
    {
         // RCTV this case has to be covered;
         
        //_displayCurrentIntersectionCount();
        //makeSound();
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
        _navModule.goRightWheel(120, false);
        _navModule.goLeftWheel(140, true);
    }
    else
    {
        _navModule.goRightWheel(140, true);
        _navModule.goLeftWheel(120, false);
    }

    uint8_t sensor = _lineMakerModule._retrieveSensorData();

    if (sensor == LineMaker::INNER_LEFT || sensor == LineMaker::INNER_RIGHT || sensor == LineMaker::MIDDLE)
    {
        _navModule.stop();
        _delay_ms(1000);
        _displayCurrentIntersectionCount();
        _delay_ms(2000);
        _state = IdentifyCornerState::GO_BACK;
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
        _navModule.adjustLeft();
        _delay_ms(200);
        _navModule.stop();
        _delay_ms(70);
        break;
    case LineMakerFlag::RIGHT_ADJUSTMENT:
        _navModule.adjustRight();
        _delay_ms(200);
        _navModule.stop();
        _delay_ms(70);
        break;
    }
}

void IdentifyCorner::_turnSecondLine()
{

    // go forward for 1 second

    // turn around from left
    if (isRight)
    {
        _navModule.goRightWheel(120, false);
        _navModule.goLeftWheel(140, true);
    }
    else
    {
        _navModule.goRightWheel(140, true);
        _navModule.goLeftWheel(120, false);
    }

    uint8_t sensor = _lineMakerModule._retrieveSensorData();

    if (sensor == LineMaker::INNER_LEFT || sensor == LineMaker::INNER_RIGHT || sensor == LineMaker::MIDDLE)
    {
        _navModule.stop();
        _delay_ms(1000);
        _state = IdentifyCornerState::GO_FORWARD_SECOND_LINE;
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
       if(_simpleCompareMAtch())
       // it founds the right spot since the second line
       {
            _display.clear();
            _display << "simple";
            //_delay_ms(5000);
            _state = IdentifyCornerState::TURN_AROUND;
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
        _navModule.adjustRight();
        _delay_ms(100);
        _navModule.stop();
        _delay_ms(30);
        break;

    case LineMakerFlag::LEFT_ADJUSTMENT:
        _blockIncrementation = false;
        _navModule.adjustLeft();
        _delay_ms(100);
        _navModule.stop();
        _delay_ms(30);
        break;

    case LineMakerFlag::RIGHT_CROSSROAD:
    {
        if (!_blockIncrementation)
        {
            // Correct this in code instead of _firstLineCount, it's _secondLineCount
            _secondLineCount++;

            _displayCurrentIntersectionCount();
            _blockIncrementation = true;
        }
        isRight = true;
        break;
    }

    case LineMakerFlag::LEFT_CROSSROAD:
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

void IdentifyCorner::_turnThirdLine()
{

    // go forward for 1 second

    // turn around from left
    if (isRight)
    {
        _navModule.goRightWheel(120, false);
        _navModule.goLeftWheel(140, true);
    }
    else
    {
        _navModule.goRightWheel(140, true);
        _navModule.goLeftWheel(120, false);
    }

    uint8_t sensor = _lineMakerModule._retrieveSensorData();

    if (sensor == LineMaker::INNER_LEFT || sensor == LineMaker::INNER_RIGHT || sensor == LineMaker::MIDDLE)
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
        if(_furtherCompareMatch())
        {
            makeSound();
            _delay_ms(2000);
            _displayCurrentIntersectionCount();
        }
        // Later start process go back to initial place here
         _state = IdentifyCornerState::TURN_AROUND;
        break;
    case LineMakerFlag::LEFT_ADJUSTMENT:
        _navModule.adjustLeft();
        _delay_ms(200);
        _navModule.stop();
        _delay_ms(70);
        break;
    case LineMakerFlag::RIGHT_ADJUSTMENT:
        _navModule.adjustRight();
        _delay_ms(200);
        _navModule.stop();
        _delay_ms(70);
        break;

    case LineMakerFlag::RIGHT_CROSSROAD:
    {
        if (!_blockIncrementation)
        {
            _firstLineCount++;
            _blockIncrementation = true;

            if(_furtherCompareMatch())
            {
                makeSound();
            
                _displayCurrentIntersectionCount();
                 _state = IdentifyCornerState::TURN_AROUND;
            }
        }
        isRight = true;
        break;
    }

    case LineMakerFlag::LEFT_CROSSROAD:
        if (!_blockIncrementation)
        {
            _secondLineCount++;
            _blockIncrementation = true;
             if(_furtherCompareMatch())
            {
                makeSound();
         
                _displayCurrentIntersectionCount();
                 _state = IdentifyCornerState::TURN_AROUND;
            }
        }
        isRight = false;
        break;
    }

}

    // void IdentifyCorner::identificationProcess(uint8_t *_beginning)
    // {
    //     _display = "Searching for corner...";
    //     while (!_found)
    //     {
    //         switch (_state)
    //         {
    //         case IdentifyCornerState::GO_FORWARD:
    //             _goForward();
    //             break;

    //         case IdentifyCornerState::DETECT_INTERSECTION:
    //             _detectIntersection();
    //             break;

    //         case IdentifyCornerState::DETECT_FORWARD:
    //             _detectForward();
    //             break;

    //         case IdentifyCornerState::TURN:
    //             _turn();
    //             break;

    //         case IdentifyCornerState::RECOGNIZE:
    //             _matchSequence();
    //             break;

    //         case IdentifyCornerState::STOP:
    //             //_stop();
    //             break;
    //         }
    //     }
    //     return;
    // }

    // // go forward should follow the line using the line maker module
    // void IdentifyCorner::_goForward()
    // {
    //     _display = "GO FORWARD";
    //     LineMakerFlag flag = _lineMakerModule.getDetectionFlag();
    //     switch (flag)
    //     {
    //     case LineMakerFlag::NO_ADJUSTMENT:
    //         _navModule.go(120, false);
    //         break;

    //     case LineMakerFlag::RIGHT_ADJUSTMENT:
    //         _navModule.adjustRight();
    //         break;

    //     case LineMakerFlag::LEFT_ADJUSTMENT:
    //         _navModule.adjustLeft();
    //         break;

    //     case LineMakerFlag::RIGHT_CROSSROAD:
    //         _navModule.stop();
    //         _state = IdentifyCornerState::DETECT_INTERSECTION;
    //         break;

    //     case LineMakerFlag::LEFT_CROSSROAD:
    //         _navModule.stop();
    //         _state = IdentifyCornerState::DETECT_INTERSECTION;
    //         break;

    //     case LineMakerFlag::FULL_CROSSROAD:
    //         _navModule.stop();
    //         _state = IdentifyCornerState::DETECT_INTERSECTION;
    //         break;
    //     }
    // }

    // // detect intersection should detect the intersection
    // void IdentifyCorner::_detectIntersection()
    // {
    //     _display = "DETECT INTERSECT";
    //     _delay_ms(2500);
    //     LineMakerFlag flag = _lineMakerModule.getDetectionFlag();
    //     uint8_t sequenceToAdd = 0;
    //     // use bitshift to add at the right place
    //     switch (flag)
    //     {
    //     case LineMakerFlag::RIGHT_CROSSROAD:
    //         sequenceToAdd |= (RIGHT);
    //         _display = "RIGHT DETECT";
    //         _delay_ms(2500);
    //         break;

    //     case LineMakerFlag::LEFT_CROSSROAD:
    //         sequenceToAdd |= (LEFT);
    //         _display = "LEFT DETECT";
    //         _delay_ms(2500);
    //         break;

    //     case LineMakerFlag::FULL_CROSSROAD:
    //         sequenceToAdd |= (LEFT | RIGHT);
    //         _display = "FULL DETECT";
    //         _delay_ms(2500);
    //         break;
    //     }
    //     _currentSequence |= (sequenceToAdd);
    //     _state = IdentifyCornerState::DETECT_FORWARD;
    // }

    // // detect forward should detect the forward
    // void IdentifyCorner::_detectForward()
    // {
    //     // go a bit forward, then wait
    //     _navModule.go(120, false);
    //     _delay_ms(1000);
    //     _navModule.stop();
    //     _display = "LOOKING FORWARD";
    //     _delay_ms(2500);
    //     LineMakerFlag flag = _lineMakerModule.getDetectionFlag();
    //     uint8_t sequenceToAdd = 0;
    //     // if flag == no line then there's no forward
    //     if (!(flag == LineMakerFlag::NO_LINE))
    //     {
    //         sequenceToAdd |= (FORWARD);
    //         _display = "FORWARD DETECTED";
    //         _delay_ms(2500);
    //     }
    //     _currentSequence |= (sequenceToAdd);
    //     // add the current sequence into th _sequence with tteh bitshift than increment
    //     _sequence |= (_currentSequence << _bitshift);
    //     _bitshift += 3;

    //     // if there's forward in the sequence, just go back to go forward
    //     if ((_currentSequence & FORWARD) != 0)
    //     {
    //         _state = IdentifyCornerState::RECOGNIZE;
    //     }
    //     else
    //     {
    //         // if there's no forward, then we need to turn
    //         _state = IdentifyCornerState::TURN;
    //     }
    // }

    // // turn should turn
    // void IdentifyCorner::_turn()
    // {

    //     // determine if left or right using the current sequence
    //     bool isLeft = false;
    //     if ((_currentSequence & LEFT) != 0)
    //     {
    //         isLeft = true;
    //     }
    //     else if ((_currentSequence & RIGHT) != 0)
    //     {
    //         isLeft = false;
    //     }

    //     // if (isLeft)
    //     // {
    //     //     _navModule.goRightWheel(120, false);
    //     //     _navModule.goLeftWheel(120, true);
    //     // }
    //     // else
    //     // {
    //     //     _navModule.goRightWheel(120, true);
    //     //     _navModule.goLeftWheel(120, false);
    //     // }

    //     // turn for 250 ms in the right direction
    //     if (isLeft)
    //     {
    //         _navModule.goRightWheel(120, false);
    //         _navModule.goLeftWheel(120, true);
    //         _delay_ms(250);
    //     }
    //     else
    //     {
    //         _navModule.goRightWheel(120, true);
    //         _navModule.goLeftWheel(120, false);
    //         _delay_ms(250);
    //     }

    //     // stop
    //     _navModule.stop();

    //     _display = "TURNING";

    //     // now turn until the line is found (left adjustment for right turn, right adjustment for left turn)
    //     if (isLeft)
    //     {
    //         _navModule.goRightWheel(120, false);
    //         _navModule.goLeftWheel(120, true);
    //         while (_lineMakerModule.getDetectionFlag() != LineMakerFlag::RIGHT_ADJUSTMENT)
    //         {
    //         }
    //     }
    //     else
    //     {
    //         _navModule.goRightWheel(120, true);
    //         _navModule.goLeftWheel(120, false);
    //         while (_lineMakerModule.getDetectionFlag() != LineMakerFlag::LEFT_ADJUSTMENT)
    //         {
    //         }
    //     }

    //     // stop
    //     _navModule.stop();

    //     _currentSequence = 0;
    //     _state = IdentifyCornerState::RECOGNIZE;
    // }

    // void IdentifyCorner::_matchSequence()
    // {
    //     _display = "MATCHING";
    //     _delay_ms(4000);
    //     // if the sequence is the same as the one we're looking for, then we found it
    //     if (_sequence == LCTH)
    //     {
    //         _display = "LCTH";
    //         _found = true;
    //     }
    //     else if (_sequence == LCBV)
    //     {
    //         _display = "LCBV";
    //         _found = true;
    //     }
    //     else if (_sequence == LCBH)
    //     {
    //         _display = "LCBH";
    //         _found = true;
    //     }
    //     else if (_sequence == LCTV)
    //     {
    //         _display = "LCTV";
    //         _found = true;
    //     }
    //     else if (_sequence == RCTH)
    //     {
    //         _display = "RCTH";
    //         _found = true;
    //     }
    //     else if (_sequence == RCTV)
    //     {
    //         _display = "RCTV";
    //         _found = true;
    //     }
    //     else if (_sequence == RCBV)
    //     {
    //         _display = "RCBV";
    //         _found = true;
    //     }
    //     else if (_sequence == RCBH)
    //     {
    //         _display = "RCBH";
    //         _found = true;
    //     }
    //     else
    //     {
    //         _display = "NOT FOUND";
    //         _found = false;
    //     }
    //     _delay_ms(4000);
    //     _state = IdentifyCornerState::GO_FORWARD;
    // }

    // void IdentifyCorner::_displayCurrentSequence()
    // {
    //     // display the current sequence
    //     char displayString[16];
    //     // sprintf(displayString, "%d", _currentSequence);
    //     //  use sprint f to format as bits display not decim

    //     // display in base 2 (binary) not decimal
    //     sprintf(displayString, "%d", _currentSequence);

    //     _display = displayString;
    // }