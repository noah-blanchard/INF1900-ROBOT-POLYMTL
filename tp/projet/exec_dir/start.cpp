#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>

#include "lcm_so1602dtr_m_fw.h"
#include "customprocs.h"

#define DEMO_DDR	DDRC // `Data Direction Register' AVR occup� par l'aff.
#define DEMO_PORT	PORTC // Port AVR occup� par l'afficheur


// Plus gros d�mos :
//static void banner(LCM&, char*, uint16_t);
//static void wave(LCM&, uint16_t, uint16_t);

int main(void) {	
	// Cr�ation de l'objet (l'afficheur est connect� sur le port DEMO_PORT)
	LCM disp(&DEMO_DDR, &DEMO_PORT);
	
	disp.clear();
	disp << "start";


	_delay_ms(5000);

	disp.clear();
	

	
	return 0;
}

