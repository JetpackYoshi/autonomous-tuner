#include <PID_v1.h>
#include <PID_AutoTune_v0.h>
#include <Encoder.h>

#define pwmPin 37
#define dirPin1 38
#define dirPin2 39
#define encA 6
#define encB 7

byte ATuneModeRemember=2;
double input=80, output=50, setpoint=180;
double kp=0.5,ki=0,kd=0;

double kpmodel=1.5, taup=100, theta[50];
double outputStart=5;
double aTuneStep=50, aTuneNoise=1, aTuneStartValue=100;
unsigned int aTuneLookBack=20;

long oldPosition  = -999;
long newPosition = 0;

boolean tuning = false;
unsigned long  modelTime, serialTime, prevSampleTime;

Encoder myEnc(encA, encB);
PID myPID(&input, &output, &setpoint,kp,ki,kd, DIRECT);
PID_ATune aTune(&input, &output);

//set to false to connect to the real world
boolean useSimulation = false;

void setup()
{
  if(useSimulation)
  {
    for(byte i=0;i<50;i++)
    {
      theta[i]=outputStart;
    }
    modelTime = 0;
  }
  //Setup the pid 
  myPID.SetMode(AUTOMATIC);
  myPID.SetOutputLimits(-255, 255);

  if(tuning)
  {
    tuning=false;
    changeAutoTune();
    tuning=true;
  }
  
  serialTime = 0;
  analogWriteResolution(8);
  pinMode(9,OUTPUT);
  pinMode(dirPin1,OUTPUT);
  pinMode(dirPin2,OUTPUT);
  Serial.begin(9600);

}

void loop()
{

  unsigned long now = millis();

  if(!useSimulation)
  { //pull the input in from the real world
    newPosition = myEnc.read();
    float velocity = (newPosition - oldPosition) / (now-prevSampleTime);
    prevSampleTime = now;
    Serial.println(velocity);
    if (newPosition != oldPosition) {
      oldPosition = newPosition;
      }
    input = newPosition;
    //input = velocity;
  }
  
  if(tuning)
  {
    byte val = (aTune.Runtime());
    if (val!=0)
    {
      tuning = false;
    }
    if(!tuning)
    { //we're done, set the tuning parameters
      kp = aTune.GetKp();
      ki = aTune.GetKi();
      kd = aTune.GetKd();
      myPID.SetTunings(kp,ki,kd);
      AutoTuneHelper(false);
    }
  }
  else myPID.Compute();
  
  if(useSimulation)
  {
    theta[30]=output;
    if(now>=modelTime)
    {
      modelTime +=100; 
      DoModel();
    }
  }
  else
  {
    if (output < 0) {
      digitalWrite(dirPin2, LOW);//backward if high
      digitalWrite(dirPin1, HIGH);//forward if high
      //Serial.println("FW");
      }  
    if (output > 0) {
      digitalWrite(dirPin2, HIGH);//backward if high
      digitalWrite(dirPin1, LOW);//forward if high
      //Serial.println("BK");
      }

    analogWrite(pwmPin,abs(output));
   
  }
  
  //send-receive with processing if it's time
  if(millis()>serialTime)
  {
    SerialReceive();
    SerialSend();
    serialTime+=500;
  }
}

void changeAutoTune()
{
 if(!tuning)
  {
    //Set the output to the desired starting frequency.
    output=aTuneStartValue;
    aTune.SetNoiseBand(aTuneNoise);
    aTune.SetOutputStep(aTuneStep);
    aTune.SetLookbackSec((int)aTuneLookBack);
    AutoTuneHelper(true);
    tuning = true;
  }
  else
  { //cancel autotune
    aTune.Cancel();
    tuning = false;
    AutoTuneHelper(false);
  }
}

void AutoTuneHelper(boolean start)
{
  if(start)
    ATuneModeRemember = myPID.GetMode();
  else
    myPID.SetMode(ATuneModeRemember);
}


void SerialSend()
{
  Serial.print("setpoint: ");Serial.print(setpoint); Serial.print(" ");
  Serial.print("input: ");Serial.print(input); Serial.print(" ");
  Serial.print("output: ");Serial.print(output); Serial.print(" ");
  if(tuning){
    Serial.println("tuning mode");
  } else {
    Serial.print("kp: ");Serial.print(myPID.GetKp());Serial.print(" ");
    Serial.print("ki: ");Serial.print(myPID.GetKi());Serial.print(" ");
    Serial.print("kd: ");Serial.print(myPID.GetKd());Serial.println();
  }
}

void SerialReceive()
{
  if(Serial.available())
  {
   char b = Serial.read(); 
   Serial.flush(); 
   if((b=='1' && !tuning) || (b!='1' && tuning))changeAutoTune();
  }
}

void DoModel()
{
  //cycle the dead time
  for(byte i=0;i<49;i++)
  {
    theta[i] = theta[i+1];
  }
  //compute the input
  input = (kpmodel / taup) *(theta[0]-outputStart) + input*(1-1/taup) + ((float)random(-10,10))/100;

}
