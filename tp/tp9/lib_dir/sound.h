#include <avr/io.h>
#include <stdint.h>

class Sound
{
public:
    void chooseFrequency(uint8_t note);
    void stopSound();

    Sound();

    ~Sound(){};

private:
    void makeSound(uint8_t timing);
    uint16_t choosenFrequency = 0;
};
