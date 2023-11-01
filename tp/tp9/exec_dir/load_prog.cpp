#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
// interrupt
#include "ByteCodeInterpreter.h"

int main()
{

  ByteCodeInterpreter bct;

  while (1)
  {
    bct.run();
  }
  return 0;
}