#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "IdentifyCorner.h"




uint8_t stepRegistered = 0b00000000;
Sound sound; 

void _printLocalization(uint8_t step)
{
	LCM disp(&DDRC, &PORTC);
	disp.clear();
	switch (step)
	{
		case LCBV:
			disp << "(4,1) N";
		break;

		case LCBH:
			disp << "(4,1) E";
		break;

		case LCTV:
			disp << "(1,1) S";
		break;

		case LCTH:
			disp << "(1,1) E";
		break;

		case RCTH :
			disp << "(1,7) O";
		break;

		case RCTV:
			disp << "(1,7) S";
		break;

		case RCBV:
			disp << "(4,7) N";
		break;

		case RCBH :
			disp << "(4,7) O";
		break;
	}
}

bool _recognizeCorner(uint8_t registration)
{
	if(registration == LCBV ||registration=  LCBH ||registration =  LCTV ||registration=  LCTH || registration= RCTH || registration= RCTV 
	||registration= RCBV || registration= RCBH)
	{
		return true;
	}
	return false;
}

void _identificationProcess(uint8_t _beginning)
{
    Flag flag = _lineMakerModule.getDetectionFlag();

    switch (flag)
    {
    case Flag::NO_ADJUSTMENT:
    {
        _navModule.go(180, false);
		if(_recognizeCorner(stepRegistered))
		{	
			_navModule.stop();
			_beginning = stepRegistered;
			_printLocalization(stepRegistered)
			sound.chooseFrequency(81);
		}
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
		if(_recognizeCorner(stepRegistered))
		{
			_navModule.stop();
			_beginning = stepRegistered;
			_printLocalization(stepRegistered);
			sound.chooseFrequency(81);
		}
        break;
    }


    case Flag::LEFT_CROSSROAD:
    {
        _navModule.stop();
		stepRegistered | = LEFT ;
		_beginning = stepRegistered;
		if(stepRegistered == 0b00001101) break;
		if(_recognizeCorner(stepRegistered))
		{	
			 _navModule.stop();
			_beginning = stepRegistered;
			_printLocalization(stepRegistered);
			sound.chooseFrequency(81);
		}
		else
		{
			 _navModule.goLeftWheel(THESPEED, true);   
    		_navModule.goRightWheel(THESPEED, false); 
		}
        break;
    }

    case Flag::RIGHT_CROSSROAD:
    {
        _navModule.stop();
		stepRegistered | = RIGHT ;
		_beginning = stepRegistered;
		if(stepRegistered == 0b00001110) break;
		if(_recognizeCorner(stepRegistered))
		{	
			_navModule.stop();
			_beginning = stepRegistered;
			_printLocalization(stepRegistered);
			sound.chooseFrequency(81);
		}
		else
		{
			_navModule.goLeftWheel(THESPEED, false);
    		_navModule.goRightWheel(THESPEED, true); 
		}
        break;
    }

	case Flag::FULL_CROSSROAD:
    {
        _navModule.stop();
		stepRegistered | = BOTH ;
		if(_recognizeCorner(stepRegistered))
		{	
			_navModule.stop();
			_beginning = stepRegistered;
			_printLocalization(stepRegistered);
			sound.chooseFrequency(81);
		}
		else
		{
			_navModule.go(180, false);
		}
        break;
    }


    default:
    {
        _navModule.stop();
        break;
    }
    }

}