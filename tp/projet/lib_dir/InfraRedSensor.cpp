#include "InfraRedSensor.h"
#include "Communication.h"
#include <stdio.h>

// //volatile uint16_t distance = 0xffff;
// volatile bool canRead = false;

// ISR(ADC_vect) {
//     canRead = true;
// }

InfraRedSensor::InfraRedSensor()
{
    //_analogicConverter.enableADCInterrupt();
}

InfraRedSensor::~InfraRedSensor()
{
    //_analogicConverter.disableADCInterrupt();
}

uint16_t InfraRedSensor::_getDistance()
{
    uint16_t distanceLevel = _analogicConverter.lecture(AN_PORT) >> PRECISION_BITSHIFT;
    return distanceLevel;
}

bool InfraRedSensor::isObstacleDetected()
{
    return _getDistance() < 100;
}