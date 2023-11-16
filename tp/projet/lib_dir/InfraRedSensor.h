#pragma once
#include <avr/io.h>
#include <avr/interrupt.h>

#include "can.h"

class InfraRedSensor {
public:
    InfraRedSensor();
    ~InfraRedSensor();

    uint16_t getDistance();
    bool isObstacleDetected();
private:
    
    static const uint16_t _distanceReferenceValue = 1000;
    uint16_t _distance;

    can _analogicConverter;
};