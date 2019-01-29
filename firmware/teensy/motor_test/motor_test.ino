#include <AccelStepper.h>

AccelStepper stepper(4, 7, 8, 9, 10);

void setup() {
  // put your setup code here, to run once:
  stepper.setMaxSpeed(1000);

}

void loop() {
  // put your main code here, to run repeatedly:
  stepper.setSpeed(800);
  stepper.runSpeed();
}
