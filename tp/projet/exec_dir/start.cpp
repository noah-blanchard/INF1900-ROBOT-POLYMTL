#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>

#include "lcm_so1602dtr_m_fw.h"
#include "customprocs.h"
#include "Bouton.h"

#define DEMO_DDR	DDRC // `Data Direction Register' AVR occup� par l'aff.
#define DEMO_PORT	PORTC // Port AVR occup� par l'afficheur

volatile uint8_t lineSeleted = 1;
volatile uint8_t columnSeleted = 1;
volatile uint8_t selectChoice = false;
volatile uint8_t validateChoice = false;

void static inline w(void) {
	cp_wait_ms(2000);
}

Bouton toValidate(INT1); // PD3
Bouton toSelect(INT0); //PD2

enum class selection {INITIAL, selectLine, selectColumn,confirmChoices};



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


void configureButtons()
{
	cli();
	toSelect.enableInterrupt();
	toValidate.enableInterrupt();
	sei();

	toSelect.setRisingEdge();
	toValidate.setRisingEdge();

}

void identifyCorner()
{
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
				while(!validateChoice)
				{
					// print choose yes
					//delai 2 sec // if validate ==> 

					// print choose no
					// delai 2 sec // if validate ==> reset line

				}
            break;
		}
	}
}
// Plus gros d�mos :
//static void banner(LCM&, char*, uint16_t);
//static void wave(LCM&, uint16_t, uint16_t);

int main(void) {	

	configureButtons();
	identifyCorner();
	// Cr�ation de l'objet (l'afficheur est connect� sur le port DEMO_PORT)
	// LCM disp(&DEMO_DDR, &DEMO_PORT);
	
	// disp.clear();
	// disp << "start";


	// _delay_ms(5000);

	// disp.clear();
	

	
	return 0;
}

