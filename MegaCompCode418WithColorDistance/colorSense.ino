char colorSense(int colorPins[], int readPin, int numSamples) {
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

  colorSensed = 'N';
  float minDistance = 1000;
  // Distance to red block
   distRed = 0;
  for (int i = 0; i < 4; i++) {
    distRed += pow(freqNorm[i] - redBlock[i], 2);
  }
  distRed = pow(distRed, 0.5);
  if (distRed < minDistance) {
    minDistance = distRed;
    colorSensed = 'R';
  }

  // Distance to blue block
   distBlue = 0;
  for (int i = 0; i < 4; i++) {
    distBlue += pow(freqNorm[i] - blueBlock[i], 2);
  }
  distBlue = pow(distBlue, 0.5);
  if (distBlue < minDistance) {
    minDistance = distBlue;
    colorSensed = 'B';
  }

  // Distance to yellow block
  distYellow = 0;
  for (int i = 0; i < 4; i++) {
    distYellow += pow(freqNorm[i] - yellowBlock[i], 2);
  }
  distYellow = pow(distYellow, 0.5);
  if (distYellow < minDistance) {
    minDistance = distYellow;
    colorSensed = 'Y';
  }
  // Distance to null block
   distNull = 0;
  for (int i = 0; i < 4; i++) {
    distNull += pow(freqNorm[i] - nullBlock[i], 2);
  }
  distNull = pow(distNull, 0.5);
  if (distNull < minDistance || freq[3] < 50) {
    minDistance = distNull;
    colorSensed = 'N';
  }

  return colorSensed;
}
