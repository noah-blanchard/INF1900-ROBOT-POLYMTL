#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "Navigation.h"

int main()
{

  // Communication com;
  Navigation nav;
  while (true)
  {
    nav.forward();
    nav.go(255);
    _delay_ms(1000);
    nav.stop();
    _delay_ms(1000);
    nav.backward();
    nav.go(255);
    _delay_ms(1000);
    nav.stop();
    _delay_ms(1000);
    nav.leftBackward();
    nav.go(255);
    _delay_ms(5000);
  }
  return 0;
}
