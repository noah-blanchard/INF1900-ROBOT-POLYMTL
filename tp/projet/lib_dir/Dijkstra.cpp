#include "Dijkstra.h"
#include <stdio.h>

Dijkstra::Dijkstra() : _display(&DDRC, &PORTC)
{
    resetAdjMatrix();
    // initialisation à MAX de la matrice d'adjacence
}

Dijkstra::~Dijkstra()
{
}

void Dijkstra::resetAdjMatrix()
{
    for (uint8_t i = 0; i < 28; ++i)
    {
        for (uint8_t j = 0; j < 28; ++j)
        {
            _ADJ_MATRIX[i][j] = POIDS_MAX;
        }
    }

    _ADJ_MATRIX[0][1] = 1;
    _ADJ_MATRIX[0][7] = 1;

    _ADJ_MATRIX[1][0] = 1;
    _ADJ_MATRIX[1][2] = 1;

    _ADJ_MATRIX[2][1] = 1;
    _ADJ_MATRIX[2][3] = 1;
    _ADJ_MATRIX[2][9] = 1;

    _ADJ_MATRIX[3][2] = 1;
    _ADJ_MATRIX[3][10] = 1;

    _ADJ_MATRIX[4][5] = 1;

    _ADJ_MATRIX[5][4] = 1;
    _ADJ_MATRIX[5][6] = 2;
    _ADJ_MATRIX[5][12] = 2;

    _ADJ_MATRIX[6][5] = 2;
    _ADJ_MATRIX[6][13] = 1;

    _ADJ_MATRIX[7][0] = 1;
    _ADJ_MATRIX[7][8] = 5;

    _ADJ_MATRIX[8][7] = 5;
    _ADJ_MATRIX[8][9] = 1;
    _ADJ_MATRIX[8][15] = 1;

    _ADJ_MATRIX[9][2] = 1;
    _ADJ_MATRIX[9][8] = 1;
    _ADJ_MATRIX[9][16] = 5;

    _ADJ_MATRIX[10][3] = 1;
    _ADJ_MATRIX[10][11] = 2;

    _ADJ_MATRIX[11][10] = 2;
    _ADJ_MATRIX[11][12] = 1;
    _ADJ_MATRIX[11][18] = 5;

    _ADJ_MATRIX[12][5] = 2;
    _ADJ_MATRIX[12][11] = 1;
    _ADJ_MATRIX[12][13] = 1;
    _ADJ_MATRIX[12][19] = 1;

    _ADJ_MATRIX[13][6] = 1;
    _ADJ_MATRIX[13][12] = 1;
    _ADJ_MATRIX[13][20] = 1;

    _ADJ_MATRIX[14][15] = 2;
    _ADJ_MATRIX[14][21] = 5;

    _ADJ_MATRIX[15][8] = 1;
    _ADJ_MATRIX[15][14] = 2;
    _ADJ_MATRIX[15][16] = 1;

    _ADJ_MATRIX[16][9] = 5;
    _ADJ_MATRIX[16][15] = 1;
    _ADJ_MATRIX[16][17] = 1;
    _ADJ_MATRIX[16][23] = 1;

    _ADJ_MATRIX[17][16] = 1;
    _ADJ_MATRIX[17][18] = 5;
    _ADJ_MATRIX[17][24] = 1;

    _ADJ_MATRIX[18][11] = 5;
    _ADJ_MATRIX[18][17] = 5;
    //_ADJ_MATRIX[18][19] = 1;
    _ADJ_MATRIX[18][25] = 5;

    _ADJ_MATRIX[19][12] = 1;
    //_ADJ_MATRIX[19][18] = 1;
    //_ADJ_MATRIX[19][20] = 1;
    _ADJ_MATRIX[19][26] = 1;

    _ADJ_MATRIX[20][13] = 1;
    //_ADJ_MATRIX[20][19] = 1;
    _ADJ_MATRIX[20][27] = 1;

    _ADJ_MATRIX[21][14] = 5;
    _ADJ_MATRIX[21][22] = 1;

    _ADJ_MATRIX[22][21] = 1;
    _ADJ_MATRIX[22][23] = 1;

    _ADJ_MATRIX[23][16] = 1;
    _ADJ_MATRIX[23][22] = 1;

    _ADJ_MATRIX[24][17] = 1;
    _ADJ_MATRIX[24][25] = 2;

    _ADJ_MATRIX[25][18] = 5;
    _ADJ_MATRIX[25][24] = 2;
    _ADJ_MATRIX[25][26] = 5;

    _ADJ_MATRIX[26][19] = 1;
    _ADJ_MATRIX[26][25] = 5;
    _ADJ_MATRIX[26][27] = 1;

    _ADJ_MATRIX[27][20] = 1;
    _ADJ_MATRIX[27][26] = 1;
}
// this function will remove a possible position (the one passed as a parameter)
// so it will set the value of any connection to this position to 255
void Dijkstra::removeNode(uint8_t x, uint8_t y)
{
    uint8_t noeud = _getNodeNumber(x, y);
    for (uint8_t i = 0; i < N_NOEUDS; i++)
    {
        _ADJ_MATRIX[noeud][i] = 255;
        _ADJ_MATRIX[i][noeud] = 255;
    }
}

