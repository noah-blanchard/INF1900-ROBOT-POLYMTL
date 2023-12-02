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

ISR(INT2_vect)
{
	// select Choice
	_delay_ms(120);
	selectChoice = true;
	EIFR |= (1 << INTF0);
	// EIFR &= ~(1 << INTF0);
}

ISR(INT1_vect)
{
	// validate Choice
	_delay_ms(120);
	validateChoice = true;
	EIFR |= (1 << INTF1);
	// EIFR &= ~(1 << INTF1);
}

MakeTrip::MakeTrip() : _display(&DDRC, &PORTC) {}

void MakeTrip::run(uint8_t *destination)
{
	_state = selection::SELECTLINE;
	sei();
	selectChoice = false;
	validateChoice = false;
	_columnSeleted = 0;
	_lineSeleted = 0;
	_confirmed = false;
	sprintf(_buffer, "Line : %d", _lineSeleted + 1);
	_display = _buffer;
	while (_state != selection::FINISH)
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
			_confirmChoices();
			break;
		}
	}

	destination[0] = _columnSeleted;
	destination[1] = _lineSeleted;
}

void MakeTrip::_selectLine()
{
	if (selectChoice)
	{
		_lineSeleted = (_lineSeleted + 1) % 4;
		sprintf(_buffer, "Line : %d", _lineSeleted + 1);
		_display = _buffer;
		selectChoice = false;
		_delay_ms(50);
	}

	if (validateChoice)
	{
		sprintf(_buffer, "Col : %d", _columnSeleted + 1);
		_display = _buffer;
		_state = selection::SELECTCOLUMN;
		validateChoice = false;
		_delay_ms(50);
	}
}

void MakeTrip::_selectColumn()
{
	if (selectChoice)
	{
		_columnSeleted = (_columnSeleted + 1) % 7;
		sprintf(_buffer, "Col : %d", _columnSeleted + 1);
		_display = _buffer;
		selectChoice = false;
		_delay_ms(50);
	}

	if (validateChoice)
	{
		sprintf(_buffer, "(%d, %d) OK?\nOUI", _lineSeleted + 1, _columnSeleted + 1);
		_display = _buffer;
		_state = selection::CONFIRMCHOICES;
		validateChoice = false;
		_delay_ms(50);
	}
}

void MakeTrip::_confirmChoices()
{
	if (selectChoice)
	{
		_select = !_select;
		if (_select)
		{
			sprintf(_buffer, "(%d, %d) OK?\nOUI", _lineSeleted + 1, _columnSeleted + 1);
			_display = _buffer;
		}
		else
		{
			sprintf(_buffer, "(%d, %d) OK?\nNON", _lineSeleted + 1, _columnSeleted + 1);
			_display = _buffer;
		}
		selectChoice = false;
		_delay_ms(50);
	}

	if (validateChoice)
	{
		if (_select == true)
		{
			_confirmed = true;
			_state = selection::FINISH;
		}
		else
		{
			_confirmed = false;
			_columnSeleted = 0;
			_lineSeleted = 0;
			sprintf(_buffer, "Line : %d", _lineSeleted + 1);
			_display = _buffer;
			_state = selection::SELECTLINE;
		}
		validateChoice = false;
		_delay_ms(50);
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
					cli();
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
