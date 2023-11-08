// Inclusion du fichier d'en-tête pour la classe Wheel.
#include "Wheel.h"

// Définition du constructeur pour la classe Wheel.
Wheel::Wheel(uint8_t wheelN) : _wheelNumber(wheelN) // Prend un numéro de roue et initialise la variable membre _wheelNumber avec celui-ci.
{
    switch (_wheelNumber) // Décision de l'action basée sur le numéro de roue.
    {
    case 0:               // Si le numéro de la roue est 0. Dans notre car OC1B est relié à la roue droite. PD5
        _output = &OCR1B; // Définir le pointeur _output sur l'adresse de OCR1B (registre de comparaison de sortie pour le canal B du minuteur 1).
        break;            // Sortir de l'instruction switch.
    case 1:               // Si le numéro de la roue est 1. Dans notre car OC1A est relié à la roue gauche. PD4
        _output = &OCR1A; // Définir le pointeur _output sur l'adresse de OCR1A (registre de comparaison de sortie pour le canal A du minuteur 1).
        break;            // Sortir de l'instruction switch.
    }

    // Configuration du registre de contrôle du minuteur 1 A (TCCR1A).
    TCCR1A = (1 << WGM10) | (1 << COM1A1) | (1 << COM1B1);
    // WGM10 est réglé pour le mode PWM 8 bits.
    // COM1A1 et COM1B1 sont réglés pour effacer OC1A/OC1B lors de la correspondance de comparaison (mode non-inverseur).

    // Configuration du registre de contrôle du minuteur 1 B (TCCR1B).
    TCCR1B = (1 << CS11); // Réglage du bit CS11 pour un prédiviseur de 8.

    // Configuration du registre de contrôle du minuteur 1 C (TCCR1C) à 0.
    TCCR1C = 0; // Pas de comparaison forcée de la sortie.
}

// Cette fonction définit la valeur de comparaison PWM pour la roue.
void Wheel::setCompareValue(uint16_t value)
{
    *_output = value; // Déréférence le pointeur _output pour assigner la 'valeur' au registre de comparaison de sortie correspondant (OCR1A ou OCR1B).
}
