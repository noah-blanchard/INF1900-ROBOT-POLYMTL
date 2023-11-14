#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "ByteCodeInterpreter.h"

void executeLoad()
{
  LED led(&PORTA, &DDRA, PA0, PA1);
  ByteCodeInterpreter interpreter;
  interpreter.receiveAndSave();
  while (true)
  {
    led.turnLedGreen();
  }
}

void executeRun()
{
  ByteCodeInterpreter interpreter;
  while (true)
  {
    interpreter.run();
  }
}

int main()
{
  LED led(&PORTA, &DDRA, PA0, PA1);
  ByteCodeInterpreter interpreter;
  interpreter.receiveAndSave();
  while (true)
  {
    interpreter.run();
    //nav.go(255, false);
  }
// #ifdef ACTIVE_LOAD
//   executeLoad();
// #else
//   executeRun();
// #endif

  return 0;
}
