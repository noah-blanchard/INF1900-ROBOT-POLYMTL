#include "Communication.h"

#ifdef ACTIVE_DEBUG
#define DEBUG(X) debug_uart(X)
#else
#define DEBUG(X)
#endif

// proto de debug_uart à implementer dans Debug.cpp
void debug_uart(const char *message);