#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
// interrupt
#include <avr/interrupt.h>
#include "LED.h"

volatile bool flag = false;

ISR(TIMER1_COMPA_vect)
{
  flag = true;
}

int main()
{
  LED led(&PORTA, &DDRA, PA0, PA1);

  TimerConfig timerConfig;
  timerConfig.timer = 1;
  timerConfig.prescaler = 1024;
  timerConfig.delay_ms = 1000;
  Timer timer = Timer(timerConfig);

  timer.enable();

  while (1)
  {
    if (flag)
    {
      led.turnLedGreen();
    }
    else
    {
      led.turnLedRed();
    }
  }
  return 0;
}