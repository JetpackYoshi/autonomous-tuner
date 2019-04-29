enum
{
  kAcknowledge,
  kSetSpeed,
  kToggle
};

void attachCommandCallbacks()
{
  cmdMessenger.attach(kSetSpeed, OnSetSpeed);
  cmdMessenger.attach(kToggle, OnToggle);
  cmdMessenger.printLfCr();
}

void OnSetSpeed(){
  float motorSpeed = cmdMessenger.readFloatArg();
  motor.setSpeed(motorSpeed);
//  cmdMessenger.sendCmdStart(kAcknowledge);
  cmdMessenger.sendCmd(kAcknowledge,motorSpeed);
}

void OnToggle(){
  digitalWrite(enablePin, !digitalRead(enablePin));
}

void OnSetStepSize(){
  float stepSize = cmdMessenger.readFloatArg();

  if (stepSize == 1){
    setStepSize(WHOLE);
  }
  else if (stepSize == 2){
    setStepSize(WHOLE);
  }
  else if (stepSize == 4){
    setStepSize(QUARTER);
  }
  else if (stepSize == 2){
    setStepSize(EIGHTH);
  }
}
