#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "ByteCodeInterpreter.h"
#include "Debug.h"

int main()
{
  ByteCodeInterpreter interpreter;
  //LED led(&PORTA, &DDRA, PA0, PA1);
  interpreter.receiveAndSave();
  while (1)
  {
    interpreter.sendByteCode();
    _delay_ms(2500);
  }
  return 0;
}