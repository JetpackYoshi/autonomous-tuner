% Audio Input Settings
deviceNum = 0;
sampleRate = 48000;
bitsPerSample = 16;
numAudioChannels = 1;

info = audiodevinfo;
inputDevices = struct2table(info.input);
recorder = audiorecorder(sampleRate,bitsPerSample,numAudioChannels,deviceNum);