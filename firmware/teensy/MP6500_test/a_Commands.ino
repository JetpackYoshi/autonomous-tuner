enum
{
  kAcknowledge,
  kSetSpeed,
};

void attachCommandCallbacks()
{
  cmdMessenger.attach(kSetSpeed, OnSetSpeed);
  cmdMessenger.printLfCr();
}

void OnSetSpeed(){
  float motorSpeed = cmdMessenger.readFloatArg();
  motor.setSpeed(motorSpeed);
//  cmdMessenger.sendCmdStart(kAcknowledge);
  cmdMessenger.sendCmd(kAcknowledge,motorSpeed);
}
