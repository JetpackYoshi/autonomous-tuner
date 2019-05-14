#include <AccelStepper.h>
#include <CmdMessenger.h>

#define enablePin 39
#define MS1 38
#define MS2 37

enum StepSizes{WHOLE,HALF,QUARTER,EIGHTH};

AccelStepper motor(1, 36, 35);
//AccelStepper motor(4, 2, 3, 4, 5);
CmdMessenger cmdMessenger = CmdMessenger(Serial);

int motorSpeed = 100;

#define kSerialSpeed 115200

void setup() {
  Serial.begin(kSerialSpeed);
  pinMode(enablePin, OUTPUT);
  pinMode(MS1, OUTPUT);
  pinMode(MS2, OUTPUT);
  attachCommandCallbacks();
  motor.setMaxSpeed(2000);
  motor.setSpeed(100);
  setStepSize(QUARTER);
}

void loop() {
  cmdMessenger.feedinSerialData();
  motor.runSpeed();
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
