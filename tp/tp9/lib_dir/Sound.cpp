#include "sound.h"

#define F_CPU 8000000
uint16_t ChosenFrequency = 0; 
 
Sound::Sound()
{
    DDRD |= (1 << PD1) | (1 << PD0); // car D0, D1, D5 et D4 sont deja pris
    PORTD |= (1 << PD1);
    PORTD &= ~(1 << PD0);
}

void Sound::stopSound()
{
    PORTD &= ~(1 << PD1);
    TCCR2A &= ~(1 << COM2A0);
}

void Sound::stopSound()
{
    PORTD &= ~(1 << PD1);
    TCCR2A &= ~(1 << COM2A0);
}

void Sound::makeSound(uint8_t timing)
{
    TCNT2 = 0;
    
    OCR2A = timing; // On initialise le timer 2

    TCCR2A |= 1 << COM2A0;

    TCCR2A |= 1 << WGM21;
    
    TCCR2B |= (1 << CS22) | (1 << CS21); // /256
}

void Sound::chooseFrequency(uint8_t note);
{
	switch (note)
    {
        case 45  :
            ChosenFrequency = 110; 
            break;       
        case 46 :        
            ChosenFrequency = 117;
            break;       
        case 47  :         
           ChosenFrequency = 123;
            break;       
        case 48  :         
            ChosenFrequency = 131;
            break;       
        case 49 :        
            ChosenFrequency = 139;
            break;       
        case 50  :         
            ChosenFrequency = 147;
            break;       
        case 51 :        
            ChosenFrequency = 156;
            break;      
        case 52  :         
            ChosenFrequency = 165;
            break;       
        case 53  :         
            ChosenFrequency = 175;
            break;       
        case 54 :        
            ChosenFrequency = 185;
            break;       
        case 55  :         
            ChosenFrequency = 196;
            break;       
        case 56 :        
            ChosenFrequency = 208;
            break;       
        case 57  :         
            ChosenFrequency = 220;
            break;       
        case 58 :        
            ChosenFrequency = 233;
            break;     
        case 59  :         
            ChosenFrequency = 247;
            break;       
        case 60  :         
           ChosenFrequency= 262;
            break;       
        case 61 :        
            ChosenFrequency= 277;
            break;       
        case 62  :         
            ChosenFrequency = 294;
            break;       
        case 63 :        
            ChosenFrequency = 311;
            break;       
        case 64  :         
            ChosenFrequency = 330;
            break;       
        case 65  :         
            ChosenFrequency = 349;
            break;       
        case 66 :        
            ChosenFrequency = 370;
            break;       
        case 67  :         
            ChosenFrequency = 392;
            break;       
        case 68 :        
            ChosenFrequency = 415;
            break;       
        case 69  :         
            ChosenFrequency= 440;
            break;       
        case 70 :        
            ChosenFrequency = 466;
            break;       
        case 71  :         
            ChosenFrequency = 494;
            break;       
        case 72  :         
            frequency = 523;
            break;       
        case 73 :        
            ChosenFrequency = 554;
            break;       
        case 74  :         
            ChosenFrequency = 587;
            break;       
        case 75 :        
            ChosenFrequency = 622;
            break;       
        case 76  :         
            ChosenFrequency = 659;
            break;       
        case 77  :         
            ChosenFrequency = 698;
            break;       
        case 78 :        
            ChosenFrequency = 740;
            break;       
        case 79  :         
            ChosenFrequency = 784;
            break;       
        case 80 :       
            ChosenFrequency = 831;
            break;       
        case 81  :         
            ChosenFrequency = 880;
            break;
    }
    makeSound(F_CPU / (ChosenFrequency*2));
}







