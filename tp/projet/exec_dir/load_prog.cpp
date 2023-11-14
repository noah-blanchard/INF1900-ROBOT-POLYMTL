#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
// interrupt
#include "ByteCodeInterpreter.h"

int main()
{

  ByteCodeInterpreter bct;
  //Navigation nav;
  bct.receiveAndSave();
  // DDRB |= (1 << PB3) | (1 << PB5); // car D0, D1, D5 et D4 sont deja pris
  //   PORTB |= (1 << PB3);
  //  Sound test;
    //uint8_t noteDuration = 200; 
    while(1){
      bct.run();
    }
  return 0;
}