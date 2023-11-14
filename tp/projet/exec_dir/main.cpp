#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "LineMaker.h"

int main(){

    LineMaker lineMaker;

    while(true){
        lineMaker.adjustTrajectory();
    }

    return 0;

}