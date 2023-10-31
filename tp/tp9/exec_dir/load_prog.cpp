#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "ByteCodeInterpreter.h"

int main()
{

  // Communication com;
  LED laLed(&PORTA, &DDRA, PA0, PA1);
  const uint16_t speed = 230;

  ByteCodeInterpreter byteCodeInterpreter;

  byteCodeInterpreter.receiveByteCode();

  laLed.turnLedRed();

  return 0;
}
