#pragma once
#include <stdint.h>

// GOOD FOR 8.3 ~ 8.5

// WHEEL SPEED CONSTANTS
const uint8_t BASE_WHEEL_SPEED = 90;
const uint8_t BACK_WHEEL_SPEED = 80;
const uint8_t TURN_WHEEL_SPEED = 70;
const uint8_t LINE_ADJUST_OFFSET = 15;
const uint8_t OFFSET = 2;

// NAVIGATION DELAY CONSTANTS
const uint16_t BASE_ADJ_FWD_AMT = 120;
const uint16_t NAV_ADJ_FWD_AMT = 165;
const uint16_t LESS_ADJ_FWD_AMT = 80;
const uint16_t BASE_FWD_DELAY_AMT = 200;
const uint8_t ADJUST_DELAY = 10;

// IDENTIFY CORNER
const uint16_t CROSS_CNT_DELAY = 270;
const uint16_t HIGH_MIDI_NOTE = 81;

// OTHER
const uint8_t DEBOUNCE_DELAY = 200;
