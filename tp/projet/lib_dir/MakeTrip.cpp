#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>

#include "lcm_so1602dtr_m_fw.h"
#include "customprocs.h"
#include "MakeTrip.h"

MakeTrip::MakeTrip(volatile bool *mb, volatile bool *sel, volatile bool *val) : _display(&DDRC, &PORTC), _mbButtonPressed(mb), _selButtonPressed(sel), _valButtonPressed(val) {}

void MakeTrip::run(uint8_t *destination)
{
	_state = selection::SELECTLINE;
	_columnSeleted = 0;
	_lineSeleted = 0;
	_confirmed = false;
	sprintf(_buffer, "Line : %d", _lineSeleted + 1);
	_display = _buffer;
	while (_state != selection::FINISH)
	{
		*_mbButtonPressed = false;
		*_selButtonPressed = false;
		*_valButtonPressed = false;
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
	if (*_selButtonPressed)
	{
		_lineSeleted = (_lineSeleted + 1) % 4;
		sprintf(_buffer, "Line : %d", _lineSeleted + 1);
		_display = _buffer;
		*_selButtonPressed = false;
	}

	if (*_valButtonPressed)
	{
		sprintf(_buffer, "Col : %d", _columnSeleted + 1);
		_display = _buffer;
		_state = selection::SELECTCOLUMN;
		*_valButtonPressed = false;
	}
}

void MakeTrip::_selectColumn()
{
	if (*_selButtonPressed)
	{
		_columnSeleted = (_columnSeleted + 1) % 7;
		sprintf(_buffer, "Col : %d", _columnSeleted + 1);
		_display = _buffer;
		*_selButtonPressed = false;
	}

	if (*_valButtonPressed)
	{
		sprintf(_buffer, "(%d, %d) OK?", _lineSeleted + 1, _columnSeleted + 1);
		_display = _buffer;
		_display.write("OUI", LCM_FW_HALF_CH);
		_state = selection::CONFIRMCHOICES;
		*_valButtonPressed = false;
	}
}

void MakeTrip::_confirmChoices()
{
	if (*_selButtonPressed)
	{
		_select = !_select;
		if (_select)
		{
			sprintf(_buffer, "(%d, %d) OK?", _lineSeleted + 1, _columnSeleted + 1);
			_display = _buffer;
    		_display.write("OUI", LCM_FW_HALF_CH);
		}
		else
		{
			sprintf(_buffer, "(%d, %d) OK?", _lineSeleted + 1, _columnSeleted + 1);
			_display = _buffer;
			_display.write("NON", LCM_FW_HALF_CH);
		}
		*_selButtonPressed = false;
	}

	if (*_valButtonPressed)
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
		*_valButtonPressed = false;
	}
}