#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "LineMaker.h"

int main(){

    LineMaker lineMaker;
    Navigation nav;

    while(true){
        lineMaker.adjustTrajectory();
        //nav.go(255, false);
    }

    return 0;

}