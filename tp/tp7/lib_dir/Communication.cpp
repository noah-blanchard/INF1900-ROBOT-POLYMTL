#include "Communication.h"

Communication::Communication()
{
    UBRR0H = 0;
    UBRR0L = 0xCF;

    UCSR0A = 0;
    UCSR0B |= (RXEN0 << 1) | (TXEN0 << 1);
}

Communication::~Communication()
{
}

void Communication::send(uint8_t data)
{
    while (!(UCSR0A & (1 << UDRE0)))
        ;
    UDR0 = data;
}

void Communication::sendString(const char *data)
{
    while (*data)
    {
        send(*data++);
    }
}

uint8_t Communication::receive()
{
    while (!(UCSR0A & (1 << RXC0)))
        ;
    return UDR0;
}

void Communication::reinitialize() 
{
    UBRR0H = 0;
    UBRR0L = 0xCF;

    UCSR0A = 0;
    UCSR0B |= (RXEN0 << 1) | (TXEN0 << 1);
}