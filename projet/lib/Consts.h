#pragma once
#include <stdint.h>

// GOOD FOR 8.3 ~ 8.5

// WHEEL SPEED CONSTANTS
const uint8_t BASE_WHEEL_SPEED = 90;
const uint8_t BACK_WHEEL_SPEED = 85;
const uint8_t TURN_WHEEL_SPEED = 75;
const uint8_t LINE_ADJUST_OFFSET = 15;
const uint8_t OFFSET = 2;

// NAVIGATION DELAY CONSTANTS
const uint16_t BASE_ADJ_FWD_AMT = 120;
const uint16_t NAV_ADJ_FWD_AMT = 170;
const uint16_t LESS_ADJ_FWD_AMT = 100;
const uint16_t BASE_FWD_DELAY_AMT = 200;
const uint8_t ADJUST_DELAY = 10;
const uint16_t NAV_STOP_DELAY = 1000;
const uint16_t INIT_TURN_STOP_DELAY = 200;
const uint16_t INIT_TURN_DELAY = 1100;
const uint16_t MEET_POST_DELAY = 2000;
const uint8_t LOW_MIDI_NOTE = 65;

// IDENTIFY CORNER
const uint16_t CROSS_CNT_DELAY = 270;
const uint8_t HIGH_MIDI_NOTE = 81;
const uint16_t IDC_STOP_DELAY = 1000;
const uint16_t IDC_INIT_DELAY = 2000;

// OTHER
const uint8_t DEBOUNCE_DELAY = 200;
