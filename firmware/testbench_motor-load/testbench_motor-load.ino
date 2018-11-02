#include <AccelStepper.h>
#include "HX711.h"

//Load Cell
#define DOUT 8
#define CLK 9
HX711 scale(DOUT, CLK);
float calibration_factor = -7050;
float tension;

//Stepper Motor
AccelStepper stepper; // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5

//Serial Communication
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;

void setup() {
  Serial.begin(9600);
  inputString.reserve(200); // reserving 200 bytes for the input string
  scale.set_scale(calibration_factor);
  //scale.tare(); //Assuming there is no weight on the scale
  stepper.setMaxSpeed(200.0);
  stepper.setAcceleration(100.0);

  stepper.runToNewPosition(0);
}

void loop() {
  //Read Scale
  tension = scale.get_units();
  
  if (stringComplete) {
    if (inputString.startsWith("QUERY",0)){
      Serial.println(tension, 8);
    }
    else if(inputString.startsWith("TARE",0)){
      scale.tare();
    }
    else{
    Serial.println(inputString);
    stepper.runToNewPosition(inputString.toInt());
    }
    
    inputString = "";
    stringComplete = false;
    
  }

}

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}
