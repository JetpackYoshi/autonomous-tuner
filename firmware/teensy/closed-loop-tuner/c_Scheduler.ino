#include <Metro.h>
#define MOTOR_INTERVAL 10 //Defined in Microseconds

#define LCD_REFRESH_INTERVAL 100 //Defined in Milliseconds
bool LCD_REFRESH_TOGGLE = true;

bool PITCH_DETECT_TOGGLE = true;

IntervalTimer myTimer;
Metro lcdMetro = Metro(LCD_REFRESH_INTERVAL);

void scheduleRoutines(){
  myTimer.begin(runStepper, 10);
  //myTimer.priority(1);
}

void performPolledRoutines(){
//  if ((lcdMetro.check() == 1) && LCD_REFRESH_TOGGLE){
//    null; //do stuff
//  }
  if (PITCH_DETECT_TOGGLE){
    detectPitch();
  }
}
