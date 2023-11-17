#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "lcm_so1602dtr_m_fw.h"
#include "customprocs.h"
#include "IdentifyCorner.h"
#include "Sound.h"



uint8_t stepRegistered = 0b00000000;


Sound sound; 

void _printLocalization(uint8_t step)
{
	switch (step)
	{
		case LCBV:
		break;

		case LCBH:
		break;

		case LCTV:
		break;

		case LCTH:
		break;

		case RCTH :
		break;

		case RCTV:
		break;

		case RCBV:
		break;

		case RCBH :
		break;
	}
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
		// Register Step;
		stepRegistered | = RIGHT ;
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
		// Register Step;
		stepRegistered | = RIGHT ;
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