#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "LED.h"
#include "Navigation.h"

int main()
{

  // Communication com;
  Navigation nav;
  LED laLed(&PORTA, &DDRA, PA0, PA1);
  const uint16_t speed = 230;
  while (true)
  {
    nav.forward();
    nav.go(speed);
    laLed.turnLedRed();
    _delay_ms(1000);
    nav.stop();
    _delay_ms(1000);
    laLed.turnLedGreen();
    nav.backward();
    nav.go(speed);
    _delay_ms(1000);
    nav.stop();
    _delay_ms(1000);
    laLed.turnLedRed();
    nav.leftForward();
    nav.go(speed);
    _delay_ms(1000);
    nav.stop();
    _delay_ms(1000);
    laLed.turnLedGreen();
    nav.stopLeft();
    nav.rightForward();
    nav.go(speed);
    _delay_ms(1000);
    nav.stop();
    _delay_ms(1000);
     laLed.turnLedRed();
    nav.rightForward();
    nav.leftBackward();
    nav.go(speed);
    _delay_ms(5000);

  }
  return 0;
}
