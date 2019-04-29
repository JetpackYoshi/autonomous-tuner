#include <AccelStepper.h>
#include <CmdMessenger.h>
IntervalTimer myTimer;

//AccelStepper motor(1, 2, 3);
AccelStepper motor(4, 2, 3, 4, 5);
int pos = 0;

void setup() {
  //Serial.begin(kSerialSpeed);

  motor.setMaxSpeed(2000);
  motor.setSpeed(1);
  myTimer.begin(runStepper, 10);

}

void loop() {
  motor.setSpeed(261.63);
  delay(500);
  motor.setSpeed(293.66);
  delay(500);
  motor.setSpeed(329.63);
  delay(500);
  motor.setSpeed(349.23);
  delay(500);
  motor.setSpeed(392.00);
  delay(500);
  motor.setSpeed(440.00);
  delay(500);
  motor.setSpeed(493.88);
  delay(500);
  motor.setSpeed(523.25);
  delay(500);
//  for (pos = 0; pos <= 2000; pos += 1) { // goes from 0 degrees to 180 degrees
//    // in steps of 1 degree
////    motor.setSpeed(pos);              // tell servo to go to position in variable 'pos'
//    delay(10);                       // waits 15ms for the servo to reach the position
//  }
//  for (pos = 2000; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
////    motor.setSpeed(pos);              // tell servo to go to position in variable 'pos'
//    delay(10);                       // waits 15ms for the servo to reach the position
//  }
}

void runStepper(){
  motor.runSpeed();
}
