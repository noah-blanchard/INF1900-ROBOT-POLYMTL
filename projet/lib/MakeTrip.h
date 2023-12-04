#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>

#include "lcm_so1602dtr_m_fw.h"
#include "customprocs.h"
// #include "Bouton.h"

#define DEMO_DDR DDRC
#define DEMO_PORT PORTC

void static inline w(void)
{
	cp_wait_ms(2000);
};

enum class selection
{
	SELECTLINE,
	SELECTCOLUMN,
	CONFIRMCHOICES,
	FINISH
};

class MakeTrip
{
public:
	MakeTrip(volatile bool* mb, volatile bool* sel, volatile bool* val);
	void run(uint8_t *_destination);

private:
	// to retrieve the button pressed vars
	volatile bool *_mbButtonPressed;
	volatile bool *_selButtonPressed;
	volatile bool *_valButtonPressed;

	void _selectLine();
	void _selectColumn();
	void _confirmChoices();

	selection _state = selection::SELECTLINE;
	uint8_t _lineSeleted;
	uint8_t _columnSeleted;
	LCM _display;
	bool _select = true;
	bool _confirmed = false;

	char _buffer[25];
};