#pragma once
#include <avr/io.h>
#include <avr/interrupt.h>

#include "can.h"

class InfraRedSensor
{
public:
    InfraRedSensor();
    ~InfraRedSensor();
    bool isObstacleDetected();
    uint8_t _getDistance();

private:
    static const uint8_t _distanceReferenceValue = 110;
    static const uint8_t PRECISION_BITSHIFT = 2;
    static const uint8_t AN_PORT = PA0;

    uint16_t _distance;

   

    can _analogicConverter;
};