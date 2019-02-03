#include <AccelStepper.h>

AccelStepper stepper(4, 7, 8, 9, 10);
float stepperSpeed;

void motorSetup(){
  stepper.setMaxSpeed(300);
}
void runStepper(){
  stepper.setSpeed(stepperSpeed);
  stepper.runSpeed();
}
