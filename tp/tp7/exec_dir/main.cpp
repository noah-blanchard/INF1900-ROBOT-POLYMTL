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
    nav.go(speed, false);
    laLed.turnLedGreen();
    _delay_ms(1000);
    laLed.turnOffLed();
    nav.stop();
    _delay_ms(1000);
    laLed.turnLedRed();
    nav.go(speed, true);
    _delay_ms(1000);
    laLed.turnOffLed();
    nav.stop();
    _delay_ms(1000);
    nav.goLeftWheel(speed, false);
    laLed.turnLedGreen();
    _delay_ms(1000);
    laLed.turnOffLed();
    nav.stopLeft();
    _delay_ms(1000);
    laLed.turnLedRed();
    nav.goLeftWheel(speed, true);
    _delay_ms(1000);
    laLed.turnOffLed();
    nav.stopLeft();
    _delay_ms(1000);
    nav.goRightWheel(speed, false);
    laLed.turnLedGreen();
    _delay_ms(1000);
    laLed.turnOffLed();
    nav.stopRight();
    _delay_ms(1000);
    laLed.turnLedRed();
    nav.goRightWheel(speed, true);
    _delay_ms(1000);
    laLed.turnOffLed();
    nav.stopRight();
    _delay_ms(1000);
    nav.goRightWheel(speed, true);
    nav.goLeftWheel(speed, false);
    laLed.turnLedGreen();
    _delay_ms(5000);
  }
  return 0;
}
