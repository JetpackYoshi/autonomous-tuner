#include <AccelStepper.h>
#include <CmdMessenger.h>

AccelStepper motor(1, 2, 3);
//AccelStepper motor(4, 2, 3, 4, 5);
CmdMessenger cmdMessenger = CmdMessenger(Serial);

int motorSpeed = 100;

#define kSerialSpeed 115200

void setup() {
  Serial.begin(kSerialSpeed);
  attachCommandCallbacks();
  motor.setMaxSpeed(2000);
  motor.setSpeed(100);

}

void loop() {
  cmdMessenger.feedinSerialData();
  motor.runSpeed();
}
