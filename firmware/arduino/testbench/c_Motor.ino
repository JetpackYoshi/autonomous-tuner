#include <AccelStepper.h>

AccelStepper stepper; // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5

void motorSetup(){
  stepper.setMaxSpeed(200.0);
  stepper.setAcceleration(100.0);
  stepper.runToNewPosition(0);
}


