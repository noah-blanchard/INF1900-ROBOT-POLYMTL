#include "LineMaker.h"
#include <stdio.h>
#include "lcm_so1602dtr_m_fw.h"
#include "customprocs.h"

void static inline w(void)
{
    cp_wait_ms(2000);
}

LineMaker::LineMaker() {}

LineMaker::~LineMaker() {}

// void LineMaker::adjustTrajectory()
// {
//     uint8_t sensorData = _retrieveSensorData();

//     switch(sensorData){
//         case MIDDLE: {
//             _nav.go(baseSpeed, false);
//             break;
//         }

//         case INNER_LEFT: {
//             _nav.goRightWheel(adjustSpeed, false);
//             _nav.goLeftWheel(baseSpeed, false);
//             break;
//         }

//         case OUTER_LEFT: {
//             _nav.goRightWheel(adjustSpeed, false);
//             _nav.goLeftWheel(baseSpeed, false);
//             break;
//         }

//         case INNER_RIGHT: {
//            _nav.goRightWheel(baseSpeed, false);
//             _nav.goLeftWheel(adjustSpeed, false);
//             break;
//         }

//         case OUTER_RIGHT: {
//             _nav.goRightWheel(baseSpeed, false);
//             _nav.goLeftWheel(adjustSpeed, false);
//             break;
//         }

//         case NONE:{
//             _nav.stop();
//             break;
//         }

//         default:{
//             break;
//         }
//     }
// }

LineMakerFlag LineMaker::getDetectionFlag()
{
    uint8_t sensorData = _retrieveSensorData();
    LineMakerFlag flag = LineMakerFlag::NO_LINE;

    if (sensorData == ALL)
    {
        flag = LineMakerFlag::FULL_CROSSROAD;
    }

    // first if there's outer left bit on, always send outer_left_detection
    // if only outer right detect, or outer right + inner right OR outer right + inner right + middle => right cross
    else if (/*(sensorData & OUTER_LEFT) == OUTER_LEFT || (sensorData & (OUTER_LEFT | INNER_LEFT)) == (OUTER_LEFT | INNER_LEFT) ||*/ (sensorData & (OUTER_LEFT | INNER_LEFT | MIDDLE)) == (OUTER_LEFT | INNER_LEFT | MIDDLE) || (sensorData & (OUTER_LEFT | INNER_LEFT | MIDDLE | INNER_RIGHT)) == (OUTER_LEFT | INNER_LEFT | MIDDLE | INNER_RIGHT) )
    {
        flag = LineMakerFlag::OUTER_LEFT_DETECTION;
    }
    // if only outer left detect, or outer left + inner left OR outer left + inner left + middle => left cross
    else if (/*(sensorData & OUTER_RIGHT) == OUTER_RIGHT || (sensorData & (OUTER_RIGHT | INNER_RIGHT)) == (OUTER_RIGHT | INNER_RIGHT) ||*/ (sensorData & (OUTER_RIGHT | INNER_RIGHT | MIDDLE)) == (OUTER_RIGHT | INNER_RIGHT | MIDDLE) || (sensorData & (OUTER_RIGHT | INNER_RIGHT | MIDDLE | INNER_LEFT)) == (OUTER_RIGHT | INNER_RIGHT | MIDDLE | INNER_LEFT))
    {
        flag = LineMakerFlag::OUTER_RIGHT_DETECTION;
    }
    else if ((sensorData & (INNER_RIGHT | MIDDLE)) == (INNER_RIGHT | MIDDLE) || sensorData == INNER_RIGHT)
    {
        flag = LineMakerFlag::RIGHT_ADJUSTMENT;
    }
    else if ((sensorData & (INNER_LEFT | MIDDLE)) == (INNER_LEFT | MIDDLE) || sensorData == INNER_LEFT)
    {
        flag = LineMakerFlag::LEFT_ADJUSTMENT;
    }
    // else if middle bit and inner right, or only innfer right => right adjustment
    // else if only middle bit on
    else if (sensorData == MIDDLE)
    {
        flag = LineMakerFlag::NO_ADJUSTMENT;
    }
    else if (sensorData & OUTER_RIGHT)
    {
        flag = LineMakerFlag::OUTER_RIGHT_DETECTION;
    }
    else if (sensorData & OUTER_LEFT)
    {
        flag = LineMakerFlag::OUTER_LEFT_DETECTION;
    }
    // else if middle bit and inner left, or only innfer left => left adjustment

    return flag;
}

