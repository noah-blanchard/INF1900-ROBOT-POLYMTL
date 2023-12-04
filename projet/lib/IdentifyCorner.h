#pragma once
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "Navigation.h"
#include "LineMaker.h"
#include "Sound.h"
#include "LED.h"
#include "lcm_so1602dtr_m_fw.h"
#include "Consts.h"

enum class Corner
{
    LCBH,
    LCBV,
    LCTH,
    LCTV,
    RCTH,
    RCTV,
    RCBH,
    RCBV
};

enum class IdentifyCornerState
{
    GO_FORWARD_FIRST_LINE,
    TURN_AROUND,
    GO_BACK_FIRST_LINE,
    TURN_SECOND_LINE,
    GO_FORWARD_SECOND_LINE,
    TURN_AROUND_SECOND_LINE,
    GO_BACK_SECOND_LINE,
    TURN_BACK_FIRST_LINE,
    TURN_BACK_SECOND_LINE,
    GO_INIT_POS,
    TURN_INIT_POS,
    FINISH,
};
class IdentifyCorner
{

public:
    IdentifyCorner(LED *robotLed);
    ~IdentifyCorner();

    void identificationProcess(uint8_t *_beginning);

private:
    Navigation _navModule;
    LineMaker _lineMakerModule;
    LCM _display;
    Sound _sound;
    LED *robotLed;

    uint8_t _intersectionCount = 0;
    uint8_t _firstLineCount = 0;
    uint8_t _secondLineCount = 0;

    void makeSound();

    // routines
    void _goForwardFirstLine();
    void _turnAround();
    void _goBackFirstLine();
    void _goInitPos();
    void _turnInitPos();
    void _turnSecondLine();
    void _turnBackFirstLine();
    void _turnBackSecondLine();
    void _goBackSecondLine();
    void _goForwardSecondLine();
    bool _simpleCompareMAtch();
    void _turnAroundSecondLine();
    void _displayInitPos();

    bool isRight = false;
    bool _sidefirst = false;

    Corner _initCorner;

    void _displayCurrentIntersectionCount();

    bool _found = false;
    bool _blockIncrementation;

    IdentifyCornerState _state = IdentifyCornerState::GO_FORWARD_FIRST_LINE;
};