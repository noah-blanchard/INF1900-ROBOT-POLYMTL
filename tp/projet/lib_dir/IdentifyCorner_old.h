#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "LineMaker.h"
#include "lcm_so1602dtr_m_fw.h"
#include "customprocs.h"
#include "Navigation.h"
#include "sound.h"

#define DEMO_DDR DDRC
#define DEMO_PORT PORTC

static const uint16_t THESPEED = 150;
static const uint16_t SLOW_TURN_SPEED = 130;

class IdentifyCorner_old
{
public:
	IdentifyCorner_old() {}

	static const uint8_t LEFT = 0b00000001;
	static const uint8_t RIGHT = 0b00000010;
	static const uint8_t BOTH = 0b00000011;

	static const uint8_t LCBV = 0b00000110;
	static const uint8_t LCBH = 0b00101101;
	static const uint8_t LCTV = 0b00001001;
	static const uint8_t LCTH = 0b00001010;
	static const uint8_t RCTH = 0b00000001;
	static const uint8_t RCTV = 0b00101110;
	static const uint8_t RCBV = 0b10011101;
	static const uint8_t RCBH = 0b00111110;

	void identificationProcess(uint8_t *_beginning);

private:
	LineMaker _lineMakerModule;
	Navigation _navModule;
	Sound _sound;

	uint8_t _stepRegistered = 0b00000000;
	uint8_t _bitshift = 0;
	bool _recognizeCorner(uint8_t _registration);
	void _printLocalization(uint8_t _step);
	void _turnRight();
	void _turnLeft();
};