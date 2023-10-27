#include "Bouton.h"

Bouton::Bouton(uint8_t int_N) : _int_N(int_N)
{}

Bouton::~Bouton(){ }

/**
 * @brief Sets the interrupt to trigger on a rising edge.
 * 
 */
void Bouton::setRisingEdge()
{
    reset();

    EICRA |= (1 << ISC00);

    EICRA = (1 << ISC01);
}

/**
 * @brief Sets the interrupt to trigger on a falling edge.
 * 
 */
void Bouton::setFallingEdge()
{
    reset();

    EICRA |= (0 << ISC00);

    EICRA = (1 << ISC01);

}

/**
 * @brief Sets the interrupt to trigger on any edge.
 * 
 */
void Bouton::setAnyEdge()
{
    reset();

    EICRA |= (1 << ISC00);

    EICRA = (0 << ISC01);

}

/**
 * @brief Enables the interrupt.
 * 
 */
void Bouton::enableInterrupt()
{
    EIMSK |= (1 << _int_N);
}

/**
 * @brief Disables the interrupt.
 * 
 */
void Bouton::disableInterrupt()
{
    EIMSK &= ~(1 << _int_N);
}

/**
 * @brief Resets the interrupt.
 * 
 */
void Bouton::reset(){
    EICRA &= ~(1 << ISC00);

    EICRA &= ~(1 << ISC01);
}

