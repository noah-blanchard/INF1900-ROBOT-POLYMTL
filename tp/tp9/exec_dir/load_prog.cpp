#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "ByteCodeInterpreter.h"
#include "Debug.h"

int main()
{
  ByteCodeInterpreter interpreter;
  while (1)
  {
    interpreter.sendByteCode();
    _delay_ms(2500);
  }
  return 0;
}