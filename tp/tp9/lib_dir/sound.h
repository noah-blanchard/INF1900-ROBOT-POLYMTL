#include <avr/io.h>

class Sound{   
    public:
       void chooseFrequency(uint8_t note);
       void stopSound();

 Sound();

~Sound() {};

    private:
       makeSound(uint8_t timing);
};
