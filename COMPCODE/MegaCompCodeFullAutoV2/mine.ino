bool mine() {
  MINEFAIL = false;
  if (position == 'T' || position == 'A') {
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
  else {
    MINEABLE = false, mineStrokes = 10;
    blockColor = 'X';  // Block destroyed, need to remove
  }
  if (TOWERDOWN) {
    MiningServo.write(mine1, 255, true);
    for (int i = 0; i <= mineStrokes; i++) {
      MiningServo.write(mine2, 255);
      delay(MineDelay);
      MiningServo.write(mine1, 255);
      delay(MineDelay);
    }
    BLOCKMINED = true;
    delay(500);
    tStart = millis();
    LS1 = false;
    digitalWrite(BeltENA1, HIGH);
    digitalWrite(BeltENA2, LOW);
    analogWrite(BeltPWM, 255);
    unsigned long tStart_load = millis();
    while (!LS1) {
      LS1 = digitalRead(LS1Pin);
    }
    while (LS1) {
      LS1 = digitalRead(LS1Pin);
      if (millis() - tStart_load > loadTime) {
        break;
        //MINEFAIL = true;
        //missLoad();
      }
    }
    delay(5);
    SilverFish = false;
    unsigned long tStart_Hall = millis();
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
      if (millis() - tStart_Hall > loadDelay) break;
    }
    digitalWrite(BeltENA1, LOW);
    digitalWrite(BeltENA2, LOW);
    analogWrite(BeltPWM, 0);
    if (SilverFish) {
      for (int i = 0; i < 10; i++) {
        MiningServo.write(mine2, 255);
        delay(MineDelay);
        MiningServo.write(mine1, 255);
        delay(MineDelay);
      }
    }
    MiningServo.write(mineServoRest);
    SENSING = true;
    if (!MINEFAIL) {
      currentBlocks[numBlocks] = blockColor;
      numBlocks = numBlocks + 1;
      if (DEBUGMODE) Serial.print(currentBlocks);
    }
    return MINEFAIL;
  }
}