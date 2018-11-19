#include <AccelStepper.h>
#define ANALOG_IN A0

float pitch;
float setPoint;

AccelStepper stepper;

void setup() {
  stepper.setMaxSpeed(1000);

}

void loop() {
  int val = map(analogRead(ANALOG_IN), 0, 1023, -1000, 1000);
  stepper.setSpeed(val);
  stepper.runSpeed();
//  int analog_in = analogRead(ANALOG_IN);
//  stepper.moveTo(analog_in);
//  stepper.setSpeed(1000);
//  stepper.runSpeedToPosition();
}
