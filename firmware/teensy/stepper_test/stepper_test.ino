/*  MotorKnob - slightly modified
 *
 * A stepper motor follows the turns of a potentiometer
 * (or other sensor) on analog input 0.
 */

#include <Stepper.h>

// the number of steps on your motor
#define STEPS 200

// create an instance of the stepper class, specifying
// the number of steps of the motor and the pins it's
// attached to
Stepper stepper(STEPS, 7, 9, 8, 10);

// the previous reading from the analog input
long previous = STEPS / 2;

void setup()
{
  // set the speed of the motor to 30 RPMs
  stepper.setSpeed(30);
}

void loop()
{
  // get the sensor value.  This equation maps the
  // analogRead range to the number of steps, so the
  // motor nicely tracks a potentiometer's position
  long val = (long)analogRead(0) * STEPS / 1024;

  // move a number of steps equal to the change in the
  // sensor reading
  stepper.step(val - previous);

  // remember the previous value of the sensor
  previous = val;
}
