#include <Encoder.h>
//#include <PID_v1.h>

#define Freq_Ts 100 //Calculation period in microseconds

#define pwmPin = 10;
#define dirPin = 3;
#define encA = 9;
#define encB = 10;

double Setpoint, Input, Output;
int Kp = 1;
int Ki = 1;
int Kd = 1;
double lastError = 0;
double sumError = 0;

long oldPosition  = -999;
long newPosition = 0;
long velocity = 0;

Encoder myEnc(encA, encB);

void setTarget(int setpoint){
  Setpoint = setpoint;
}

void setKp(int kp){
  Kp = kp;
}

void motorSetup(){
  Output = 0;
  pinMode(pwmPin, OUTPUT); 
  pinMode(dirPin, OUTPUT);
}

void motorStep(){
  newPosition = myEnc.read();
  velocity = (newPosition - oldPosition) / (Freq_Ts*0.000001);
  if (newPosition != oldPosition) {
    oldPosition = newPosition;
    //Serial.println(newPosition);
    //Serial.println(newPosition);
  }
  Input = newPosition;
  double target = Setpoint;// set the seek to target by mapping the potentiometer range to the encoder max count
  double error = Input - target; // find the error term = current position - target

  //PID Calculation
  //double motorSpeed = Kp * error; //Just using P
  double motorSpeed = Kp * error + Kd * (error - lastError) + Ki * sumError; //Full PID
  
  //Correction for motor Speed sign
  if (motorSpeed < 0) {
    motorSpeed = motorSpeed * -1;
  }

  if (error < 0) {
  digitalWrite(3, LOW);//backward if high
      digitalWrite(2, HIGH);//forward if high
      Serial.println("FW");
  }  
  if (error > 0) {
  digitalWrite(3, HIGH);//backward if high
      digitalWrite(2, LOW);//forward if high
      Serial.println("BK");
  }  
  lastError = error;
  sumError += error; 

  Output = constrain(motorSpeed, 0,150);
  Output = abs(Output);
  analogWrite(10,Output);
}



