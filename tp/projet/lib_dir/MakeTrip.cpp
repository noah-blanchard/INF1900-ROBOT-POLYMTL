#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>

#include "lcm_so1602dtr_m_fw.h"
#include "customprocs.h"
#include "MakeTrip.h"

#define DEMO_DDR DDRC	// `Data Direction Register' AVR occup� par l'aff.
#define DEMO_PORT PORTC // Port AVR occup� par l'afficheur

volatile uint8_t selectChoice = false;
volatile uint8_t validateChoice = false;

ISR(INT0_vect)
{
	// select Choice
	selectChoice = true;
	EIFR |= (1 << INTF0);
	// EIFR &= ~(1 << INTF0);
}

ISR(INT1_vect)
{
	// validate Choice
	validateChoice = true;
	EIFR |= (1 << INTF1);
	// EIFR &= ~(1 << INTF1);
}

MakeTrip::MakeTrip() : _display(&DDRC, &PORTC) {}

void MakeTrip::run(uint8_t* destination)
{
	_columnSeleted = 0;
	_lineSeleted = 0;
	sprintf(_buffer, "Line : %d", _lineSeleted + 1);
	_display = _buffer;
	while (!_confirmed)
	{
		validateChoice = false;
		switch (_state)
		{
		case selection::SELECTLINE:
			_selectLine();
			break;
		case selection::SELECTCOLUMN:
			_selectColumn();
			break;
		case selection::CONFIRMCHOICES:
			__confirmChoices();
			break;
		}
	}
}

void MakeTrip::_selectLine()
{
	if (selectChoice)
	{
		sprintf(_buffer, "Line : %d", _lineSeleted + 1);
		_display = _buffer;
		_lineSeleted = (_lineSeleted + 1) % 4;
		selectChoice = false;
	}

	if (validateChoice)
	{
		sprintf(_buffer, "Col : %d", _columnSeleted + 1);
		_display = _buffer;
		_state = selection::SELECTCOLUMN;
		validateChoice = false;
	}
}

void MakeTrip::_selectColumn()
{
	if (selectChoice)
	{
		sprintf(_buffer, "Col : %d", _columnSeleted + 1);
		_display = _buffer;
		_columnSeleted = (_columnSeleted + 1) % 4;
		selectChoice = false;
	}

	if (validateChoice)
	{
		_state = selection::CONFIRMCHOICES;
		validateChoice = false;
	}
}

void MakeTrip::selectDestination(uint8_t *_destination)
{

	LCM disp(&DDRC, &PORTC);
	disp.clear();
	// w();
	char _buffer[25];
	selection select = selection::SELECTLINE;
	while (select != selection::FINISH)
	{
		switch (select)
		{
		case selection::SELECTLINE:
			validateChoice = false;
			while (!validateChoice)
			{
				sprintf(_buffer, "Line : %d", _lineSeleted + 1);
				disp = _buffer;
				//_delay_ms(3000);
				if (selectChoice)
				{
					_lineSeleted = (_lineSeleted + 1) % 4;
					sprintf(_buffer, "Line : %d", _lineSeleted + 1);
					disp = _buffer;
					selectChoice = false;
				}
				// disp = "heyyy";
				//_delay_ms(2000);
				// if(validateChoice)
				//{
				// validateChoice = false;
				// selectChoice = false;
				// select = selection::SELECTCOLUMN;
				//}
			}

			disp = "hiii";
			_delay_ms(2000);
			validateChoice = false;
			selectChoice = false;
			select = selection::SELECTCOLUMN;
			break;

		case selection::SELECTCOLUMN:
			// disp.clear();
			// sprintf(_buffer, "Col : %d", _columnSeleted+1);
			// disp = _buffer;
			while (!validateChoice)
			{
				if (selectChoice)
				{
					_columnSeleted = (_columnSeleted + 1) % 7;
					sprintf(_buffer, "Col : %d", _columnSeleted + 1);
					disp = _buffer;
					// sprintf(_buffer, "val : %d", validateChoice);
					// disp = _buffer;

					selectChoice = false;
				}

				// if(validateChoice)
				//{
				// disp.clear();
				// sprintf(_buffer, "L %d - C %d - ok?", _lineSeleted+1,_columnSeleted+1);
				// disp = _buffer;
				// validateChoice = false;
				// selectChoice = false;
				// select = selection::CONFIRMCHOICES;
				//}
			}
			disp.clear();
			sprintf(_buffer, "L %d - C %d - ok?", _lineSeleted + 1, _columnSeleted + 1);
			disp = _buffer;
			validateChoice = false;
			selectChoice = false;
			select = selection::CONFIRMCHOICES;
			break;

		case selection::CONFIRMCHOICES:
			while (!validateChoice || !selectChoice)
			{
				if (validateChoice)
				{
					sprintf(_buffer, "L %d - C %d", _lineSeleted + 1, _columnSeleted + 1);
					disp = _buffer;
					select = selection::FINISH;
					// w();
				}

				if (selectChoice)
				{
					_lineSeleted = 1;
					_columnSeleted = 1;
					validateChoice = false;
					selectChoice = false;
					select = selection::SELECTLINE;
				}
			}
			break;
		}
	}
	disp = "HQHQ";
	_destination[0] = _columnSeleted;
	_destination[1] = _lineSeleted;
}
