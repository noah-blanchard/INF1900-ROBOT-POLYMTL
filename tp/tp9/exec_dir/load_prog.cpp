#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "ByteCodeInterpreter.h"
#include "Debug.h"

void runByteCode()
{
  ByteCodeInterpreter byteCodeInterpreter;
  Communication comm;
  while (true)
  {
    byteCodeInterpreter.run();
  }
}

void loadByteCode()
{
  ByteCodeInterpreter byteCodeInterpreter;
  LED led(&PORTA, &DDRA, PA0, PA1);
  Communication comm;
  led.turnLedRed();
  //byteCodeInterpreter.receiveAndSave();
  while(true)
  {
    led.turnLedGreen();
    comm.sendString("loadByteCode");
  }
}

int main()
{

Communication comm;
comm.sendString("loadByteCode");
#ifdef LOAD
  loadByteCode();
#else
  runByteCode();
#endif
  return 0;
}