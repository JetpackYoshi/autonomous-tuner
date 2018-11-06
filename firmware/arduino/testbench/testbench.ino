#include "HX711.h"
#include <EEPROM.h>

// EEPROM
int eeAddress = 0;
float calibration_factor = -7050;
float tension;

//Serial Communication
String inputString = "";
boolean stringComplete = false;

void setup() {
  Serial.begin(9600);
  inputString.reserve(200); // reserving 200 bytes for the input string
  load();
  scaleSetup();
  motorSetup();
  lcdSetup();
}

void loop() {
  getTension();
  cmd();
}


