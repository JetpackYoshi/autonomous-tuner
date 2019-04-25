#include <Metro.h>
#define MOTOR_INTERVAL 10 //Defined in Microseconds

bool PITCH_DETECT_TOGGLE = true;

IntervalTimer myTimer;
//Metro lcdMetro = Metro(LCD_REFRESH_INTERVAL);

void scheduleRoutines(){
  myTimer.begin(runStepper, 10);
  //myTimer.priority(1);
}

void performPolledRoutines(){
  tuningPID.Compute();
  if (PITCH_DETECT_TOGGLE){
    detectPitch();
    runStateMachine();
  }
}
