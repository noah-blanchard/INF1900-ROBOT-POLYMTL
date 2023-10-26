
#include "Timer.h"

uint8_t Timer::findPrescalerBits(uint16_t prescaler, uint8_t timer){
    switch (timer) {
        case 0:
            switch (prescaler) {
                case 1:
                    return (1 << CS00);
                case 8:
                    return (1 << CS01);
                case 64:
                    return (1 << CS01) | (1 << CS00);
                case 256:
                    return (1 << CS02);
                default:
                    return 0;
            }
            break;

        case 1:
            switch (prescaler) {
                case 1:
                    return (1 << CS10);
                case 8:
                    return (1 << CS11);
                case 64:
                    return (1 << CS11) | (1 << CS10);
                case 256:
                    return (1 << CS12);
                case 1024:
                    return (1 << CS12) | (1 << CS10);
                default:
                    return 0;
            }
            break;

        case 2:
            switch (prescaler) {
                case 1:
                    return (1 << CS20);
                case 8:
                    return (1 << CS21);
                case 64:
                    return (1 << CS21) | (1 << CS20);
                case 256:
                    return (1 << CS22);
                default:
                    return 0;
            }
            break;

        default:
            return 0;
    }
}

Timer::Timer(TimerConfig config_p) : config(config_p)
{

    TCCR1A = 0;
    TCCR1B = 0;
    timer_freq = BASE_FREQ / config.prescaler;

    uint8_t prescaler_bits = Timer::findPrescalerBits(config.prescaler, config.timer);

    switch (config.timer)
    {
    case 0:
        if (prescaler_bits)
        {
            TCCR0A = 0;
            TCCR0B |= (1 << WGM12) | prescaler_bits; // prescaler 256 and CTC mode
            TCNT0 = 0;
            OCR0A = timer_freq * config.delay_ms / 1000;
        }
        break;
    case 1:
        if (prescaler_bits)
        {
            TCCR1A |= 0;
            TCCR1B |= (1 << WGM12) | prescaler_bits; // prescaler 1024 and CTC mode
            TCNT1 = 0;
            OCR1A = timer_freq * config.delay_ms / 1000;
        }
        break;
    case 2:
        if (prescaler_bits)
        {
            TCCR2A = 0;
            TCCR2B |= (1 << WGM12) | prescaler_bits; // prescaler 256 and CTC mode
            TCNT2 = 0;
            OCR2A = timer_freq * config.delay_ms / 1000;
        }
        break;
    }
}

Timer::~Timer()
{
}

void Timer::enable()
{
    // Start timer
    this->reset();
    switch (config.timer)
    {
    case 0:
        TIMSK0 |= (1 << OCIE0A);
        break;
    case 1:
        TIMSK1 |= (1 << OCIE1A);
        break;
    case 2:
        TIMSK2 |= (1 << OCIE2A);
        break;
    default:
        // Throw error
        break;
    }
}

void Timer::disable()
{
    // Stop timer
    switch (config.timer)
    {
    case 0:
        TIMSK0 &= ~(1 << OCIE0A);
        break;
    case 1:
        TIMSK1 &= ~(1 << OCIE1A);
        break;
    case 2:
        TIMSK2 &= ~(1 << OCIE2A);
        break;
    default:
        // Throw error
        break;
    }
}

void Timer::reset()
{
    // Reset timer
    switch (config.timer)
    {
    case 0:
        TCNT0 = 0;
        break;
    case 1:
        TCNT1 = 0;
        break;
    case 2:
        TCNT2 = 0;
        break;
    default:
        // Throw error
        break;
    }
}