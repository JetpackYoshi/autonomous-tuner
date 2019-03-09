#include <Encoder.h>
#include <PID_v1.h>

#define motorPWM 37
#define motorDir1 38
#define motorDir2 39
Encoder myEnc(6, 7);

String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

//Define Variables we'll be connecting to
double Setpoint, Input, Output;

//Specify the links and initial tuning parameters
double Kp=0.8, Ki=0.1, Kd=0.1;
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

void setup() {
  Serial.begin(115200);
  analogWriteResolution(12);
  pinMode(9,OUTPUT);
  pinMode(motorDir1,OUTPUT);
  pinMode(motorDir2,OUTPUT);

  Input = myEnc.read();
  Setpoint = 5000;

  myPID.SetMode(AUTOMATIC);
  myPID.SetOutputLimits(-4095, 4095);

  inputString.reserve(200);
}

long oldPosition  = -999;

void loop() {
  if (stringComplete) {
    Setpoint = inputString.toInt(); 
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
  Input = myEnc.read();
  myPID.Compute();
  if(Output>0){
    digitalWrite(motorDir1, HIGH);
    digitalWrite(motorDir2, LOW);
  }else{
    digitalWrite(motorDir1, LOW);
    digitalWrite(motorDir2, HIGH);
  }
  //Serial.println(Output);
  analogWrite(37, abs(Output));
  if (Input != oldPosition) {
    Serial.println(Input);
    oldPosition = Input;
  }

}

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}
