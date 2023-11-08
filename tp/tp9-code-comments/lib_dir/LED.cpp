// Définit la fréquence de l'horloge pour les calculs de délais à 8MHz
#define F_CPU 8000000UL

// Inclut la bibliothèque pour utiliser la fonction _delay_ms
#include <util/delay.h>

// Inclut le fichier d'entête pour la classe LED
#include "LED.h"

// Définit un délai constant pour la couleur ambre à 10 millisecondes
constexpr uint8_t DELAY_AMBER_COLOR = 10;

// Déclaration du constructeur de la classe LED
LED::LED(Register port, Register mode, uint8_t greenLed, uint8_t redLed)
    : _port(port), _mode(mode), _greenLed(greenLed), _redLed(redLed) // Initialise les membres de la classe
{
    *mode |= (1 << greenLed) | (1 << redLed); // Configure les broches de la LED verte et rouge en sortie
}

// Méthode pour éteindre à la fois la LED verte et la LED rouge
void LED::turnOffLed()
{
    *_port &= ~((1 << _greenLed) | (1 << _redLed)); // Met à 0 les bits correspondants aux LEDs dans le registre de port
}

// Méthode pour allumer la LED rouge et éteindre la LED verte
void LED::turnLedRed()
{
    turnOffLed();             // Assure que les deux LEDs sont éteintes avant d'allumer la LED rouge
    *_port |= (1 << _redLed); // Met à 1 le bit correspondant à la LED rouge dans le registre de port pour l'allumer
}

// Méthode pour allumer la LED verte et éteindre la LED rouge
void LED::turnLedGreen()
{
    turnOffLed();               // Assure que les deux LEDs sont éteintes avant d'allumer la LED verte
    *_port |= (1 << _greenLed); // Met à 1 le bit correspondant à la LED verte dans le registre de port pour l'allumer
}

// Méthode pour créer une couleur ambre en allumant successivement la LED verte puis la LED rouge
void LED::turnLedAmber()
{
    turnLedGreen();               // Commence par allumer la LED verte
    _delay_ms(DELAY_AMBER_COLOR); // Attend pendant le délai défini pour la couleur ambre
    turnLedRed();                 // Puis allume la LED rouge
    _delay_ms(DELAY_AMBER_COLOR); // Attend à nouveau pendant le délai défini pour la couleur ambre
}
