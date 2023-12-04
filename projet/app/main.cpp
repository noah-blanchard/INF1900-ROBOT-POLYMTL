#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "Robot.h"
//#include "lcm_so1602dtr_m_fw.h"


int main(){

    //LCM _disp(&DDRC, &PORTC);
    Robot robot;
    //_disp.write("HAHA");
    while(true){
       robot.runRoutine();
    }

    return 0;

}