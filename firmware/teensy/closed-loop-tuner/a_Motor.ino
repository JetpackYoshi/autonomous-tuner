#include <AccelStepper.h>

#define speedLimit 2000

//H-Bridge//
//AccelStepper stepper(4, 2, 3, 4, 5);

//ULN2003//
//AccelStepper stepper(4, 2, 3, 5, 4);
//AccelStepper stepper(4, 4, 2, 5, 3);

//MP6500//
AccelStepper stepper(1, 2, 3);

float stepperSpeed;

void motorSetup(){
  stepper.setMaxSpeed(speedLimit);
}

void runStepper(){
  stepper.setSpeed(stepperSpeed);
//  stepper.setSpeed(-Output);
  stepper.runSpeed();
}

void stopMotor(){
  noInterrupts();
  stepperSpeed = 0;
  interrupts();
}
