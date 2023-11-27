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

private:
    static const uint8_t _distanceReferenceValue = 230;
    static const uint8_t PRECISION_BITSHIFT = 2;
    static const uint8_t AN_PORT = PA0;

    uint16_t _distance;

    uint8_t _getDistance();

    can _analogicConverter;
};