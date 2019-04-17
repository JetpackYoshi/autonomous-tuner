enum
{
  kAcknowledge,
  kGetPitchData,
  kSetTarget,
  kBeginTuning,
  kStopTuning,
  kCalibrate,
  kToggleRawStream
};

void attachCommandCallbacks()
{
  cmdMessenger.attach(kGetPitchData, OnGetPitchData);
  cmdMessenger.attach(kSetTarget, OnSetTarget);
  cmdMessenger.attach(kBeginTuning, OnBeginTuning);
  cmdMessenger.attach(kStopTuning, OnStopTuning);
  cmdMessenger.attach(kCalibrate, OnCalibrate);
  cmdMessenger.attach(kToggleRawStream, toggleRawStream);
  cmdMessenger.printLfCr();
}

void OnGetPitchData(){
  cmdMessenger.sendCmdStart(kAcknowledge);
  cmdMessenger.sendCmdArg(FREQ_TARGET);
  cmdMessenger.sendCmdArg(filtered_note);
  cmdMessenger.sendCmdEnd();
}

void toggleRawStream(){
  stream = !stream;
  if (stream){
    cmdMessenger.sendCmd(kAcknowledge, "Streaming On");
  }else{
    cmdMessenger.sendCmd(kAcknowledge, "Streaming Off");
  }
}

void OnBeginTuning(){
  tuningState = TUNING;
  cmdMessenger.sendCmd(kAcknowledge, "Tuning Start");
}

void OnStopTuning(){
  tuningState = READY;
  stopMotor();
}

void OnCalibrate(){
  // Calibrate Stuff
}

void OnSetTarget()
{
  String target = cmdMessenger.readStringArg();

  if (target == "E"){
    cmdMessenger.sendCmd(kAcknowledge, "Target: E String");
    setTuningTarget(STRING_E5);
  }
  else if(target == "A"){
    cmdMessenger.sendCmd(kAcknowledge, "Target: A String");
    setTuningTarget(STRING_A4);
  }
  else if(target == "D"){
    cmdMessenger.sendCmd(kAcknowledge, "Target: D String");
    setTuningTarget(STRING_D4);
  }
  else if(target == "G"){
    cmdMessenger.sendCmd(kAcknowledge, "Target: G String");
    setTuningTarget(STRING_G3);
  }else{
    cmdMessenger.sendCmd(kAcknowledge, "Unknown String");
  }
}

void OnSetTarget2(){
  FREQ_TARGET = cmdMessenger.readFloatArg();
  FREQ_BAND = cmdMessenger.readFloatArg();
}
