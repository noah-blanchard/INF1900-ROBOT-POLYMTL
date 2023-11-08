// Inclusion du fichier d'en-tête pour la classe Navigation
#include "Navigation.h"

// Valide la valeur de vitesse pour s'assurer qu'elle est dans l'intervalle de 0 à Wheel::MAX_COMPARE_VALUE
uint16_t Navigation::_validateSpeed(uint16_t speed)
{
    if (speed > Wheel::MAX_COMPARE_VALUE) // Si la vitesse est supérieure à la valeur maximale autorisée
        speed = Wheel::MAX_COMPARE_VALUE; // Fixe la vitesse à la valeur maximale
    else if (speed < 0)                   // Si la vitesse est inférieure à zéro
        speed = 0;                        // Fixe la vitesse à zéro

    return speed; // Retourne la vitesse validée
}

// Constructeur qui crée un nouvel objet Navigation et initialise le registre DDRD
Navigation::Navigation() : _leftWheel(0), _rightWheel(1) // Initialise les roues gauche et droite
{
    DDRD = (1 << PD4) | (1 << PD5) | (1 << PD6) | (1 << PD7); // Configure les broches PD4 à PD7 comme sorties, sorties PWM pour les roues
    PORTD |= (1 << PD5);                                      // Met la broche PD5 à l'état haut, sortie OC1A, pour la roue droite
    PORTD |= (1 << PD4);                                      // Met la broche PD4 à l'état haut, sortie OC1B, pour la roue gauche
    PORTD &= ~(1 << PD6);                                     // Met la broche PD6 à l'état bas, PD6, direction de la roue gauche
    PORTD &= ~(1 << PD7);                                     // Met la broche PD7 à l'état bas, PD7, direction de la roue droite
}

// Configure la roue gauche pour avancer
void Navigation::leftForward()
{
    PORTD &= ~(1 << PD6); // Met la broche PD6 à l'état bas pour avancer
}

// Configure la roue droite pour avancer
void Navigation::rightForward()
{
    PORTD &= ~(1 << PD7); // Met la broche PD7 à l'état bas pour avancer
}

// Configure la roue gauche pour reculer
void Navigation::leftBackward()
{
    PORTD |= (1 << PD6); // Met la broche PD6 à l'état haut pour reculer
}

// Configure la roue droite pour reculer
void Navigation::rightBackward()
{
    PORTD |= (1 << PD7); // Met la broche PD7 à l'état haut pour reculer
}

// Configure les deux roues pour avancer
void Navigation::forward()
{
    this->leftForward();  // Roue gauche vers l'avant
    this->rightForward(); // Roue droite vers l'avant
}

// Configure les deux roues pour reculer
void Navigation::backward()
{
    this->leftBackward();  // Roue gauche vers l'arrière
    this->rightBackward(); // Roue droite vers l'arrière
}

// Configure les deux roues pour bouger à la vitesse donnée et dans la direction spécifiée
void Navigation::go(uint16_t speed, bool backward)
{
    speed = _validateSpeed(speed); // Valide la vitesse

    if (backward) // Si le mouvement doit être en arrière
    {
        this->backward(); // Les deux roues vers l'arrière
    }
    else // Sinon
    {
        this->forward(); // Les deux roues vers l'avant
    }

    _leftWheel.setCompareValue(speed);  // Définit la vitesse de la roue gauche
    _rightWheel.setCompareValue(speed); // Définit la vitesse de la roue droite
}

// Configure la roue gauche pour bouger à la vitesse donnée et dans la direction spécifiée
void Navigation::goLeftWheel(uint16_t speed, bool backward)
{
    speed = _validateSpeed(speed); // Valide la vitesse

    if (backward) // Si le mouvement doit être en arrière
    {
        this->leftBackward(); // Roue gauche vers l'arrière
    }
    else // Sinon
    {
        this->leftForward(); // Roue gauche vers l'avant
    }

    _leftWheel.setCompareValue(speed); // Définit la vitesse de la roue gauche
}

// Configure la roue droite pour bouger à la vitesse donnée et dans la direction spécifiée
void Navigation::goRightWheel(uint16_t speed, bool backward)
{
    speed = _validateSpeed(speed); // Valide la vitesse

    if (backward) // Si le mouvement doit être en arrière
    {
        this->rightBackward(); // Roue droite vers l'arrière
    }
    else // Sinon
    {
        this->rightForward(); // Roue droite vers l'avant
    }

    _rightWheel.setCompareValue(speed); // Définit la vitesse de la roue droite
}

// Arrête le mouvement des deux roues
void Navigation::stop()
{
    this->stopLeft();  // Arrête la roue gauche
    this->stopRight(); // Arrête la roue droite
}

// Arrête le mouvement de la roue gauche
void Navigation::stopLeft()
{
    _leftWheel.setCompareValue(0); // Met la vitesse de la roue gauche à zéro
    PORTD &= ~(1 << PD4);          // Met la broche PD4 à l'état bas
    PORTD &= ~(1 << PD6);          // Met la broche PD6 à l'état bas
}

// Arrête le mouvement de la roue droite
void Navigation::stopRight()
{
    _rightWheel.setCompareValue(0); // Met la vitesse de la roue droite à zéro
    PORTD &= ~(1 << PD5);           // Met la broche PD5 à l'état bas
    PORTD &= ~(1 << PD7);           // Met la broche PD7 à l'état bas
}
