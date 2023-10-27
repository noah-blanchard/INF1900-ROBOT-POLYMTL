#include "Timer.h"


/**
 * @brief Finds the prescaler bits for a given prescaler value and timer number.
 * 
 * @param prescaler The prescaler value to find the bits for.
 * @param timer The timer number to find the bits for.
 * @return uint8_t The prescaler bits for the given prescaler value and timer number.
 */
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

/**
 * @brief Constructor for the Timer class. Initializes the timer with the provided configuration.
 * 
 * @param config TimerConfig structure containing the timer's configuration parameters.
 */
Timer::Timer(TimerConfig config) : _config(config)
{

    TCCR1A = 0;
    TCCR1B = 0;
    _timerFreq = BASE_FREQ / _config.prescaler;

    uint8_t prescaler_bits = Timer::findPrescalerBits(_config.prescaler, _config.timer);

    switch (_config.timer)
    {
    case 0:
        if (prescaler_bits)
        {
            TCCR0A = 0;
            TCCR0B |= (1 << WGM12) | prescaler_bits; // prescaler 256 and CTC mode
            TCNT0 = 0;
            OCR0A = _timerFreq * _config.delay_ms / 1000;
        }
        break;
    case 1:
        if (prescaler_bits)
        {
            TCCR1A |= 0;
            TCCR1B |= (1 << WGM12) | prescaler_bits; // prescaler 1024 and CTC mode
            TCNT1 = 0;
            OCR1A = _timerFreq * _config.delay_ms / 1000;
        }
        break;
    case 2:
        if (prescaler_bits)
        {
            TCCR2A = 0;
            TCCR2B |= (1 << WGM12) | prescaler_bits; // prescaler 256 and CTC mode
            TCNT2 = 0;
            OCR2A = _timerFreq * _config.delay_ms / 1000;
        }
        break;
    }
}

/**
 * @brief Destructor for the Timer class. Handles any necessary cleanup.
 */
Timer::~Timer()
{
}

/**
 * @brief Enables the timer interrupts based on the timer number specified in the configuration.
 * 
 * This function sets the appropriate interrupt mask register bit to enable timer interrupts.
 */
void Timer::enable()
{
    this->reset();
    switch (_config.timer)
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
        break;
    }
}

/**
 * @brief Disables the timer interrupts based on the timer number specified in the configuration.
 * 
 * This function clears the appropriate interrupt mask register bit to disable timer interrupts.
 */
void Timer::disable()
{
    switch (_config.timer)
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
        break;
    }
}

/**
 * @brief Resets the timer counter value to 0 based on the timer number specified in the configuration.
 */
void Timer::reset()
{
    switch (_config.timer)
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
        break;
    }
}