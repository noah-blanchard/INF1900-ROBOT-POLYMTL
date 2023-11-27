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

uint8_t lineSeleted = 0;
uint8_t columnSeleted = 0;
volatile uint8_t selectChoice = false;
volatile uint8_t validateChoice = false;


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

void MakeTrip::selectDestination(uint8_t* _destination)
{
	
	LCM disp(&DDRC, &PORTC);
	disp.clear();
	w();
	char buf[25];
	selection select = selection::SELECTLINE;
	while(true)
	{
		switch(select)
		{
			case selection::SELECTLINE:
			sprintf(buf, "Line : %d", lineSeleted+1);
			disp = buf;
				while(!validateChoice)
				{
					
					if(selectChoice)
					{
						lineSeleted = (lineSeleted +1) % 4;
						sprintf(buf, "Line : %d", lineSeleted+1);
						disp = buf;
						selectChoice = false;
					}
					
				}
				validateChoice = false;
				selectChoice = false;
				select = selection::SELECTCOLUMN;
				
            break;

			case selection::SELECTCOLUMN:
				disp.clear();
				sprintf(buf, "Col : %d", columnSeleted+1);
				disp = buf;
				while(!validateChoice)
				{
					if(selectChoice)
					{
						columnSeleted = (columnSeleted +1) % 7;
						sprintf(buf, "%d", columnSeleted+1);
						disp = buf;
						selectChoice = false;
					}
					
				}
				validateChoice = false;
				selectChoice = false;
				select = selection::CONFIRMCHOICES;
				disp.clear();
				sprintf(buf, "%d %d", lineSeleted+1,columnSeleted+1);
				disp = buf;
				w();
				disp << "ok ?";

            break;

			case selection::CONFIRMCHOICES:
				if(validateChoice)
				{
					disp << "C L";
					sprintf(buf, "L %d - C %d", lineSeleted+1,columnSeleted+1);
					disp = buf;
					w();
				}

				if(selectChoice)
				{
					lineSeleted = 1;
					columnSeleted=1;
					validateChoice = false;
					selectChoice = false;
					select = selection::SELECTLINE;
				}
				
            break;
		}
	}
}

