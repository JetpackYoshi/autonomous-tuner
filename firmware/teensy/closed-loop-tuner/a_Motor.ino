#include <AccelStepper.h>

#define speedLimit 1000

#define enablePin 39
#define MS1 38
#define MS2 37

//MP6500//
AccelStepper stepper(1, 36, 35);

float stepperSpeed;

void motorSetup(){
  stepper.setMaxSpeed(speedLimit);
  pinMode(enablePin, OUTPUT);
  pinMode(MS1, OUTPUT);
  pinMode(MS2, OUTPUT);
  setStepSize(QUARTER);
  stopMotor();
  
}

void runStepper(){
  stepper.setSpeed(stepperSpeed);
//  stepper.setSpeed(-Output);
  stepper.runSpeed();
}

void stopMotor(){
  // Set motor speed to zero and disable motor driver
  noInterrupts();
  stepperSpeed = 0;
  interrupts();
  digitalWrite(enablePin, HIGH);
}

void setStepSize(StepSizes stepSize){
  // Sets the step size of the MP6500 Motor Driver
  
  // *MS1* | *MS2* | *Microstep Resolution*
  // ------|-------------------------------
  //  Low  |  Low  |  Full step
  // ------|-------|-----------------------
  //  High |  Low  |  Half (1/2) step
  // ------|-------|-----------------------
  //  Low  |  High |  Quarter (1/4) step
  // ------|-------|-----------------------
  //  High |  High |  Eighth (1/8) step
  // --------------------------------------
  
  switch(stepSize){
    case WHOLE: {
      digitalWrite(MS1, LOW);
      digitalWrite(MS2, LOW);
      break;
    }
    case HALF: {
      digitalWrite(MS1, HIGH);
      digitalWrite(MS2, LOW);
      break;
    }
    case QUARTER: {
      digitalWrite(MS1, LOW);
      digitalWrite(MS2, HIGH);
      break;
    }
    case EIGHTH: {
      digitalWrite(MS1, HIGH);
      digitalWrite(MS2, HIGH);
      break;
    }
  }
}
