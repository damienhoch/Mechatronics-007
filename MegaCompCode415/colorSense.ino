char colorSense(int colorPins[], int thresholdLow[][3], int thresholdHigh[][3], int readPin, int numSamples) {
  //
  int tries = 0;
  bool Discrepencies = false;
  float R[numSamples], G[numSamples], B[numSamples], C[numSamples];  // raw pulse time samples
  float RF, GF, BF, CF;                                              // filtered data
  float freq[4] = { 0, 0, 0, 0 };
  float freqNorm[3] = { 0, 0, 0 };
  char colorSensed;
  digitalWrite(colorPins[0], HIGH);  // Set frequency scaling
  digitalWrite(colorPins[1], LOW);
  //////// Take specified number of samples
  // Select RED Filter
  digitalWrite(colorPins[2], LOW);
  digitalWrite(colorPins[3], LOW);
  delay(10);
  for (int i = 0; i < numSamples; i++) {
    R[i] = pulseIn(readPin, LOW);
  }

  // Select BLUE Filter
  digitalWrite(colorPins[2], LOW);
  digitalWrite(colorPins[3], HIGH);
  delay(10);
  for (int i = 0; i < numSamples; i++) {
    B[i] = pulseIn(readPin, LOW);
  }

  // Select GREEN Filter
  digitalWrite(colorPins[2], HIGH);
  digitalWrite(colorPins[3], HIGH);
  delay(10);
  for (int i = 0; i < numSamples; i++) {
    G[i] = pulseIn(readPin, LOW);
  }

  // Select CLEAR Filter
  digitalWrite(colorPins[2], HIGH);
  digitalWrite(colorPins[3], LOW);
  delay(10);
  for (int i = 0; i < numSamples; i++) {
    C[i] = pulseIn(readPin, LOW);
  }

  RF = movingAverage(R, numSamples);
  GF = movingAverage(G, numSamples);
  BF = movingAverage(B, numSamples);
  CF = movingAverage(C, numSamples);

  freq[0] = 10000 / RF;
  freq[1] = 10000 / GF;
  freq[2] = 10000 / BF;
  freq[3] = 10000 / CF;
  for (int i = 0; i < 3; i++) {
    freqNorm[i] = 100 * (freq[i] / freq[3]);
  }
  if ((freqNorm[0] <= thresholdHigh[0][0]) && (freqNorm[0] >= thresholdLow[0][0]) && (freqNorm[1] <= thresholdHigh[0][1]) && (freqNorm[2] <= thresholdHigh[0][2])) {
    colorSensed = 'R';
    Serial3.println("Red Block Deteced once");
  } else if ((freqNorm[0] <= thresholdHigh[1][0]) && (freqNorm[0] >= thresholdLow[1][0]) && (freqNorm[1] <= thresholdHigh[1][1]) && (freqNorm[1] >= thresholdLow[1][1]) && (freqNorm[2] <= thresholdHigh[1][2]) && (freqNorm[2] >= thresholdLow[1][2])) {
    colorSensed = 'Y';
    Serial3.println("Yellow Block detected once");
  } else if ((freqNorm[0] <= thresholdHigh[2][0]) && (freqNorm[1] <= thresholdHigh[2][1]) && (freqNorm[2] <= thresholdHigh[2][2]) && (freqNorm[2] >= thresholdLow[2][2])) {
    colorSensed = 'B';
    Serial3.println("Blue Block detected once");
  }
  else{
    colorSensed = 'N';
  }
  if(COLORDEBUGMODE){
    Serial3.print(freqNorm[0]);
    Serial3.print(freqNorm[1]);
    Serial3.println(freqNorm[2]);
  }

  return colorSensed;
}