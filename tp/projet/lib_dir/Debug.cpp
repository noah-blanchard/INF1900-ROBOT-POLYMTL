#include "Debug.h"

void debug_uart(const char *message, Communication comm)
{
    comm.sendString(message);
}