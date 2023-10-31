#pragma once

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "ByteCodeInterpreter.h"
#include "LED.h"
#include "Debug.h"

int main()
{

  // Communication com;
  LED laLed(&PORTA, &DDRA, PA0, PA1);

  ByteCodeInterpreter byteCodeInterpreter;

  byteCodeInterpreter.receiveByteCode();

  Communication comm;

  while(true){
  laLed.turnLedRed();
  DEBUG("HELLO", comm);
  }

  return 0;
}
