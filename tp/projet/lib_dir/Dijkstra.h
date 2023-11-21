#include <avr/io.h>
#include "Types.h"

const uint8_t POIDS_MAX = 255;
const uint8_t N_NOEUDS = 28;

class Dijkstra
{
public:
    Dijkstra();
    ~Dijkstra();
    void run(uint8_t destination, Move *moveArray);

private:
    const uint8_t _ADJ_MATRIX[N_NOEUDS][N_NOEUDS];
    uint8_t _dijkstraResult[28];
    uint8_t _destination;

    void _emptyDijkstraResult();
    void _dijkstra();
}