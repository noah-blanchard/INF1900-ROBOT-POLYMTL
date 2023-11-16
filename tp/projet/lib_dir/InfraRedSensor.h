#pragma once
#include <avr/io.h>
#include <avr/interrupt.h>

#include "can.h"

volatile uint16_t distance = OxFFFF;
volatile can analogicConverter;

class InfraRedSensor {
public:
    InfraRedSensor();
    ~InfraRedSensor();

    uint16_t getDistance();
    bool detectObstacle();
private:
    
    static const uint8_t _distanceReferenceValue = 255;
    uint16_t _distance;
};