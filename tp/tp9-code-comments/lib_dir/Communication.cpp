/**
 * @file Communication.cpp
 * @brief Implementation of the Communication class
 */

#include "Communication.h"

/**
 * @brief Constructeur de la classe Communication.
 * Initialise les registres pour configurer la communication série.
 */
Communication::Communication()
{
    UBRR0H = 0;    // Mise à zéro de la partie haute du registre UBRR (Baud Rate Register)
    UBRR0L = 0xCF; // Configuration de la partie basse du registre UBRR pour un baud rate spécifique
    // on met la baudrate à 2400 baud !

    UCSR0A = 0;                            // Configuration du registre de contrôle A pour le USART
    UCSR0B |= (1 << RXEN0) | (1 << TXEN0); // Activation de la réception (RX) et de l'émission (TX)
}

/**
 * @brief Destructeur de la classe Communication.
 * Actuellement vide car il n'y a pas de nettoyage spécifique nécessaire à la destruction de l'objet.
 */
Communication::~Communication()
{
    // Aucune action à effectuer dans le destructeur pour le moment.
}

/**
 * @brief Envoyer un octet de données via la communication série.
 *
 * @param data L'octet de données à envoyer.
 */
void Communication::send(uint8_t data)
{
    while (!(UCSR0A & (1 << UDRE0))) // Attend que le registre de données soit vide.
        ;
    UDR0 = data; // Place l'octet de données dans le registre UDR0 pour l'envoyer.
}

/**
 * @brief Envoyer une chaîne de caractères via la communication série.
 *
 * @param data Pointeur vers la chaîne de caractères à envoyer.
 */
void Communication::sendString(const char *data)
{
    while (*data) // Tant que le pointeur ne pointe pas vers la fin de la chaîne
    {
        send(*data++); // Envoyer le caractère courant et incrémente le pointeur
    }
}

/**
 * @brief Recevoir un octet de données via la communication série.
 *
 * @return uint8_t L'octet de données reçu.
 */
uint8_t Communication::receive()
{
    while (!(UCSR0A & (1 << RXC0))) // Attend que des données soient disponibles à la lecture.
        ;
    return UDR0; // Retourne l'octet de données reçu situé dans le registre UDR0.
}

/**
 * @brief Réinitialiser la communication série.
 * Réinitialise les registres à leurs valeurs par défaut pour la communication série.
 */
void Communication::reinitialize()
{
    UBRR0H = 0;    // Réinitialisation de la partie haute du registre UBRR
    UBRR0L = 0xCF; // Réinitialisation de la partie basse du registre UBRR pour un baud rate spécifique

    UCSR0A = 0;                            // Réinitialisation du registre de contrôle A pour le USART
    UCSR0B |= (1 << RXEN0) | (1 << TXEN0); // Réactivation de la réception et de l'émission
}
