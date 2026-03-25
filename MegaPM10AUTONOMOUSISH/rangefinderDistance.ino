float rangefinderDistance(float AnalogIn, float beta, float gamma) {
  // 
  float Voltage = (AnalogIn / 1024.) * 5.; // Convert from ADC input to voltage
  return beta * pow(Voltage, gamma); // Convert voltage to distance (cm) using the calibration equation
}