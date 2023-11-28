#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>

#include "lcm_so1602dtr_m_fw.h"
#include "customprocs.h"
//#include "Bouton.h"

#define DEMO_DDR	DDRC 
#define DEMO_PORT	PORTC 


void static inline w(void) {
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
    MakeTrip();
   void selectDestination( uint8_t* _destination);
   void run(uint8_t* _destination);
private:

	void _selectLine();
	void _selectColumn();
	void _confirmChoices();

	bool _confirmed = false;
	selection _state = selection::SELECTLINE;
	uint8_t _lineSeleted;
	uint8_t _columnSeleted;
	LCM _display;

	char _buffer[25];
};