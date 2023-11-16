#include "InfraRedSensor.h"

ISR(ADC_vect) {
    distance = analogicConverter.lecture(0);
}

InfraRedSensor::InfraRedSensor() {
    this->_can.enableADCInterrupt();
}

InfraRedSensor::~InfraRedSensor() {
    this->_can.disableADCInterrupt();
}

bool InfraRedSensor::detectObstacle() {
    return distance < _distanceReferenceValue;
}