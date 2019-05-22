#include <AccelStepper.h>

#define speedLimit 1000

#define enablePin 39
#define MS1 38
#define MS2 37

//L298N//
//AccelStepper stepper(4, 2, 3, 4, 5);

//ULN2003//
//AccelStepper stepper(4, 2, 3, 5, 4);

//MP6500//
AccelStepper stepper(1, 36, 35);

float stepperSpeed;

void motorSetup(){
  stepper.setMaxSpeed(speedLimit);
  pinMode(enablePin, OUTPUT);
  pinMode(MS1, OUTPUT);
  pinMode(MS2, OUTPUT);
  setStepSize(QUARTER);
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
  digitalWrite(enablePin, HIGH);
}

void setStepSize(StepSizes stepSize){
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
