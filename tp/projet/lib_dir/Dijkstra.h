#pragma once
#include <avr/io.h>
#include "Types.h"

const uint8_t POIDS_MAX = 255;
const uint8_t N_NOEUDS = 28;

class Dijkstra
{
public:
    Dijkstra();
    ~Dijkstra();
    void run(uint8_t *start, uint8_t *destination, Move *moveArray);
    void removeNode(uint8_t x, uint8_t y);

private:
    uint8_t _ADJ_MATRIX[N_NOEUDS][N_NOEUDS];
    uint8_t _dijkstraResult[28];
    uint8_t _destination;
    uint8_t _start;

    void _emptyDijkstraResult();
    void _resetAdjMatrix();
    void _dijkstra();
    uint8_t _getNodeNumber(uint8_t x, uint8_t y);
};