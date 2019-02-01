#define MOTOR_INTERVAL 10 //Defined in Microseconds

IntervalTimer myTimer;

void scheduleRoutines(){
  myTimer.begin(runStepper, 10);
  //myTimer.priority(1);
}
