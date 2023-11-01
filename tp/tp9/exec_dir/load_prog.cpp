#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
// interrupt
#include "ByteCodeInterpreter.h"

int main()
{

  ByteCodeInterpreter bct;
  Navigation nav;
  //bct.receiveAndSave();
  while (1)
  {
    bct.sendByteCode();
    // //_delay_ms(1000);
    // // nav.go(255, false);
    // nav.goLeftWheel(255, false);
    bct.run();
  }
  return 0;
}