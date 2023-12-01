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
   // DDRA = (1 << AN_PORT);
}

InfraRedSensor::~InfraRedSensor()
{
    //_analogicConverter.disableADCInterrupt();
}

uint16_t InfraRedSensor::_getDistance()
{
    uint16_t distanceLevel = _analogicConverter.lecture(AN_PORT);
    return distanceLevel;
}

bool InfraRedSensor::isObstacleDetected()
{
    return false;
}