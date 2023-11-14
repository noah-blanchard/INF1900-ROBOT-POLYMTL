#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>

#include "lcm_so1602dtr_m_fw.h"
#include "customprocs.h"
#include "Bouton.h"

#define DEMO_DDR	DDRC // `Data Direction Register' AVR occup� par l'aff.
#define DEMO_PORT	PORTC // Port AVR occup� par l'afficheur

volatile uint8_t lineSeleted = 1;
volatile uint8_t columnSeleted = 1;
volatile uint8_t selectChoice = false;
volatile uint8_t validateChoice = false;

Bouton toValidate(1); // PD3
Bouton toSelect(0); //PD2

enum class selection {INITIAL, selectLine, selectColumn,confirmChoices};

ISR ( INT0_vect) {
// select Choice
selectChoice = true;
EIFR |= (1 << INTF0) ;

}

ISR ( INT1_vect) {
// validate Choice
validateChoice = true;
EIFR |= (1 << INTF0) ;

}



void configureButtons()
{
	cli();
	toSelect.enableInterrupt();
	toSelect.setLowLevel();

	toValidate.enableInterrupt();
	toValidate.setLowLevel();

	sei();

}

void identifyCorner()
{
	selection select = selection::INITIAL;
	while(true)
	{
		switch(select)
		{
			case selection::INITIAL:
				selectLine = 1;
				// print line on LCD
				select = selection::selectLine;
            break;


			case selection::selectLine:
				while(!validateChoice)
				{
					if(selectChoice)
					{
						// ====> section critique ===> block interruptions
						lineSeleted += (1 % 4);
						// upgrade Line on LCD
					}
					selectChoice = false;
				}
				validateChoice = false;
				select = selection::selectColumn;
				// Print line validated on LCD
            break;

			case selection::selectColumn:
				columnSeleted = 1;
				while(!validateChoice)
				{
					if(selectChoice)
					{
						// ====> section critique ===> block interruptions
						columnSeleted += (1 % 7);
						// upgrade Line on LCD
					}
					selectChoice = false;
				}
				validateChoice = false;
				select = selection::confirmChoices;
				// Print choices on LCD

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

