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
	selection select = selection::selectLine;
	while(true)
	{
		switch(select)
		{
			// case selection::INITIAL:
			// 	// print line on LCD
 			// 	disp << "LINE";
			// 	w();
			// 	sprintf(buf, "%d", lineSeleted+1);
			// 	disp = buf;
			// 	w();

			// 	select = selection::selectLine;
            // break;


			case selection::selectLine:
			sprintf(buf, "Line : %d", lineSeleted+1);
					disp = buf;
				while(!validateChoice)
				{
					
					if(selectChoice)
					{
						// ====> section critique ===> block interruptions
						lineSeleted = (lineSeleted +1) % 4;
						sprintf(buf, "Line : %d", lineSeleted+1);
					disp = buf;
						// upgrade Line on LCD
						
						
						w();
						selectChoice = false;
					}
					
				}
				
				_delay_ms(1000);
				select = selection::selectColumn;
				// Print line validated on LCD
				// disp << "COLUMN";
				// w();
				// sprintf(buf, "Column : %d", columnSeleted+1);
				// disp = buf;
				// w();
            break;

			case selection::selectColumn:
				validateChoice = false;
				sprintf(buf, "%d", columnSeleted+1);
				disp = buf;
				while(!validateChoice)
				{
					
					
					if(selectChoice)
					{
						columnSeleted = (columnSeleted +1) % 7;
						// ====> section critique ===> block interruptions
						sprintf(buf, "%d", columnSeleted+1);
						disp = buf;
						// upgrade column on LCD
						
						
						selectChoice = false;
					}
					
				}
				//validateChoice = false;
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
				
            break;
		}
	}
}

