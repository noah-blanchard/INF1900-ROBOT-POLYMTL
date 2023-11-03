#include "sound.h"

#define F_CPU 8000000
 
Sound::Sound()
{
    DDRB |= (1 << PB3) | (1 << PB5);
    PORTB |= (1 << PB3);
    PORTB &= ~(1 << PB5);
}

// Sound::Sound()
// {
//     DDRD |= (1 << PD7) | (1 << PD6);
//     PORTD |= (1 << PD7);
//     PORTD &= ~(1 << PD6);
// }

void Sound::stopSound()
{
    PORTB &= ~(1 << PB3);
    TCCR0A &= ~(1 << COM0A0);
}

// void Sound::stopSound()
// {
//     PORTD &= ~(1 << PD7);
//     TCCR2A &= ~(1 << COM2A0);
// }

// void Sound::makeSound(uint16_t frequency)
// {
//     uint32_t timing = (F_CPU / (2. * 256 * frequency)) - 1.;
//     TCNT2 = 0;
    
//     OCR2A = timing; // On initialise le timer 2

//     TCCR2A |= 1 << COM2A0;

//     TCCR2A |= 1 << WGM21;
    
//     TCCR2B |= (1 << CS22) | (1 << CS21); // /256
// }

void Sound::makeSound(uint16_t frequency)
{
    uint32_t timing = (F_CPU / (2. * 256 * frequency)) - 1.;
    TCNT0 = 0;
    
    OCR0A = timing; // On initialise le timer 2

    TCCR0A |= 1 << COM0A0;

    TCCR0A |= 1 << WGM01;
    
    TCCR0B |= (1 << CS02);
}

// void Sound::makeSound(uint16_t frequency)
// {
//     uint32_t timing = ((F_CPU / (2. * 256 * frequency)) - 1.);

//     TCNT0 = 0;
    
//     OCR0A = timing;

//     TCCR0A = (1 << WGM01) | (1 << WGM00)  | (1 << COM0A1); 
//     TCCR0B = (1 << CS02);  
// }

void Sound::chooseFrequency(uint8_t note)
{
	switch (note)
    {
        case 45  :
            choosenFrequency = 110; 
            break;       
        case 46 :        
            choosenFrequency = 117;
            break;       
        case 47  :         
           choosenFrequency = 123;
            break;       
        case 48  :         
            choosenFrequency = 131;
            break;       
        case 49 :        
            choosenFrequency = 139;
            break;       
        case 50  :         
            choosenFrequency = 147;
            break;       
        case 51 :        
            choosenFrequency = 156;
            break;      
        case 52  :         
            choosenFrequency = 165;
            break;       
        case 53  :         
            choosenFrequency = 175;
            break;       
        case 54 :        
            choosenFrequency = 185;
            break;       
        case 55  :         
            choosenFrequency = 196;
            break;       
        case 56 :        
            choosenFrequency = 208;
            break;       
        case 57  :         
            choosenFrequency = 220;
            break;       
        case 58 :        
            choosenFrequency = 233;
            break;     
        case 59  :         
            choosenFrequency = 247;
            break;       
        case 60  :         
           choosenFrequency= 262;
            break;       
        case 61 :        
            choosenFrequency= 277;
            break;       
        case 62  :         
            choosenFrequency = 294;
            break;       
        case 63 :        
            choosenFrequency = 311;
            break;       
        case 64  :         
            choosenFrequency = 330;
            break;       
        case 65  :         
            choosenFrequency = 349;
            break;       
        case 66 :        
            choosenFrequency = 370;
            break;       
        case 67  :         
            choosenFrequency = 392;
            break;       
        case 68 :        
            choosenFrequency = 415;
            break;       
        case 69  :         
            choosenFrequency= 440;
            break;       
        case 70 :        
            choosenFrequency = 466;
            break;       
        case 71  :         
            choosenFrequency = 494;
            break;       
        case 72  :         
            choosenFrequency = 523;
            break;       
        case 73 :        
            choosenFrequency = 554;
            break;       
        case 74  :         
            choosenFrequency = 587;
            break;       
        case 75 :        
            choosenFrequency = 622;
            break;       
        case 76  :         
            choosenFrequency = 659;
            break;       
        case 77  :         
            choosenFrequency = 698;
            break;       
        case 78 :        
            choosenFrequency = 740;
            break;       
        case 79  :         
            choosenFrequency = 784;
            break;       
        case 80 :       
            choosenFrequency = 831;
            break;       
        case 81  :         
            choosenFrequency = 880;
            break;
    }
    makeSound(choosenFrequency);
}
