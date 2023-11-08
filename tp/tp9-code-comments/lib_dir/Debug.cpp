#include "Debug.h"

/**
 * @brief Envoie un message de débogage sur l'UART.
 *
 * Cette fonction utilise une instance de la classe Communication pour
 * envoyer une chaîne de caractères sur l'interface série pour le débogage.
 * C'est utile pour envoyer des messages d'état ou des erreurs sur un terminal.
 *
 * @param message Le message à envoyer pour le débogage.
 * @param comm L'instance de Communication utilisée pour envoyer le message.
 */
void debug_uart(const char *message, Communication comm)
{
    comm.sendString(message); // Utilise la méthode sendString de l'instance comm pour envoyer le message.
}
