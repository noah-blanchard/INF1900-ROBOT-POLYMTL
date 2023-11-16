#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "Robot.h"

int main(){

    Robot robot;

    while(true){
        robot.runRoutine();
    }

    return 0;

}