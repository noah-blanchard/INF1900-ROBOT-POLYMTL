/**
 * @file Communication.cpp
 * @brief Implementation of the Communication class
 */

#include "Communication.h"

/**
 * @brief Construct a new Communication object
 * 
 */
Communication::Communication()
{
    UBRR0H = 0;
    
    UBRR0L = 0xCF;

    UCSR0A = 0;
    UCSR0B |= (RXEN0 << 1) | (TXEN0 << 1);
}

/**
 * @brief Destroy the Communication object
 * 
 */
Communication::~Communication()
{
}

/**
 * @brief Send a byte of data through the serial communication
 * 
 * @param data The byte of data to send
 */
void Communication::send(uint8_t data)
{
    while (!(UCSR0A & (1 << UDRE0)))
        ;
    UDR0 = data;
}

/**
 * @brief Send a string of data through the serial communication
 * 
 * @param data The string of data to send
 */
void Communication::sendString(const char *data)
{
    while (*data)
    {
        send(*data++);
    }
}

/**
 * @brief Receive a byte of data through the serial communication
 * 
 * @return uint8_t The received byte of data
 */
uint8_t Communication::receive()
{
    while (!(UCSR0A & (1 << RXC0)))
        ;
    return UDR0;
}

/**
 * @brief Reinitialize the serial communication
 * 
 */
void Communication::reinitialize() 
{
    UBRR0H = 0;
    UBRR0L = 0xCF;

    UCSR0A = 0;
    UCSR0B |= (RXEN0 << 1) | (TXEN0 << 1);
}