#include "Dijkstra.h"
#include <stdio.h>

Dijkstra::Dijkstra() : _display(&DDRC, &PORTC) : _start(0), _destination(0)
{
    _resetAdjMatrix();
    // initialisation à MAX de la matrice d'adjacence
}

Dijkstra::~Dijkstra()
{
}

void Dijkstra::_resetAdjMatrix()
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

void Dijkstra::_dijkstra()
{
    uint8_t poids[N_NOEUDS];
    uint8_t noeudPrecedent[N_NOEUDS];
    bool noeudVisite[N_NOEUDS];

    for (uint8_t i = 0; i < N_NOEUDS; ++i)
    {
        poids[i] = POIDS_MAX;
        noeudPrecedent[i] = 255; // Utiliser une valeur qui signifie "aucun prédecesseur"
        noeudVisite[i] = false;
    }

    // poids[0] = 0; // Partir du noeud 0
    // partir du noeud _start
    poids[_start] = 0;
    // noeudPrecedent[0] = 0;
    noeudPrecedent[_start] = 0;

    for (uint8_t i = 0; i < N_NOEUDS - 1; i++)
    {
        uint8_t minPoids = POIDS_MAX;
        uint8_t minIndex;

        for (uint8_t j = 0; j < N_NOEUDS; j++)
        {
            if (!noeudVisite[j] && poids[j] <= minPoids)
            {
                minPoids = poids[j];
                minIndex = j;
            }
        }

        noeudVisite[minIndex] = true;

        for (uint8_t j = 0; j < N_NOEUDS; j++)
        {
            if (!noeudVisite[j] && _ADJ_MATRIX[minIndex][j] && poids[minIndex] != POIDS_MAX && poids[minIndex] + _ADJ_MATRIX[minIndex][j] < poids[j])
            {
                poids[j] = poids[minIndex] + _ADJ_MATRIX[minIndex][j];
                noeudPrecedent[j] = minIndex;
            }
        }
    }

    // Reconstruire le chemin
    uint8_t noeudCourant = _destination;
    uint8_t cheminIndex = 0;

    while (noeudCourant != 0)
    {
        _dijkstraResult[cheminIndex++] = noeudCourant;
        noeudCourant = noeudPrecedent[noeudCourant];
    }
    _dijkstraResult[cheminIndex] = 0; // Ajouter le noeud de départ

    // Inverser le chemin
    for (uint8_t i = 0; i < cheminIndex / 2; i++)
    {
        uint8_t temp = _dijkstraResult[i];
        _dijkstraResult[i] = _dijkstraResult[cheminIndex - i - 1];
        _dijkstraResult[cheminIndex - i - 1] = temp;
    }

    // put a 255 at the end of the array so we know where it ends
    _dijkstraResult[cheminIndex] = 255;
}
