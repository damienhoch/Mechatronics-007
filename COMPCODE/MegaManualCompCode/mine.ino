bool mine() {
  MINEFAIL = false;
  if(position == 'T'){
    SENSING = false;
    blockColor = 'W';
    mineStrokes = numHitsWood[currentAxe];
  }
  while (SENSING) {
    blockColor = colorSense(colorPins, thresholdLow, thresholdHigh, readPin, numSamples);
    switch (blockColor) {
      case 'R':
        Serial.println("R");
        mineStrokes = numHitsIron[currentAxe];
        SENSING = false;
        break;
      case 'Y':
        Serial.println("Y");
        mineStrokes = numHitsStone[currentAxe];
        SENSING = false;
        break;
      case 'B':
        Serial.println("B");
        mineStrokes = numHitsDiamond[currentAxe];
        SENSING = false;
        break;
    }
  }
  if (mineStrokes <= 10) MINEABLE = true;
  else MINEABLE = false, mineStrokes = 10;
  if (TOWERDOWN) {
    MiningServo.write(mine1,255,true);
    for (int i = 0; i <= mineStrokes; i++) {
      MiningServo.write(mine2,255);
      delay(MineDelay);
      MiningServo.write(mine1,255);
      delay(MineDelay);
    }
    BLOCKMINED = true;
    tStart = millis();
    delay(250);
    LS1 = true;
    digitalWrite(BeltENA1, HIGH);
    digitalWrite(BeltENA2, LOW);
    analogWrite(BeltPWM, 255);
    while (LS1) {
      LS1 = digitalRead(LS1Pin);
      t = millis();
      if(t-tStart>loadTime){
        MINEFAIL = true;
        LS1 = true;
      }
    }
    delay(5);
    SilverFish = false;
    while (!MINEFAIL && LS1 == false) {
      LS1 = digitalRead(LS1Pin);
      if (NOSHIELD && blockColor != 'W') {
        for (int i = 0; i < 5; i++) {
          HeVals[i] = map(analogRead(A5), 0, 1023, 0, 500) / 100.0;
        }
        HeVal = movingAverage(HeVals, 5);
        if (abs(HeVal - quiescent) > HeThreshold) {
          SilverFish = true;
          Serial.println("Silverfish Detected");
        }
      }
    }
    digitalWrite(BeltENA1, LOW);
    digitalWrite(BeltENA2, LOW);
    analogWrite(BeltPWM, 0);
    if (SilverFish) {
      for (int i = 0; i < 10; i++) {
        MiningServo.write(mine2,255);
        delay(MineDelay);
        MiningServo.write(mine1,255);
        delay(MineDelay);
      }
    }
  }
  MiningServo.write(mineServoRest);
  SENSING = true;
  if(!MINEFAIL){
    currentBlocks[0] = blockColor;
    incrementLoaderArray();
    numBlocks = numBlocks + 1;
  }
  return MINEFAIL;
}