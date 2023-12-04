#pragma once
#include <avr/io.h>
#include <util/delay.h>
#include "Types.h" // import lcm
#include "lcm_so1602dtr_m_fw.h"

const uint8_t POIDS_MAX = 255;
const uint8_t N_NOEUDS = 28;
const uint8_t COL_NUMBERS = 7;

class Dijkstra
{
public:
    Dijkstra();
    ~Dijkstra();
    void run(uint8_t *start, uint8_t *destination, Move *moveArray);
    void removeNode(uint8_t x, uint8_t y);
    void resetAdjMatrix();

private:
    uint8_t _ADJ_MATRIX[N_NOEUDS][N_NOEUDS];
    uint8_t _dijkstraResult[28];
    uint8_t _destination;
    uint8_t _start;
    LCM _display;

    void _emptyDijkstraResult();
    void _dijkstra();
    uint8_t _getNodeNumber(uint8_t x, uint8_t y);
};