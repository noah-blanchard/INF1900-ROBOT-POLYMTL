#include "InfraRedSensor.h"
#include <stdio.h>

// //volatile uint16_t distance = 0xffff;
// volatile bool canRead = false;

// ISR(ADC_vect) {
//     canRead = true;
// }

InfraRedSensor::InfraRedSensor()
{
    //_analogicConverter.enableADCInterrupt();
   // DDRA = (1 << AN_PORT);
}

InfraRedSensor::~InfraRedSensor()
{
    //_analogicConverter.disableADCInterrupt();
}

uint8_t InfraRedSensor::_getDistance()
{
    uint8_t distanceLevel = _analogicConverter.lecture(AN_PORT) >> PRECISION_BITSHIFT;
    return distanceLevel;
}

bool InfraRedSensor::isObstacleDetected()
{
    return _getDistance() >= _distanceReferenceValue;
}