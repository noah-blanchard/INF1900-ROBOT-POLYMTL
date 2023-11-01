#pragma once

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "ByteCodeInterpreter.h"

void runByteCode()
{
  ByteCodeInterpreter byteCodeInterpreter;
  while (true)
  {
    byteCodeInterpreter.run();
  }
}

int main()
{

#ifdef LOAD
  byteCodeInterpreter.receiveAndSave();
  return 0;
#else
    runByteCode();
  return 0;
}