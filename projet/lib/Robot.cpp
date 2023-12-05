#include "Robot.h"

volatile bool mbButtonPressed = false;
volatile bool selButtonPressed = false;
volatile bool valButtonPressed = false;

// ISR FOR IN0 and INT2

ISR(INT0_vect)
{
    mbButtonPressed = true;
    _delay_ms(DEBOUNCE_DELAY);
    EIFR |= (1 << INTF0);
}

ISR(INT2_vect)
{
    selButtonPressed = true;
    _delay_ms(DEBOUNCE_DELAY);
    EIFR |= (1 << INTF2);
}

ISR(INT1_vect)
{
    valButtonPressed = true;
    _delay_ms(DEBOUNCE_DELAY);
    EIFR |= (1 << INTF1);
}

Robot::Robot() : _display(&DDRC, &PORTC), _navModule(_currentPosition, &_currentOrientation), _ledModule(&PORTB, &DDRB, PB6, PB7), _identifyCornerModule(&_ledModule), _maketripModule(&mbButtonPressed, &selButtonPressed, &valButtonPressed)
{

    // SETUP ROBOT BUTTONS
    cli();
    _motherBoardButton = Bouton(INT0);
    _validateButton = Bouton(INT1);
    _selectButton = Bouton(INT2);
    _motherBoardButton.setFallingEdge();
    _motherBoardButton.enableInterrupt();
    _validateButton.setFallingEdge();
    _validateButton.enableInterrupt();
    _selectButton.setFallingEdge();
    _selectButton.enableInterrupt();
    sei();

    // SETUP ROBOT POSITION AND ORIENTATION
    _beginning[0] = 0;
    _beginning[1] = 0;
    _currentOrientation = Orientation::SOUTH;
    _currentPosition[0] = 0;
    _currentPosition[1] = 0;

    // SETUP ROBOT STATE
    _currentState = RobotState::MODE_SELECTION;

    // DISPLAY INIT MESSAGE
    _display.clear();
    _display.write("HELLO");
    _display.write("I AM TOP G", LCM_FW_HALF_CH);

    // INIT TURN LED RED
    _ledModule.turnLedRed();
}

Robot::~Robot()
{
}

void Robot::runRoutine()
{

    switch (_currentState)
    {
    case RobotState::MODE_SELECTION:
    {
        _modeSelectionRoutine();
        break;
    }

    case RobotState::IDENTIFY_CORNER:
    {
        _identifyCornerModule.identificationProcess(_beginning);
        _currentState = RobotState::NONE;
        break;
    }

    case RobotState::MAKE_TRIP:
    {
        _dijkstraModule.resetAdjMatrix();
        _maketripModule.run(_destination);
        _currentState = RobotState::CALCULATE_PATH;
        break;
    }
    case RobotState::CALCULATE_PATH:
    {
        _calculatePathRoutine();
        break;
    }
    case RobotState::NAVIGATE_TRIP:
    {
        _navigateTripRoutine();
        break;
    }
    case RobotState::PARKING:
    {
        _parkingRoutine();
        break;
    }

    default:
    {
        break;
    }
    }
}

void Robot::_calculatePathRoutine()
{
    _dijkstraModule.run(_beginning, _destination, _moveArray);
    _currentState = RobotState::NAVIGATE_TRIP;
}

void Robot::_navigateTripRoutine()
{
    Move tripResult = _navModule.followTrip(_moveArray); // returns the last move of the trip ! will return the one with a post if invalid
    if (tripResult.orientation != Orientation::FINISHED)
    {
        char buffer[28];

        _beginning[0] = _currentPosition[0];
        _beginning[1] = _currentPosition[1];
        _dijkstraModule.removeNode(tripResult.x, tripResult.y);
        _currentState = RobotState::CALCULATE_PATH;
    }
    else
    {
        _beginning[0] = tripResult.x;
        _beginning[1] = tripResult.y;
        _currentState = RobotState::PARKING;
    }
}

void Robot::_parkingRoutine()
{
    _navModule.parking();
    _delay_ms(500);

    for (uint8_t i = 0; i < 5; i++)
    {
        _soundModule.chooseFrequency(80);
        _delay_ms(200);
        _soundModule.stopSound();
        _delay_ms(100);
    }

    _currentState = RobotState::MAKE_TRIP;
}

void Robot::_modeSelectionRoutine()
{
    if (selButtonPressed)
    {
        selButtonPressed = false;
        _display = "Make a Trip";
        _currentState = RobotState::MAKE_TRIP;
    }
    else if (mbButtonPressed)
    {
        mbButtonPressed = false;
        _display = "Identify Corner";
        _ledModule.turnOffLed();
        _delay_ms(IDC_INIT_DELAY);
        _currentState = RobotState::IDENTIFY_CORNER;
    }
}