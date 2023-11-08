/**
 * @file Timer.cpp
 * @brief Implementation of the Timer class.
 */

#include "Timer.h"

/**
 * @brief Finds the prescaler bits for a given prescaler value and timer number.
 *
 * @param prescaler The prescaler value to find the bits for.
 * @param timer The timer number to find the bits for.
 * @return uint8_t The prescaler bits for the given prescaler value and timer number.
 */
uint8_t Timer::findPrescalerBits(uint16_t prescaler, uint8_t timer)
{

    // Détermine le timer pour lequel configurer le présélecteur.
    // Pour chaque timer, le présélecteur correct est choisi en fonction de la valeur donnée.
    // Les bits de présélecteur sont définis en fonction de la datasheet du microcontrôleur.

    switch (timer)
    {
    case 0:
        switch (prescaler)
        {
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
        switch (prescaler)
        {
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
        switch (prescaler)
        {
        case 1:
            return (1 << CS20);
        case 8:
            return (1 << CS21);
        case 64:
            return (1 << CS21) | (1 << CS20);
        case 256:
            return (1 << CS22) | (1 << CS21);
        default:
            return 0;
        }
        break;

    default:
        return 0;
    }
}

Timer::Timer() {}

/**
 * @brief Constructor for the Timer class. Initializes the timer with the provided configuration.
 *
 * @param config TimerConfig structure containing the timer's configuration parameters.
 */
// Constructeur de la classe Timer avec un argument de configuration.
Timer::Timer(TimerConfig config) : _config(config) // Initialisation de la variable membre _config avec le paramètre config.
{
    _timerFreq = BASE_FREQ / _config.prescaler;                                          // Calcule la fréquence du timer en divisant la fréquence de base par la valeur de présélecteur.
    uint8_t prescaler_bits = Timer::findPrescalerBits(_config.prescaler, _config.timer); // Obtient les bits de présélecteur pour la configuration du timer.

    // Configure le timer basé sur le numéro de timer spécifié dans _config.
    switch (_config.timer) // Vérifie quel timer est utilisé.
    {
    case 0:                 // Si c'est le timer 0.
        if (prescaler_bits) // Si les bits de présélecteur ne sont pas 0 (valides).
        {
            TCCR0A = (1 << WGM02);                        // Configure le timer 0 pour le mode CTC (Clear Timer on Compare Match).
            TCCR0B |= prescaler_bits;                     // Configure le présélecteur pour le timer 0 en modifiant seulement les bits de présélecteur.
            TCNT0 = 0;                                    // Initialise le compteur du timer 0 à 0.
            OCR0A = _timerFreq * _config.delay_ms / 1000; // Calcule et définit la valeur de comparaison pour le timer 0.
        }
    case 1:
        if (prescaler_bits)
        {
            // La configuration est similaire au timer 0, mais avec des registres spécifiques au timer 1.

            TCCR1A |= (1 << WGM12);
            TCCR1B |= prescaler_bits; // prescaler 1024 and CTC mode
            TCNT1 = 0;
            OCR1A = _timerFreq * _config.delay_ms / 1000;
        }
        break;
    case 2:
        if (prescaler_bits)
        {
            // La configuration est similaire au timer 0, mais avec des registres spécifiques au timer 2.

            TCCR2A = (1 << WGM22);
            TCCR2B |= prescaler_bits; // prescaler 256 and CTC mode
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
// Définition de la méthode 'enable' qui active les interruptions pour le timer spécifique.
void Timer::enable()
{
    this->reset(); // Appelle la méthode 'reset' pour remettre à zéro le compteur du timer.

    // Commence une instruction switch pour traiter les différents numéros de timer possibles.
    switch (_config.timer)
    {
    case 0: // Cas où le timer à activer est le Timer 0.
        // Met à jour le registre TIMSK0 (Timer Interrupt Mask Register 0) en positionnant le bit OCIE0A (Timer/Counter0 Output Compare Match A Interrupt Enable).
        // Cela permet d'activer les interruptions lorsque Timer 0 atteint la valeur correspondant à OCR0A.
        TIMSK0 |= (1 << OCIE0A);
        break; // Interrompt l'instruction switch après avoir traité le Timer 0.
    case 1:    // Cas où le timer à activer est le Timer 1.
        // Met à jour le registre TIMSK1 (Timer Interrupt Mask Register 1) en positionnant le bit OCIE1A (Timer/Counter1 Output Compare Match A Interrupt Enable).
        // Cela permet d'activer les interruptions lorsque Timer 1 atteint la valeur correspondant à OCR1A.
        TIMSK1 |= (1 << OCIE1A);
        break; // Interrompt l'instruction switch après avoir traité le Timer 1.
    case 2:    // Cas où le timer à activer est le Timer 2.
        // Met à jour le registre TIMSK2 (Timer Interrupt Mask Register 2) en positionnant le bit OCIE2A (Timer/Counter2 Output Compare Match A Interrupt Enable).
        // Cela permet d'activer les interruptions lorsque Timer 2 atteint la valeur correspondant à OCR2A.
        TIMSK2 |= (1 << OCIE2A);
        break; // Interrompt l'instruction switch après avoir traité le Timer 2.
    default:
        // Si le numéro du timer ne correspond à aucun des cas gérés (0, 1, ou 2), aucune action n'est prise.
        break; // Interrompt l'instruction switch dans le cas d'un numéro de timer non géré.
    }
}

/**
 * @brief Disables the timer interrupts based on the timer number specified in the configuration.
 *
 * This function clears the appropriate interrupt mask register bit to disable timer interrupts.
 */
// Définition de la méthode 'disable' qui désactive les interruptions de timer.
void Timer::disable()
{
    // Utilise une instruction switch pour traiter le numéro du timer à partir de la configuration.
    switch (_config.timer)
    {
    case 0: // Si le timer à désactiver est le Timer 0.
        // Met à jour le registre TIMSK0 (Timer Interrupt Mask Register 0) pour effacer le bit OCIE0A.
        // Utilise l'opération bitwise AND avec le complément de (1 << OCIE0A) pour désactiver l'interruption.
        TIMSK0 &= ~(1 << OCIE0A);
        break; // Sort de l'instruction switch.
    case 1:    // Si le timer à désactiver est le Timer 1.
        // Agit de même pour le registre TIMSK1, effaçant le bit OCIE1A pour désactiver l'interruption pour le Timer 1.
        TIMSK1 &= ~(1 << OCIE1A);
        break; // Sort de l'instruction switch.
    case 2:    // Si le timer à désactiver est le Timer 2.
        // Agit de même pour le registre TIMSK2, effaçant le bit OCIE2A pour désactiver l'interruption pour le Timer 2.
        TIMSK2 &= ~(1 << OCIE2A);
        break; // Sort de l'instruction switch.
    default:
        // Si le numéro du timer ne correspond pas à 0, 1 ou 2, aucune action n'est prise.
        break; // Sort de l'instruction switch.
    }
}

/**
 * @brief Resets the timer counter value to 0 based on the timer number specified in the configuration.
 */
// Définition de la méthode 'reset' qui remet le compteur du timer à 0.
void Timer::reset()
{
    // Utilise une instruction switch pour traiter le numéro du timer à partir de la configuration.
    switch (_config.timer)
    {
    case 0: // Si le timer à réinitialiser est le Timer 0.
        // Affecte la valeur 0 au registre TCNT0, réinitialisant ainsi le compteur du Timer 0.
        TCNT0 = 0;
        break; // Sort de l'instruction switch.
    case 1:    // Si le timer à réinitialiser est le Timer 1.
        // Affecte la valeur 0 au registre TCNT1, réinitialisant ainsi le compteur du Timer 1.
        TCNT1 = 0;
        break; // Sort de l'instruction switch.
    case 2:    // Si le timer à réinitialiser est le Timer 2.
        // Affecte la valeur 0 au registre TCNT2, réinitialisant ainsi le compteur du Timer 2.
        TCNT2 = 0;
        break; // Sort de l'instruction switch.
    default:
        // Si le numéro du timer ne correspond pas à 0, 1 ou 2, aucune action n'est prise.
        break; // Sort de l'instruction switch.
    }
}
