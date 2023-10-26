#include "Bouton.h"

/*
volatile uint8_t btnState = false;
    ISR(INT0_vect)
{ // action quand l'event d'interruption int1 a lieu

    _delay_ms(30);

    btnState = true;

    EIFR |= (1 << INTF1); // oublier toutes les requetes en cours
}
*/

Bouton::Bouton(uint8_t int_N_p) : int_N(int_N_p)
{}

Bouton::~Bouton(){ }

void Bouton::setRisingEdge()
{
    reset();

    EICRA |= (1 << ISC00);

    EICRA = (1 << ISC01);
}
void Bouton::setFallingEdge()
{
    reset();

    EICRA |= (0 << ISC00);

    EICRA = (1 << ISC01);

}
void Bouton::setAnyEdge()
{
    reset();

    EICRA |= (1 << ISC00);

    EICRA = (0 << ISC01);

}

void Bouton::enableInterrupt()
{
    EIMSK |= (1 << int_N); // on active l'interruption sur int_N
}

void Bouton::disableInterrupt()
{
    EIMSK &= ~(1 << int_N); // on desactive l'interruption sur int_
}

void Bouton::reset(){
    EICRA &= ~(1 << ISC00);

    EICRA &= ~(1 << ISC01);
}

