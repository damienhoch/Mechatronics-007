bool mine() {
  LS1 = true;
  MINESUCCESS = true;
  int Ls1Old = true;
  bool SwitchRead = true;
  bool FIRSTTIME = true;
  if (position == 'T' || position == 'A') {
    SENSING = false;
    blockColor = 'W';
    mineStrokes = numHitsWood[currentAxe];
  }
  while (!lineFollow('R', 10, 10)) {
  }
  while (SENSING) {
    blockColor = colorSense(colorPins, thresholdLow, thresholdHigh, readPin, numSamples);
    switch (blockColor) {
      case 'R':
        Serial3.println("R");
        mineStrokes = numHitsIron[currentAxe];
        SENSING = false;
        break;
      case 'Y':
        Serial3.println("Y");
        mineStrokes = numHitsStone[currentAxe];
        SENSING = false;
        break;
      case 'B':
        Serial3.println("B");
        mineStrokes = numHitsDiamond[currentAxe];
        SENSING = false;
        break;
    }
  }
  if (mineStrokes <= 11) MINEABLE = true;
  else {
    MINEABLE = false, mineStrokes = 11;
    blockColor = 'X';  // Block destroyed, need to remove
  }
  if (TOWERDOWN) {
    MiningServo.write(mine2, 255, true);
    for (int i = 0; i <= mineStrokes; i++) {
      MiningServo.write(mine1, 255);
      delay(MineDelay);
      MiningServo.write(mine2, 255);
      delay(MineDelay);
    }
    BLOCKMINED = true;
    delay(250);
    tStart = millis();
    LS1 = false;
    digitalWrite(BeltENA1, HIGH);
    digitalWrite(BeltENA2, LOW);
    analogWrite(BeltPWM, 255);
    unsigned long tStart_load = millis();
    while (SwitchRead) {
      LS1 = digitalRead(LS1Pin);
      Ls1Old = LS1;
      if (LS1) {
        delay(50);
        LS1 = digitalRead(LS2Pin);
        if (LS1 == Ls1Old) SwitchRead = false;
      }
    }
    while (!SwitchRead) {
      LS1 = digitalRead(LS1Pin);
      Ls1Old = LS1;
      if (millis() - tStart_load > loadTime) {
        if (FIRSTTIME) {
          Serial3.println("Correcting missload");
          digitalWrite(BeltENA1, LOW);
          digitalWrite(BeltENA2, LOW);
          analogWrite(BeltPWM, 0);
          TowerServo.write(towerServoTap);
          delay(MineDelay);
          TowerServo.write(towerServoRest);
          digitalWrite(BeltENA1, HIGH);
          digitalWrite(BeltENA2, LOW);
          analogWrite(BeltPWM, 255);
          FIRSTTIME = false;
          tStart_load = millis();
        } else {
          break;

          //MINEFAIL = true;
          //missLoad();
        }
      }
      if (!LS1) {
        delay(50);
        LS1 = digitalRead(LS1Pin);
        if (LS1 == Ls1Old) SwitchRead = true;
      }
    }
    delay(5);
    SilverFish = false;
    unsigned long tStart_Hall = millis();
    while (MINESUCCESS && LS1 == false) {
      LS1 = digitalRead(LS1Pin);
      if (NOSHIELD && blockColor != 'W') {
        for (int i = 0; i < 5; i++) {
          HeVals[i] = map(analogRead(A5), 0, 1023, 0, 500) / 100.0;
        }
        HeVal = movingAverage(HeVals, 5);
        if (abs(HeVal - quiescent) > HeThreshold) {
          SilverFish = true;
          Serial3.println("Silverfish Detected");
        }
      }
      if (millis() - tStart_Hall > loadDelay) {
        break;
      }
    }
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
    delay(1000);  // Delay from silverfish before mining
  }
  MiningServo.write(mineServoRest);
  TowerServo.write(towerServoRest);

  SENSING = true;
  if (MINESUCCESS) {
    currentBlocks[numBlocks] = blockColor;
    numBlocks = numBlocks + 1;
    if (DEBUGMODE) Serial3.print(currentBlocks);
  }
  if (position == 'T' || position == 'A') {
    delay(500);
  }
  return MINESUCCESS;
}
