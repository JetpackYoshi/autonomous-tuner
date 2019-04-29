#include <Metro.h>
#define MOTOR_INTERVAL 10 //Defined in Microseconds

IntervalTimer myTimer;
Metro ledMetro = Metro(500);

void scheduleRoutines(){
  myTimer.begin(runStepper, 10);
  pinMode(13,OUTPUT);
  //myTimer.priority(1);
}

void performPolledRoutines(){
  tuningPID.Compute();
  detectPitch();
  runStateMachine();
  
  //blink LED
  if (ledMetro.check() == 1){
    digitalWrite(13, !digitalRead(13));
  }
}
