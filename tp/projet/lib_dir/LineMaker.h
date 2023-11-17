#include <avr/io.h>

#define OUTER_LEFT_SENSOR PA2
#define INNER_LEFT_SENSOR PA4
#define MIDDLE_SENSOR PA5
#define INNER_RIGHT_SENSOR PA6
#define OUTER_RIGHT_SENSOR PA7
#define DDR_SENSOR DDRA
#define PORT_SENSOR PORTA
#define PIN_SENSOR PINA

enum class Flag
{
    LEFT_ADJUSTMENT,
    RIGHT_ADJUSTMENT,
    NO_ADJUSTMENT,
    NO_LINE,
    FULL_CROSSROAD,
    //T_CROSSROAD,
    CORNER_CROSSROAD,
    LEFT_CROSSROAD,
    RIGHT_CROSSROAD,
    };

class LineMaker
{

public:
    LineMaker();
    ~LineMaker();

    Flag getDetectionFlag();

    static const uint8_t NONE = 0b00000;
    static const uint8_t OUTER_LEFT = 0b00001;
    static const uint8_t INNER_LEFT = 0b00010;
    static const uint8_t MIDDLE = 0b00100;
    static const uint8_t INNER_RIGHT = 0b01000;
    static const uint8_t OUTER_RIGHT = 0b10000;
    static const uint8_t ALL = 0b11111;

    //Je rajoute des static const pour eviter les longues lignes de codes
    static const uint8_t LEFT_INTERSECTION =   OUTER_LEFT | INNER_LEFT  | MIDDLE;
    static const uint8_t RIGHT_INTERSECTION = OUTER_RIGHT | INNER_RIGHT | MIDDLE;

    static const uint8_t INNER_LEFT_MIDDLE  =  INNER_LEFT | MIDDLE;
    static const uint8_t INNER_RIGHT_MIDDLE = INNER_RIGHT | MIDDLE;

    static const uint8_t INNER_OUTER_LEFT =  INNER_LEFT | OUTER_LEFT ;
    static const uint8_t INNER_OUTER_RIGHT = INNER_RIGHT | OUTER_RIGHT ;




private:
    uint8_t _retrieveSensorData();
};
