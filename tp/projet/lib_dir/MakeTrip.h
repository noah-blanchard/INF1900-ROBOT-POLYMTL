#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>

#include "lcm_so1602dtr_m_fw.h"
#include "customprocs.h"
#include "Bouton.h"

#define DEMO_DDR	DDRC 
#define DEMO_PORT	PORTC 


void static inline w(void) {
	cp_wait_ms(2000);
}


enum class selection
{
	INITIAL, 
	selectLine, 
	selectColumn,
	confirmChoices
};




class MakeTrip()
{
public:
   
    MakeTrip(); 
private:

   void selectDestination( uint8_t _destination[2]);
};