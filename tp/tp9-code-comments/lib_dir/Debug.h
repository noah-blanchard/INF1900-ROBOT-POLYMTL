#pragma once
#include "Communication.h"

#ifdef ACTIVE_DEBUG
#define DEBUG(X, COMM) debug_uart(X, COMM)
#else
#define DEBUG(X, COMM)
#endif

void debug_uart(const char *message, Communication comm);