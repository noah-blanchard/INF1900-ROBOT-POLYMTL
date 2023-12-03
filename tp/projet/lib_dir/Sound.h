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
    void makeSound(uint16_t frequency);
    uint16_t choosenFrequency = 0;
};
