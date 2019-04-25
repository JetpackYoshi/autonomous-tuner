enum
{
  kAcknowledge,    // 0   
  kGetStatus,      // 1
  kGetPitchData,   // 2
  kSetTarget,      // 3
  kBeginTuning,    // 4
  kStopTuning,     // 5
  kCalibrate,      // 6
  kToggleRawStream // 7
};

void sendHeader(){
  cmdMessenger.sendCmdArg(systemState);
  cmdMessenger.sendCmdArg(Error);
}
void attachCommandCallbacks()
{
  cmdMessenger.attach(kGetStatus, OnGetStatus);
  cmdMessenger.attach(kGetPitchData, OnGetPitchData);
  cmdMessenger.attach(kSetTarget, OnSetTarget);
  cmdMessenger.attach(kBeginTuning, OnBeginTuning);
  cmdMessenger.attach(kStopTuning, OnStopTuning);
  cmdMessenger.attach(kCalibrate, OnCalibrate);
  cmdMessenger.attach(kToggleRawStream, toggleRawStream);
  cmdMessenger.printLfCr();
}

void OnGetStatus(){
  // Returns the System State and Error
  cmdMessenger.sendCmdStart(kAcknowledge);
  sendHeader();
  cmdMessenger.sendCmdEnd();
}

void OnGetPitchData(){
  // Returns the setpoint pitch and the measured pitch
  cmdMessenger.sendCmdStart(kAcknowledge);
  sendHeader();
  cmdMessenger.sendCmdArg(FREQ_TARGET);
  cmdMessenger.sendCmdArg(filtered_note);
  cmdMessenger.sendCmdEnd();
}

void toggleRawStream(){
  // Toggles telemetry stream on and off
  stream = !stream;
  if (stream){
    cmdMessenger.sendCmdStart(kAcknowledge);
    sendHeader();
    cmdMessenger.sendCmdArg("Streaming On");
    cmdMessenger.sendCmdEnd();
  }else{
    cmdMessenger.sendCmdStart(kAcknowledge);
    sendHeader();
    cmdMessenger.sendCmdArg("Streaming Off");
    cmdMessenger.sendCmdEnd();
  }
}

void OnBeginTuning(){
  // Activate motor and start live tuning process
  systemState = TUNING;
  cmdMessenger.sendCmdStart(kAcknowledge);
  sendHeader();
  cmdMessenger.sendCmdArg("Tuning Start");
  cmdMessenger.sendCmdEnd();
}

void OnStopTuning(){
  // Stops the tuning process. Halts motor and returns system to READY state
  cmdMessenger.sendCmdStart(kAcknowledge);
  sendHeader();
  cmdMessenger.sendCmdArg("Stopped Tuning");
  cmdMessenger.sendCmdEnd();
  systemState = READY;
  stopMotor();
}

void OnCalibrate(){
  // Calibrate Stuff
}

void OnSetTarget()
  // Set the tuning profile via the letter of the note
{
  String target = cmdMessenger.readStringArg();

  if (target == "E"){
    cmdMessenger.sendCmdStart(kAcknowledge);
    sendHeader();
    cmdMessenger.sendCmdArg("Target: E String");
    cmdMessenger.sendCmdEnd();
    setTuningTarget(STRING_E5);
  }
  else if(target == "A"){
    cmdMessenger.sendCmdStart(kAcknowledge);
    sendHeader();
    cmdMessenger.sendCmdArg("Target: A String");
    cmdMessenger.sendCmdEnd();
    setTuningTarget(STRING_A4);
  }
  else if(target == "D"){
    cmdMessenger.sendCmdStart(kAcknowledge);
    sendHeader();
    cmdMessenger.sendCmdArg("Target: D String");
    cmdMessenger.sendCmdEnd();
    setTuningTarget(STRING_D4);
  }
  else if(target == "G"){
    cmdMessenger.sendCmdStart(kAcknowledge);
    sendHeader();
    cmdMessenger.sendCmdArg("Target: G String");
    cmdMessenger.sendCmdEnd();
    setTuningTarget(STRING_G3);
  }else{
    cmdMessenger.sendCmdStart(kAcknowledge);
    sendHeader();
    cmdMessenger.sendCmdArg("Unknown String");
    cmdMessenger.sendCmdEnd();
  }
}

void OnSetTarget2(){
  // Set the tuning profile via explicit pitch and band values
  FREQ_TARGET = cmdMessenger.readFloatArg();
  FREQ_BAND = cmdMessenger.readFloatArg();
  Setpoint = FREQ_TARGET;
}
