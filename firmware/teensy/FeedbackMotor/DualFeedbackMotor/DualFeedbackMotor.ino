#include <Encoder.h>
#include <PID_v1.h>

#define Freq_Ts 100
#define pwmPin 37
#define dirPin1 2
#define dirPin2 3
#define encA 6
#define encB 7

double Setpoint, Input, Output;
double p_Kp=2, p_Ki=5, p_Kd=1;
double v_Kp=2, v_Ki=5, v_Kd=1;
double lastError = 0;
double sumError = 0;

long oldPosition  = -999;
long newPosition = 0;

long velocity = 0;

enum ControlModes{POSITION, VELOCITY};
ControlModes controlMode = POSITION;

Encoder myEnc(encA, encB);
PID positionPID(&Input, &Output, &Setpoint, p_Kp, p_Ki, p_Kd, DIRECT);
PID velocityPID(&Input, &Output, &Setpoint, v_Kp, v_Ki, v_Kd, DIRECT);

void motorSetup(){
  pinMode(pwmPin, OUTPUT); 
  pinMode(dirPin1, OUTPUT);
  pinMode(dirPin2, OUTPUT);
  
  Output = 0;
  positionPID.SetMode(AUTOMATIC);
  velocityPID.SetMode(AUTOMATIC);
  
  positionPID.SetOutputLimits(-1023, 1023);
  velocityPID.SetOutputLimits(-1023, 1023);
}

void motorCalc(){
  newPosition = myEnc.read();
  velocity = (newPosition - oldPosition) / (Freq_Ts*0.000001);
  if (newPosition != oldPosition) {
    oldPosition = newPosition;
    //Serial.println(newPosition);
    //Serial.println(newPosition);
  }

  switch (controlMode){
    case POSITION: {
      Input = newPosition;
      positionPID.Compute();
      break;
    }
    case VELOCITY: {
      Input = velocity;
      velocityPID.Compute();
      break;
    }
  }

  if (Output < 0) {
      digitalWrite(dirPin2, LOW);//backward if high
      digitalWrite(dirPin1, HIGH);//forward if high
      //Serial.println("FW");
  }  
  if (Output > 0) {
      digitalWrite(dirPin2, HIGH);//backward if high
      digitalWrite(dirPin1, LOW);//forward if high
      //Serial.println("BK");
  }

  analogWrite(pwmPin,abs(Output));
  
}
