#pragma once
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "Navigation.h"
#include "LineMaker.h"
#include "lcm_so1602dtr_m_fw.h"


enum class IdentifyCornerState
{
    GO_FORWARD,
    DETECT_INTERSECTION,
    DETECT_FORWARD,
    TURN,
    RECOGNIZE,
    STOP
};
class IdentifyCorner
{

public:
    IdentifyCorner();
    ~IdentifyCorner();

    void identificationProcess(uint8_t *_beginning);

private:
    Navigation _navModule;
    LineMaker _lineMakerModule;
    LCM _display;

    //sequence
    uint16_t _sequence = 0b0000000000000000;
    uint8_t _currentSequence;
    uint8_t _bitshift = 0;
    bool _found = false;

    // routines for each state
    void _goForward();
    void _detectIntersection();
    void _detectForward();
    void _turn();
    void _stop();

    void _displayCurrentSequence();

    // match the sequence
    void _matchSequence();

    IdentifyCornerState _state = IdentifyCornerState::GO_FORWARD;

    // define
    static const uint8_t LEFT = 0b00000100;
    static const uint8_t RIGHT = 0b00000001;
    static const uint8_t FORWARD = 0b00000010;

    // define sequence
    static const uint16_t LCTH = 0b0000000100001011; // left corner top horizontal
    static const uint16_t LCTV = 0b0000000000100011; // left corner top vertical
    static const uint16_t LCBH = 0b0000000000111100; // left corner bottom horizontal
    static const uint16_t LCBV = 0b0000000000110001; // left corner bottom vertical
    static const uint16_t RCTH = 0b0000000000000110; // right corner top horizontal
    static const uint16_t RCTV = 0b0000001001001011; // right corner top vertical
    static const uint16_t RCBH = 0b0000000000011011; // right corner bottom horizontal
    static const uint16_t RCBV = 0b0000000000110100; // right corner bottom vertical
};