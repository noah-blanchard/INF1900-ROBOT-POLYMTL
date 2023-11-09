// Inclusion du fichier d'en-tête pour la classe Bouton.
#include "Bouton.h"

// Constructeur de la classe Bouton.
Bouton::Bouton(uint8_t int_N) : _int_N(int_N) // Initialise le numéro d'interruption avec la valeur passée.
{
}

// Destructeur de la classe Bouton, actuellement vide car il n'y a rien à nettoyer explicitement.
Bouton::~Bouton() {}

// Configure l'interruption pour qu'elle soit déclenchée sur un front montant.
void Bouton::setRisingEdge()
{
    reset(); // Réinitialise la configuration des interruptions.

    EICRA |= (1 << ISC00); // Configure le premier bit pour la détection de front montant.

    EICRA = (1 << ISC01); // Ceci est probablement une erreur. Il devrait utiliser '|=', sinon cela écrase la configuration précédente.
}

// Configure l'interruption pour qu'elle soit déclenchée sur un front descendant.
void Bouton::setFallingEdge()
{
    reset(); // Réinitialise la configuration des interruptions.

    EICRA |= (0 << ISC00); // Efface le premier bit pour la détection de front descendant (inutile car écrire 0 n'a aucun effet).

    EICRA = (1 << ISC01); // Ceci est probablement une erreur. Il devrait utiliser '|=', sinon cela écrase la configuration précédente.
}

// Configure l'interruption pour qu'elle soit déclenchée sur les deux fronts.
void Bouton::setAnyEdge()
{
    reset(); // Réinitialise la configuration des interruptions.

    EICRA |= (1 << ISC00); // Configure le premier bit pour la détection de n'importe quel front.

    EICRA = (0 << ISC01); // Ceci est probablement une erreur. Il devrait utiliser '|=', sinon cela écrase la configuration précédente.
}

// Active les interruptions.
void Bouton::enableInterrupt()
{
    EIMSK |= (1 << _int_N); // Active l'interruption en configurant le bit correspondant dans le registre EIMSK.
}

// Désactive les interruptions.
void Bouton::disableInterrupt()
{
    EIMSK &= ~(1 << _int_N); // Désactive l'interruption en nettoyant le bit correspondant dans le registre EIMSK.
}

// Réinitialise la configuration des interruptions.
void Bouton::reset()
{
    EICRA &= ~(1 << ISC00); // Nettoie le bit pour la détection de front montant.

    EICRA &= ~(1 << ISC01); // Nettoie le bit pour la détection de front descendant.
}
