#pragma once
#include <avr/io.h>
#include <avr/interrupt.h>

#include "can.h"

class InfraRedSensor {
public:
    InfraRedSensor();
    ~InfraRedSensor();

    uint8_t getDistance();
    bool isObstacleDetected();
private:
    
    static const uint8_t _distanceReferenceValue = 230;
    static const uint8_t PRECISION_BITSHIFT = 2;

    uint16_t _distance;

    can _analogicConverter;
};