// Inclusion du fichier d'en-tête pour la classe Sound
#include "sound.h"

// Définition de la fréquence du CPU pour le temporisateur
#define F_CPU 8000000

// Constructeur de la classe Sound
Sound::Sound()
{
    DDRB |= (1 << PB3) | (1 << PB5); // Configure les broches PB3 et PB5 comme sorties
    PORTB |= (1 << PB3);             // Met la broche PB3 à l'état haut
    PORTB &= ~(1 << PB5);            // Met la broche PB5 à l'état bas
}

// Arrête la production de son
void Sound::stopSound()
{
    PORTB &= ~(1 << PB3);     // Met la broche PB3 à l'état bas
    TCCR0A &= ~(1 << COM0A0); // Désactive la comparaison de sortie sur le Timer0
}

// Produit un son à la fréquence spécifiée
void Sound::makeSound(uint16_t frequency)
{
    uint32_t timing = (F_CPU / (2. * 256 * frequency)) - 1; // Calcule la valeur de temporisation pour la fréquence
    TCNT0 = 0;                                              // Initialise le compteur du Timer0

    OCR0A = timing; // Affecte la valeur calculée au registre de comparaison OCR0A

    TCCR0A |= 1 << COM0A0; // Active le basculement de la broche à chaque comparaison réussie

    TCCR0A |= 1 << WGM01; // Configure le Timer0 pour le mode CTC (Clear Timer on Compare Match)

    TCCR0B |= (1 << CS02); // Démarre le Timer0 avec un prédiviseur de 256
}

// Choisi la fréquence du son basée sur la note musicale donnée
void Sound::chooseFrequency(uint8_t note)
{
    switch (note)
    {
    // Correspondance entre les notes musicales et les fréquences
    case 45:
        choosenFrequency = 110;
        break;
    case 46:
        choosenFrequency = 117;
        break;
    case 47:
        choosenFrequency = 123;
        break;
    case 48:
        choosenFrequency = 131;
        break;
    case 49:
        choosenFrequency = 139;
        break;
    case 50:
        choosenFrequency = 147;
        break;
    case 51:
        choosenFrequency = 156;
        break;
    case 52:
        choosenFrequency = 165;
        break;
    case 53:
        choosenFrequency = 175;
        break;
    case 54:
        choosenFrequency = 185;
        break;
    case 55:
        choosenFrequency = 196;
        break;
    case 56:
        choosenFrequency = 208;
        break;
    case 57:
        choosenFrequency = 220;
        break;
    case 58:
        choosenFrequency = 233;
        break;
    case 59:
        choosenFrequency = 247;
        break;
    case 60:
        choosenFrequency = 262;
        break;
    case 61:
        choosenFrequency = 277;
        break;
    case 62:
        choosenFrequency = 294;
        break;
    case 63:
        choosenFrequency = 311;
        break;
    case 64:
        choosenFrequency = 330;
        break;
    case 65:
        choosenFrequency = 349;
        break;
    case 66:
        choosenFrequency = 370;
        break;
    case 67:
        choosenFrequency = 392;
        break;
    case 68:
        choosenFrequency = 415;
        break;
    case 69:
        choosenFrequency = 440;
        break;
    case 70:
        choosenFrequency = 466;
        break;
    case 71:
        choosenFrequency = 494;
        break;
    case 72:
        choosenFrequency = 523;
        break;
    case 73:
        choosenFrequency = 554;
        break;
    case 74:
        choosenFrequency = 587;
        break;
    case 75:
        choosenFrequency = 622;
        break;
    case 76:
        choosenFrequency = 659;
        break;
    case 77:
        choosenFrequency = 698;
        break;
    case 78:
        choosenFrequency = 740;
        break;
    case 79:
        choosenFrequency = 784;
        break;
    case 80:
        choosenFrequency = 831;
        break;
    case 81:
        choosenFrequency = 880;
        break;
    }
    makeSound(choosenFrequency);
}
