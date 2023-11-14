#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 8000000
#include <util/delay.h>
#include "Bouton.h"
#include "LED.h"
volatile uint8_t selectChoice = true;
volatile uint8_t validateChoice = false;



ISR ( INT0_vect) {
// validate Choice
selectChoice = !selectChoice;
EIFR |= (1 << INTF0) ;

}


ISR ( INT1_vect) {
// validate Choice
selectChoice = !selectChoice;
EIFR |= (1 << INTF1) ;

}

int main()
{
// DDRA = (1<< DDA0 | 1<< DDA1);
//     DDRD = (0<< DDA0 | 0<< DDA1);
//     PORTA |= (1 << PA1);


LED led(&PORTB, &DDRB, PB0, PB1);
//     DDRD = 0xff;
    Bouton gauche(INT0);
    Bouton droite(INT1);

    gauche.setAnyEdge();
    droite.setAnyEdge();
    cli();
    gauche.enableInterrupt();
    droite.enableInterrupt();
    sei();

    
    while(true)
    {
        if(selectChoice)
        {
            led.turnLedGreen();
        }else{
            led.turnLedRed();
        }


    }
   
    
    
}


