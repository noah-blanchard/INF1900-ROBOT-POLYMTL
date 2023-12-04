/**
 * @file Bouton.cpp
 * @brief Implementation of the Bouton class.
 *
 * This file contains the implementation of the Bouton class, which provides methods to set up and control an interrupt triggered by a button press.
 * The class provides methods to set the interrupt trigger to rising, falling or any edge, enable or disable the interrupt, and reset the interrupt.
 */
#include "Bouton.h"

Bouton::Bouton()
{

}

/**
 * @brief Constructor for the Bouton class.
 *
 * @param int_N The interrupt number associated with the button.
 */
Bouton::Bouton(uint8_t int_N) : _int_N(int_N)
{
}

Bouton::~Bouton() {}


void Bouton::setRisingEdge()
{
    reset();
    switch (_int_N)
    {
    case INT0:
        EICRA |= (1 << ISC00);
        EICRA |= (1 << ISC01);
        break;
    case INT1:
        EICRA |= (1 << ISC10);
        EICRA |= (1 << ISC11);
        break;
    case INT2:
        EICRA |= (1 << ISC20);
        EICRA |= (1 << ISC21);
        break;
    default:
        break;
    }
}

/**
 * @brief Sets the interrupt to trigger on a falling edge.
 *
 */
void Bouton::setFallingEdge()
{
    reset();

    switch (_int_N)
    {
    case INT0:
        EICRA &= ~(1 << ISC00);
        EICRA |= (1 << ISC01);
        break;
    case INT1:
        EICRA &= ~(1 << ISC10);
        EICRA |= (1 << ISC11);
        break;
    case INT2:
        EICRA &= ~(1 << ISC20);
        EICRA |= (1 << ISC21);
        break;
    default:
        break;
    }
}

/**
 * @brief Sets the interrupt to trigger on any edge.
 *
 */
void Bouton::setAnyEdge()
{
    reset();

    switch (_int_N)
    {
    case INT0:
        EICRA |= (1 << ISC00);
        EICRA &= ~(1 << ISC01);
        break;
    case INT1:
        EICRA |= (1 << ISC10);
        EICRA &= ~(1 << ISC11);
        break;
    case INT2:
        EICRA |= (1 << ISC20);
        EICRA &= ~(1 << ISC21);
        break;
    default:
        break;
    }
}

/**
 * @brief Enables the interrupt.
 *
 */
void Bouton::enableInterrupt()
{
    // check if _int_N is valid / defined before enabling interrupt

    if(_int_N == INT0 || _int_N == INT1 || _int_N == INT2)
    {
        cli();
        EIMSK |= (1 << _int_N);
        sei();
    }
}

/**
 * @brief Disables the interrupt.
 *
 */
void Bouton::disableInterrupt()
{
    if(_int_N == INT0 || _int_N == INT1 || _int_N == INT2)
    {
        cli();
        EIMSK &= ~(1 << _int_N);
        sei();
    }
}

/**
 * @brief Resets the interrupt.
 *
 */
void Bouton::reset()
{
    switch (_int_N)
    {
    case INT0:
        EIFR |= (1 << INTF0);
        break;
    case INT1:
        EIFR |= (1 << INTF1);
        break;
    case INT2:
        EIFR |= (1 << INTF2);
        break;
    }
}
