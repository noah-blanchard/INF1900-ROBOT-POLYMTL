#pragma once
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
class Bouton
{
public:
// default constructor
  Bouton();
  Bouton(uint8_t int_N_p);
  ~Bouton();
  
  void setRisingEdge();
  void setFallingEdge();
  void setAnyEdge();
  void enableInterrupt();
  void disableInterrupt();
  void reset();

private:
  uint8_t _int_N;
};