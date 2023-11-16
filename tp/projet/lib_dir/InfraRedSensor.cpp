#include "InfraRedSensor.h"
#include "Communication.h"
#include <stdio.h>

// //volatile uint16_t distance = 0xffff;
// volatile bool canRead = false;

// ISR(ADC_vect) {
//     canRead = true;
// }

InfraRedSensor::InfraRedSensor() {
    //_analogicConverter.enableADCInterrupt();

}

InfraRedSensor::~InfraRedSensor() {
    //_analogicConverter.disableADCInterrupt();
}

uint8_t InfraRedSensor::getDistance(){
    uint8_t distanceLevel = _analogicConverter.lecture(PA0) >> PRECISION_BITSHIFT;
    return distanceLevel;
}

bool InfraRedSensor::isObstacleDetected() {
    return getDistance() >= _distanceReferenceValue;
}