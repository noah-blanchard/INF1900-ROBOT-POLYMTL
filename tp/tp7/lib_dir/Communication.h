#include <stdint.h>

class Communication
{
public:
    Communication();
    ~Communication();
    void send(uint8_t data);
    void sendString(const char *data);
    uint8_t receive();
};