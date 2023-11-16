#include "InfraRedSensor.h"

//volatile uint16_t distance = 0xffff;
volatile bool canRead = false;

ISR(ADC_vect) {
    canRead = true;
}

InfraRedSensor::InfraRedSensor() {
    _analogicConverter.enableADCInterrupt();

}

InfraRedSensor::~InfraRedSensor() {
    _analogicConverter.disableADCInterrupt();
}

uint16_t InfraRedSensor::getDistance(){
    return _analogicConverter.lecture(0);
}

bool InfraRedSensor::isObstacleDetected() {
    bool returnValue = false;
    if(canRead){
        returnValue = getDistance() >= _distanceReferenceValue;
        canRead = false;
        }
    return returnValue;
}