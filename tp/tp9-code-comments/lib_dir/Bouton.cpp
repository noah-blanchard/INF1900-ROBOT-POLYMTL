// Inclusion du fichier d'en-tête pour la classe Bouton.
#include "Bouton.h"

// Constructeur de la classe Bouton qui initialise le numéro d'interruption avec la valeur passée en argument.
Bouton::Bouton(uint8_t int_N) : _int_N(int_N)
{
}

// Destructeur de la classe Bouton, qui est vide puisqu'il n'y a pas de ressources spécifiques à libérer.
Bouton::~Bouton() {}

// Méthode pour configurer l'interruption pour qu'elle soit déclenchée sur un niveau bas.
void Bouton::setLowLevel()
{
    reset(); // Appelle la méthode de réinitialisation pour effacer les réglages précédents.

    // Utilise un switch pour appliquer les réglages appropriés en fonction du numéro d'interruption.
    switch (_int_N)
    {
    case INT0: // Pour l'interruption 0, efface les bits ISC00 et ISC01.
        EICRA &= ~(1 << ISC00);
        EICRA &= ~(1 << ISC01);
        break;
    case INT1: // Pour l'interruption 1, efface les bits ISC10 et ISC11.
        EICRA &= ~(1 << ISC10);
        EICRA &= ~(1 << ISC11);
        break;
    case INT2: // Pour l'interruption 2, efface les bits ISC20 et ISC21.
        EICRA &= ~(1 << ISC20);
        EICRA &= ~(1 << ISC21);
        break;
    }
}

// Méthode pour configurer l'interruption pour qu'elle soit déclenchée sur un front montant.
void Bouton::setRisingEdge()
{
    reset(); // Réinitialise les réglages d'interruption.

    // Configure les bits de contrôle des interruptions externes pour déclencher sur un front montant.
    switch (_int_N)
    {
    case INT0: // Pour l'interruption 0, active les bits ISC00 et ISC01.
        EICRA |= (1 << ISC00);
        EICRA |= (1 << ISC01);
        break;
    case INT1: // Pour l'interruption 1, active les bits ISC10 et ISC11.
        EICRA |= (1 << ISC10);
        EICRA |= (1 << ISC11);
        break;
    case INT2: // Pour l'interruption 2, active les bits ISC20 et ISC21.
        EICRA |= (1 << ISC20);
        EICRA |= (1 << ISC21);
        break;
    }
}

// Méthode pour configurer l'interruption pour qu'elle soit déclenchée sur un front descendant.
void Bouton::setFallingEdge()
{
    reset(); // Réinitialise les réglages d'interruption.

    // Configure les bits de contrôle pour déclencher sur un front descendant.
    switch (_int_N)
    {
    case INT0: // Pour l'interruption 0, efface ISC00 et active ISC01.
        EICRA &= ~(1 << ISC00);
        EICRA |= (1 << ISC01);
        break;
    case INT1: // Pour l'interruption 1, efface ISC10 et active ISC11.
        EICRA &= ~(1 << ISC10);
        EICRA |= (1 << ISC11);
        break;
    case INT2: // Pour l'interruption 2, efface ISC20 et active ISC21.
        EICRA &= ~(1 << ISC20);
        EICRA |= (1 << ISC21);
        break;
    }
}

// Méthode pour configurer l'interruption pour qu'elle soit déclenchée sur n'importe quel front.
void Bouton::setAnyEdge()
{
    reset(); // Réinitialise les réglages d'interruption.

    // Configure les bits de contrôle pour déclencher sur n'importe quel front (montant ou descendant).
    switch (_int_N)
    {
    case INT0: // Pour l'interruption 0, active ISC00 et efface ISC01.
        EICRA |= (1 << ISC00);
        EICRA &= ~(1 << ISC01);
        break;
    case INT1: // Pour l'interruption 1, active ISC10 et efface ISC11.
        EICRA |= (1 << ISC10);
        EICRA &= ~(1 << ISC11);
        break;
    case INT2: // Pour l'interruption 2, active ISC20 et efface ISC21.
        EICRA |= (1 << ISC20);
        EICRA &= ~(1 << ISC21);
        break;
    }
}

// Active les interruptions.
void Bouton::enableInterrupt()
{
    EIMSK |= (1 << _int_N); // Active le bit correspondant au numéro d'interruption dans le registre EIMSK.
}

// Désactive les interruptions.
void Bouton::disableInterrupt()
{
    EIMSK &= ~(1 << _int_N); // Efface le bit correspondant au numéro d'interruption dans le registre EIMSK.
}

// Réinitialise le drapeau d'interruption en écrivant un 1 logique.
void Bouton::reset()
{
    // Efface les drapeaux d'interruption en écrivant un 1 logique dans le registre EIFR.
    switch (_int_N)
    {
    case INT0: // Pour l'interruption 0, efface le drapeau INTF0.
        EIFR |= (1 << INTF0);
        break;
    case INT1: // Pour l'interruption 1, efface le drapeau INTF1.
        EIFR |= (1 << INTF1);
        break;
    case INT2: // Pour l'interruption 2, efface le drapeau INTF2.
        EIFR |= (1 << INTF2);
        break;
    }
}
