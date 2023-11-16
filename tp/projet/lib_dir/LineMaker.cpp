#include "LineMaker.h"

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

Flag LineMaker::getDetectionFlag()
{
    uint8_t sensorData = _retrieveSensorData();
    Flag flag = Flag::NO_LINE;

    if (sensorData == MIDDLE)
    {
        flag = Flag::NO_ADJUSTMENT;
    }
    else if (sensorData == INNER_LEFT || sensorData == OUTER_LEFT || (sensorData & INNER_LEFT_MIDDLE) == INNER_LEFT_MIDDLE || (sensorData &INNER_OUTER_LEFT)==INNER_OUTER_LEFT)
    {
        flag = Flag::RIGHT_ADJUSTMENT;
    }
    else if (sensorData == INNER_RIGHT || sensorData == OUTER_RIGHT || (sensorData & INNER_RIGHT_MIDDLE) == INNER_RIGHT_MIDDLE || (sensorData &INNER_OUTER_RIGHT)==INNER_OUTER_RIGHT)
    {
        flag = Flag::LEFT_ADJUSTMENT;
    }
    else if (sensorData == NONE)
    {
        flag = Flag::NO_LINE;
    }

    else if ((sensorData & LEFT_INTERSECTION )== LEFT_INTERSECTION)
    {
        flag = Flag::LEFT_CROSSROAD;

    }

    else if ((sensorData & RIGHT_INTERSECTION)== RIGHT_INTERSECTION)
    {
        flag = Flag::RIGHT_CROSSROAD;

    }


    else if ((sensorData & ALL  ) == ALL )
    {
        flag = Flag::FULL_CROSSROAD;

    } 

    return flag;
}

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
