void mine(void){
  while (SENSING) {
        blockColor = colorSense(colorPins, thresholdLow, thresholdHigh, readPin, numSamples);
        switch (blockColor) {
          case 'R':
            Serial.println("R");
            mineStrokes = numHitsWood[2];
            SENSING = false;
            break;
          case 'Y':
            Serial.println("Y");
            mineStrokes = numHitsWood[1];
            SENSING = false;
            break;
          case 'B':
            Serial.println("B");
            mineStrokes = numHitsWood[3];
            SENSING = false;
            break;
        }
      }
      if (TOWERDOWN) {
        MiningServo.write(mine1);
        delay(125);
        for (int i = 0; i <= mineStrokes; i++) {
          MiningServo.write(mine2);
          delay(MineDelay);
          MiningServo.write(mine1);
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
        }
        delay(5);
        SilverFish = false;
        while (LS1 == false) {
          LS1 = digitalRead(LS1Pin);
          for (int i=0; i<5;i++){
            HeVals[i] = map(analogRead(A5), 0, 1023, 0, 500) / 100.0;
          }
          HeVal = movingAverage(HeVals, 5);
          if (DEBUGMODE) Serial.println(HeVal);
          if (abs(HeVal-quiescent) > HeThreshold) {
            SilverFish = true;
            Serial.println("Silverfish Detected");
          }
        }
        digitalWrite(BeltENA1, LOW);
        digitalWrite(BeltENA2, LOW);
        analogWrite(BeltPWM, 0);
        if (SilverFish) {
          for (int i = 0; i < 10; i++) {
            MiningServo.write(mine2);
            delay(MineDelay);
            MiningServo.write(mine1);
            delay(MineDelay);
          }
        }
      }
      MiningServo.write(mineServoRest);
      Command = 0;
      SENSING = true;
}