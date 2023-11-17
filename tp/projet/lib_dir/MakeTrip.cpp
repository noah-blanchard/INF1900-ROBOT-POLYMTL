#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>

#include "lcm_so1602dtr_m_fw.h"
#include "customprocs.h"
#include "MakeTrip.h"

#define DEMO_DDR	DDRC // `Data Direction Register' AVR occup� par l'aff.
#define DEMO_PORT	PORTC // Port AVR occup� par l'afficheur

uint8_t lineSeleted = 1;
uint8_t columnSeleted = 1;
volatile uint8_t selectChoice = false;
volatile uint8_t validateChoice = false;

void static inline w(void) {
	cp_wait_ms(2000);
}_destination

ISR ( INT0_vect) {
// select Choice
selectChoice = true;
EIFR |= (1 << INTF0) ;

}

ISR ( INT1_vect) {
// validate Choice
validateChoice = true;
EIFR |= (1 << INTF1) ;

}


MakeTrip::MakeTrip(){}

void MakeTrip::selectDestinations(uint8_t _destination[2])
{
	cli();
	toSelect.enableInterrupt();
	toValidate.enableInterrupt();
	sei();

	toSelect.setRisingEdge();
	toValidate.setRisingEdge();

	LCM disp(&DDRC, &PORTC);
	disp.clear();
	w();
	char buf[25];
	selection select = selection::INITIAL;
	while(true)
	{
		switch(select)
		{
			case selection::INITIAL:
				lineSeleted = 1;
				// print line on LCD
 				disp << "LINE";
				w();
				sprintf(buf, "%d", lineSeleted);
				disp = buf;
				w();

				select = selection::selectLine;
            break;


			case selection::selectLine:
				while(!validateChoice)
				{
					if(selectChoice)
					{
						// ====> section critique ===> block interruptions
						lineSeleted = (lineSeleted +1) % 4;
						// upgrade Line on LCD
						sprintf(buf, "Line : %d", lineSeleted+1);
						disp = buf;
						w();
					}
					selectChoice = false;
				}
				validateChoice = false;
				
				select = selection::selectColumn;
				// Print line validated on LCD
				disp << "COLUMN";
				w();
				sprintf(buf, "Column : %d", columnSeleted+1);
				disp = buf;
				w();
            break;

			case selection::selectColumn:
				columnSeleted = 1;

				while(!validateChoice)
				{
					if(selectChoice)
					{
						// ====> section critique ===> block interruptions
						columnSeleted = (columnSeleted +1) % 7;
						// upgrade column on LCD
						sprintf(buf, "%d", columnSeleted+1);
						disp = buf;
						w();
					}
					selectChoice = false;
				}
				validateChoice = false;
				select = selection::confirmChoices;
				// Print choices on LCD
				disp << "C L";
				sprintf(buf, "%d %d", lineSeleted+1,columnSeleted+1);
				disp = buf;
				w();

            break;

			case selection::confirmChoices:
				if(validateChoice)
				{
					disp << "C L";
					sprintf(buf, "%d %d", lineSeleted+1,columnSeleted+1);
					disp = buf;
					w();
				}
				if()
            break;
		}
	}
}

