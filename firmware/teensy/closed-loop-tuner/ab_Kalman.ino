float R = 6.1E-3;  // Observation Noise Covariance
float Q = 7e-4;    // Process Noise Covariance
float Pc = 0.0;
float G = 0.0;     // Kalman Gain
float P = 1.0;
float Xp = 0.0;
float Zp = 0.0;
float Xe = 0.0;

float StepFilter(float measured_pitch){
  // Shift Calculated Predictions
  Xp = Xe;
  Zp = Xp;

  // Calculate PC covariance
  Pc = P + Q;

  // Calculate Gain
  G = Pc / (Pc + R);
  P = (1-G)* Pc;

  // Calculate Estimate
  Xe = G*(measured_pitch-Zp) + Xp;

  return Xe;
}