// LineMakerFlag LineMaker::getDetectionFlag()
// {
//     uint8_t sensorData = _retrieveSensorData();
//     LineMakerFlag flag = LineMakerFlag::NO_LINE;

//     if (sensorData == ALL)
//     {
//         flag = LineMakerFlag::FULL_CROSSROAD;
//     }
//     else if ((sensorData & LEFT_INTERSECTION) == LEFT_INTERSECTION || (sensorData & (LEFT_INTERSECTION | INNER_RIGHT) == (LEFT_INTERSECTION | INNER_RIGHT)))
//     {
//         flag = LineMakerFlag::LEFT_CROSSROAD;
//     }

//     else if ((sensorData & RIGHT_INTERSECTION) == RIGHT_INTERSECTION || (sensorData & (RIGHT_INTERSECTION | INNER_LEFT) == (RIGHT_INTERSECTION | INNER_LEFT)))
//     {
//         flag = LineMakerFlag::RIGHT_CROSSROAD;
//     }
//     else if (sensorData == INNER_LEFT || sensorData == OUTER_LEFT || (sensorData & INNER_LEFT_MIDDLE) == INNER_LEFT_MIDDLE || (sensorData & INNER_OUTER_LEFT) == INNER_OUTER_LEFT)
//     {
//         flag = LineMakerFlag::RIGHT_ADJUSTMENT;
//     }
//     else if (sensorData == INNER_RIGHT || sensorData == OUTER_RIGHT || (sensorData & INNER_RIGHT_MIDDLE) == INNER_RIGHT_MIDDLE || (sensorData & INNER_OUTER_RIGHT) == INNER_OUTER_RIGHT)
//     {
//         flag = LineMakerFlag::LEFT_ADJUSTMENT;
//     }

//     // outer_left_detect
//     else if (sensorData == INNER_LEFT_MIDDLE || sensorData == INNER_LEFT || sensorData == OUTER_LEFT)
//     {
//         flag = LineMakerFlag::OUTER_LEFT_DETECT;
//     }

//     // outer_right_detect
//     else if (sensorData == INNER_RIGHT_MIDDLE || sensorData == INNER_RIGHT || sensorData == OUTER_RIGHT)
//     {
//         flag = LineMakerFlag::OUTER_RIGHT_DETECT;
//     }

//     else if (sensorData == NONE)
//     {
//         flag = LineMakerFlag::NO_LINE;
//     }

//     else if (sensorData == MIDDLE)
//     {
//         flag = LineMakerFlag::NO_ADJUSTMENT;
//     }

//     return flag;
// }

uint8_t LineMaker::_retrieveSensorData()
{
    uint8_t sensorData = 0;

    // Masques pour isoler les bits des capteurs spécifiques
    uint8_t maskOuterLeft = (1 << OUTER_LEFT_SENSOR);
    uint8_t maskInnerLeft = (1 << INNER_LEFT_SENSOR);
    uint8_t maskMiddle = (1 << MIDDLE_SENSOR);
    uint8_t maskInnerRight = (1 << INNER_RIGHT_SENSOR);
    uint8_t maskOuterRight = (1 << OUTER_RIGHT_SENSOR);

    // Extraire chaque bit de capteur et décaler à la position appropriée
    sensorData |= ((PINA & maskOuterLeft) >> OUTER_LEFT_SENSOR) << 0;   // Décaler le bit OUTER_LEFT_SENSOR à la position 0
    sensorData |= ((PINA & maskInnerLeft) >> INNER_LEFT_SENSOR) << 1;   // Décaler le bit INNER_LEFT_SENSOR à la position 1
    sensorData |= ((PINA & maskMiddle) >> MIDDLE_SENSOR) << 2;          // Décaler le bit MIDDLE_SENSOR à la position 2
    sensorData |= ((PINA & maskInnerRight) >> INNER_RIGHT_SENSOR) << 3; // Décaler le bit INNER_RIGHT_SENSOR à la position 3
    sensorData |= ((PINA & maskOuterRight) >> OUTER_RIGHT_SENSOR) << 4; // Décaler le bit OUTER_RIGHT_SENSOR à la position 4

    return sensorData;
}
