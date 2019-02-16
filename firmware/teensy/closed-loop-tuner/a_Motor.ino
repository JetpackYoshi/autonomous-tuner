#include <AccelStepper.h>

#define speedLimit 300

AccelStepper stepper(4, 2, 3, 4, 5);
float stepperSpeed;

void motorSetup(){
  stepper.setMaxSpeed(speedLimit);
}

void runStepper(){
  stepper.setSpeed(stepperSpeed);
  stepper.runSpeed();
}
