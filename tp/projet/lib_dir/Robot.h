// Avr Imports
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

// Library Imports
#include "Timer.h"
#include "LineMaker.h"
#include "Navigation.h"
#include "InfraRedSensor.h"
#include "Communication.h"

// Define the clock speed
#define F_CPU 8000000UL



// robot.h will be a big State Machine
/** Possible States
 * 1. Waiting for mode chose : identify corner or travel to position
 * 2. Identify corner : identify the corner and orientation and store the data
 * 3. Travel to position : travel to the position and orientation
 * 4. Calculate path using A* and adjency matrix
 * 5. Follow line
 * 6. Meet crossroad : different type, corner, T, cross ...
 * 7. Turn at crossroad
 */

// states enum class
enum class State
{
    MODE_SELECTION,
    IDENTIFY_CORNER,
    MAKE_TRIP,
    TRAVEL_POSITION_SELECTION,
    CALCULATE_PATH,
    FOLLOW_LINE,
    MEET_CROSSROAD,
    MEET_OBSTACLE,
    TURN_AT_CROSSROAD
};

// orientation enum class
enum class Orientation
{
    NORTH,
    EAST,
    SOUTH,
    WEST
};

class Robot
{
public:
    Robot();
    ~Robot();

    void runRoutine(); // this function will be called in main loop to dispatch to the right routine

private:
    static const uint8_t _BASE_SPEED = 180;
    static const uint16_t _BASE_PAUSE_DELAY = 80;

    Timer _delayTimerModule;
    LineMaker _lineMakerModule;
    Navigation _navModule;
    InfraRedSensor _irSensorModule;

    State _currentState;

    uint8_t _currentPosition[2];
    Orientation _currentOrientation;

    uint8_t _destination[2];

    void _customDelay(uint16_t delay);
    void _pause();

    // following functions are the routines for each possible states
    void _modeSelectionRoutine();
    void _identifyCornerRoutine();
    void _travelPositionSelectionRoutine();
    void _calculatePathRoutine();
    void _followLineRoutine();
    void _meetCrossroadRoutine();
    void _turnAtCrossroadRoutine();
};