uint8_t Dijkstra::_getNodeNumber(uint8_t x, uint8_t y)
{
    return y * COL_NUMBERS + x;
}

void Dijkstra::run(uint8_t *start, uint8_t *destination, Move *moveArray)
{
    _destination = _getNodeNumber(destination[0], destination[1]);
    _start = _getNodeNumber(start[0], start[1]);

    char buffer[20];
    // print destination for 2 SEC then START for 2 SEC
    // print start for 2 SEC then destination for 2 SEC
    sprintf(buffer, "Destination: %d", _destination);
    _display = buffer;
    _delay_ms(2000);

    sprintf(buffer, "Start: %d", _start);
    _display = buffer;
    _delay_ms(2000);

    _emptyDijkstraResult();
    _dijkstra();

    uint8_t lastX = start[0];
    uint8_t lastY = start[1];

    uint8_t indexR = 0;
    uint8_t indexM = 0;
    while (_dijkstraResult[indexR] != 255)
    {
        uint8_t result = _dijkstraResult[indexR];
        uint8_t nextX = _dijkstraResult[indexR] % 7;
        uint8_t nextY = _dijkstraResult[indexR] / 7;
        Orientation nextOrientation = Orientation::NORTH;
        indexR++;
        if (lastX == nextX && lastY == nextY)
        {

            continue;
        }
        else if (lastX < nextX)
        {
            nextOrientation = Orientation::EAST;
        }
        else if (lastX > nextX)
        {
            nextOrientation = Orientation::WEST;
        }
        else if (lastY < nextY)
        {
            nextOrientation = Orientation::SOUTH;
        }
        else if (lastY > nextY)
        {
            nextOrientation = Orientation::NORTH;
        }

        lastX = nextX;
        lastY = nextY;

        moveArray[indexM].orientation = nextOrientation;
        moveArray[indexM].x = nextX;
        moveArray[indexM].y = nextY;

        indexM++;
    }

    // put ORIENTATION FINISHED at the end of the array and last x last y
    moveArray[indexM].orientation = Orientation::FINISHED;
    moveArray[indexM].x = lastX;
    moveArray[indexM].y = lastY;
}

void Dijkstra::_emptyDijkstraResult()
{
    for (uint8_t i = 0; i < 28; ++i)
    {
        _dijkstraResult[i] = 255;
    }
}

// implement _dijstra for the _ADJ_MATRIX
// and fill _dijkstraResult
// the start node should be _start
// the destination node should be _destinationvoid
void Dijkstra::_dijkstra()
{
    uint8_t dist[N_NOEUDS];
    bool visited[N_NOEUDS];
    uint8_t predecessor[N_NOEUDS];

    // Initialize distances, visited status, and predecessors
    for (uint8_t i = 0; i < N_NOEUDS; i++)
    {
        dist[i] = POIDS_MAX;
        visited[i] = false;
        predecessor[i] = POIDS_MAX; // Use POIDS_MAX as a marker for no predecessor
    }

    // Distance to the start node is zero
    dist[_start] = 0;

    for (uint8_t count = 0; count < N_NOEUDS - 1; count++)
    {
        // Find the minimum distance node from the set of unvisited nodes
        uint8_t min = POIDS_MAX, minNode = 0;
        for (uint8_t v = 0; v < N_NOEUDS; v++)
        {
            if (!visited[v] && dist[v] <= min)
            {
                min = dist[v];
                minNode = v;
            }
        }

        // Mark the picked node as processed
        visited[minNode] = true;

        // Update dist value and predecessor of the adjacent nodes
        for (uint8_t v = 0; v < N_NOEUDS; v++)
        {
            if (!visited[v] && _ADJ_MATRIX[minNode][v] && dist[minNode] != POIDS_MAX && dist[minNode] + _ADJ_MATRIX[minNode][v] < dist[v])
            {
                dist[v] = dist[minNode] + _ADJ_MATRIX[minNode][v];
                predecessor[v] = minNode;
            }
        }
    }

    // Path reconstruction
    uint8_t current = _destination;
    uint8_t pathIndex = 0;
    while (current != _start)
    {
        if (current == POIDS_MAX)
        {
            // No path found
            _dijkstraResult[0] = 255; // Indicating no path
            return;
        }
        _dijkstraResult[pathIndex++] = current;
        current = predecessor[current];
    }
    _dijkstraResult[pathIndex++] = _start; // Adding start node at the end

    // Reversing the path to get it from start to destination
    for (uint8_t i = 0; i < pathIndex / 2; i++)
    {
        uint8_t temp = _dijkstraResult[i];
        _dijkstraResult[i] = _dijkstraResult[pathIndex - 1 - i];
        _dijkstraResult[pathIndex - 1 - i] = temp;
    }

    // Mark the end of the result path
    _dijkstraResult[pathIndex] = 255;
}
