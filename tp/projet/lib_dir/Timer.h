#pragma once
#include <stdint.h>
#include <avr/io.h>

struct TimerConfig
{
    uint8_t timer;
    uint16_t prescaler;
    float delay_ms;
};

struct PrescalerMapping {
    uint16_t key;
    uint8_t value;
};

const PrescalerMapping TIMER0_2_PRESCALERS[] = {
    {1, (1 << CS00)},
    {8, (1 << CS01)},
    {64, (1 << CS01) | (1 << CS00)},
    {256, (1 << CS02)}
};

const PrescalerMapping TIMER1_PRESCALERS[] = {
    {1, (1 << CS10)},
    {8, (1 << CS11)},
    {64, (1 << CS11) | (1 << CS10)},
    {256, (1 << CS12)},
    {1024, (1 << CS12) | (1 << CS10)}
};

class Timer
{
public:
    Timer();
    Timer(TimerConfig config);
    ~Timer();
    void enable();
    void disable();
    void reset();

    static uint8_t findPrescalerBits(uint16_t key, uint8_t timer);

    static const uint32_t BASE_FREQ = 8000000UL;
    static const uint32_t PRESCALER_8BITS = 256;
    static const uint32_t PRESCALER_16BITS = 1024;

private:
    TimerConfig _config;
    uint16_t _timerFreq;
};