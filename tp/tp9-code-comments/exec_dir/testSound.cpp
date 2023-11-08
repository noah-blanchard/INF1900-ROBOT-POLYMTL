#include <avr/io.h>
#define F_CPU 8000000
#include <util/delay.h>
#include "sound.h"


int main() {

    Sound test;
    uint8_t noteDuration = 200; // 200 ms par note
    
    for (int i = 0; i < 30 * 1000 / noteDuration; ++i) // Jouer pendant 30 secondes
    {
        // Séquence de notes de votre choix, par exemple, C, D, E, F, G, A, B
        for (int note = 60; note <= 71; ++note)
        {
            test.chooseFrequency(note);
            _delay_ms(noteDuration);
            test.stopSound();
            _delay_ms(10); // Pause entre les notes
        }
    }
}


