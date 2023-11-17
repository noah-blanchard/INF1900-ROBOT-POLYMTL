#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "IdentifyCorner.h"

uint8_t stepRegistered = 0b000000;

// IdentifyCorner::IndentifyCorner(){}

void IdentifyCorner::_printLocalization(uint8_t step)
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

	case RCTH:
		disp << "(1,7) O";
		break;

	case RCTV:
		disp << "(1,7) S";
		break;

	case RCBV:
		disp << "(4,7) N";
		break;

	case RCBH:
		disp << "(4,7) O";
		break;
	}
}

bool IdentifyCorner::_recognizeCorner(uint8_t registration)
{
	if (registration == LCBV || registration == LCBH || registration == LCTV || registration == LCTH || registration == RCTH || registration == RCTV || registration == RCBV || registration == RCBH)
	{
		return true;
	}
	return false;
}

void IdentifyCorner::identificationProcess(uint8_t *_beginning)
{

	Flag flag = _lineMakerModule.getDetectionFlag();
	LCM disp(&DDRC, &PORTC);
	disp.clear();
	disp << "Searching...";

	switch (flag)
	{
	case Flag::NO_ADJUSTMENT:
	{
		_navModule.go(180, false);
		break;
	}
	case Flag::RIGHT_CROSSROAD:
	{
		_navModule.stop();
		_stepRegistered |= (RIGHT << _bitshift);
		_bitshift += 2;
		// turn right
		_turnRight();
		// mettre right à la
	}
	case Flag::LEFT_CROSSROAD:
	{
		_navModule.stop();
		_stepRegistered |= (LEFT << _bitshift);
		_bitshift += 2;
		// turn left
		_turnLeft();
		if (_stepRegistered == LCBV)
		{
			disp << "LCBV";
		}
		// mettre left à la
	}
	}

	// void IdentifyCorner::identificationProcess(uint8_t _beginning)
	// {
	// 	Flag flag = _lineMakerModule.getDetectionFlag();
	// 	LCM disp(&DDRC, &PORTC);
	// 	disp.clear();
	// 	disp << "Searching";
	// 	switch (flag)
	// 	{
	// 	case Flag::NO_ADJUSTMENT:
	// 	{
	// 		_navModule.go(180, false);
	// 		if (_recognizeCorner(_stepRegistered))
	// 		{

	// 			_navModule.stop();
	// 			_beginning = _stepRegistered;
	// 			_printLocalization(_stepRegistered);
	// 			_sound.chooseFrequency(81);
	// 		}

	// 		break;
	// 	}
	// 	case Flag::LEFT_ADJUSTMENT:
	// 	{
	// 		_navModule.adjustLeft();
	// 		_delay_ms(200);
	// 		_navModule.stop();
	// 		_delay_ms(80);
	// 		break;
	// 	}
	// 	case Flag::RIGHT_ADJUSTMENT:
	// 	{
	// 		_navModule.adjustRight();
	// 		_delay_ms(200);
	// 		_navModule.stop();
	// 		_delay_ms(80);
	// 		break;
	// 	}
	// 	case Flag::NO_LINE:
	// 	{
	// 		_navModule.stop();
	// 		if (_recognizeCorner(_stepRegistered))
	// 		{
	// 			_navModule.stop();
	// 			_beginning = _stepRegistered;
	// 			_printLocalization(_stepRegistered);
	// 			_sound.chooseFrequency(81);
	// 		}
	// 		break;
	// 	}

	// 	case Flag::LEFT_CROSSROAD:
	// 	{
	// 		_navModule.stop();
	// 		_stepRegistered |= LEFT;
	// 		_beginning = _stepRegistered;
	// 		if (_stepRegistered == 0b00001101)
	// 			break;
	// 		if (_recognizeCorner(_stepRegistered))
	// 		{
	// 			_navModule.stop();
	// 			_beginning = _stepRegistered;
	// 			_printLocalization(_stepRegistered);
	// 			_sound.chooseFrequency(81);
	// 		}
	// 		else
	// 		{
	// 			_turnLeft();
	// 		}
	// 		break;
	// 	}

	// 	case Flag::RIGHT_CROSSROAD:
	// 	{
	// 		_navModule.stop();
	// 		_stepRegistered |= RIGHT;
	// 		_beginning = _stepRegistered;
	// 		if (_stepRegistered == 0b00001110)
	// 			break;
	// 		if (_recognizeCorner(_stepRegistered))
	// 		{
	// 			_navModule.stop();
	// 			_beginning = _stepRegistered;
	// 			_printLocalization(_stepRegistered);
	// 			_sound.chooseFrequency(81);
	// 		}
	// 		else
	// 		{
	// 			_turnRight();
	// 		}
	// 		break;
	// 	}

	// 	case Flag::FULL_CROSSROAD:
	// 	{
	// 		_navModule.stop();
	// 		_stepRegistered |= BOTH;
	// 		if (_recognizeCorner(_stepRegistered))
	// 		{
	// 			_navModule.stop();
	// 			_beginning = _stepRegistered;
	// 			_printLocalization(_stepRegistered);
	// 			_sound.chooseFrequency(81);
	// 		}
	// 		else
	// 		{
	// 			_navModule.go(180, false);
	// 		}
	// 		break;
	// 	}

	// 	default:
	// 	{
	// 		_navModule.stop();
	// 		break;
	// 	}
	// 	}
	// }

	void IdentifyCorner::_turnRight()
	{
		// while line maker doesnt meet NO_ADJUSTMENT
		while (_lineMakerModule.getDetectionFlag() != Flag::LEFT_ADJUSTMENT)
		{
			_navModule.goLeftWheel(SLOW_TURN_SPEED, false);
			_navModule.goRightWheel(SLOW_TURN_SPEED, true);
		}
	}

	void IdentifyCorner::_turnLeft()
	{
		// while line maker doesnt meet NO_ADJUSTMENT
		while (_lineMakerModule.getDetectionFlag() != Flag::RIGHT_ADJUSTMENT)
		{
			_navModule.goRightWheel(SLOW_TURN_SPEED, false);
			_navModule.goLeftWheel(SLOW_TURN_SPEED, true);
		}
	}