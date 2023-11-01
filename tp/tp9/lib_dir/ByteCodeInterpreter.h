#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
// interruptions
#include <avr/interrupt.h>
#include "Communication.h"
#include "memoire_24.h"
#include "LED.h"
#include "Debug.h"
#include "Timer.h"

#define F_CPU 8000000UL

class ByteCodeInterpreter
{
public:
    ByteCodeInterpreter();
    ~ByteCodeInterpreter();
    bool receiveAndSave();
    void sendByteCode();
    void run();

    // Instructions const
    // Déclaration des constantes d'instructions
    static const uint8_t DBT = 0x01;  // début
    static const uint8_t ATT = 0x02;  // attendre
    static const uint8_t DAL = 0x44;  // allumer la DEL
    static const uint8_t DET = 0x45;  // éteindre la DEL
    static const uint8_t SGO = 0x48;  // jouer une sonorité
    static const uint8_t SAR = 0x09;  // arrêter de jouer la sonorité
    static const uint8_t MAR = 0x60;  // arrêter les moteurs
    static const uint8_t MAR2 = 0x61; // arrêter les moteurs
    static const uint8_t MAV = 0x62;  // avancer
    static const uint8_t MRE = 0x63;  // reculer
    static const uint8_t TRD = 0x64;  // tourner à droite
    static const uint8_t TRG = 0x65;  // tourner à gauche
    static const uint8_t DBC = 0xC0;  // début de boucle
    static const uint8_t FBC = 0xC1;  // fin de boucle
    static const uint8_t FIN = 0xFF;  // fin

    static const uint16_t defaultDelayValue = 25; // default to 25 ms
private:
    void
    interpreteByteCode(uint8_t byteCode);
    void executeDBC(uint16_t iterationAddress, uint16_t startAdress);
    void executeATT(uint16_t delayAddress);
    void executeDAL(uint16_t colorAddress);
    void customDelay(uint16_t delay);

    Communication com;
    Memoire24CXXX memory;
    LED led;
    uint16_t currentAddress = 0x00;
    Timer timer;
};