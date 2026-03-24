double lineFollow(char direction) {
  counts1 = leftEnc.read();
  counts2 = rightEnc.read();
  float d1 = (counts1) / (GearRatio * countsPerRev) * 2 * PI * rw;
  float d2 = (counts2) / (GearRatio * countsPerRev) * 2 * PI * rw;
  float dE = (d1 + d2) / 2.;  // Average elapsed distance
  switch (direction) {
    case 'f':
      FWDCAP = true;
      digitalWrite(RENAF, HIGH);
      qtrF.read(sensorValues);
      SumAi = 0;
      SumAiDi = 0;
      for (int i = 0; i < 8; i++) {
        Sensor_value_unbiased[i] = sensorValues[i] - sensor_biasF[i];
        SumAi = SumAi + Sensor_value_unbiased[i];
        SumAiDi = SumAiDi + (Sensor_value_unbiased[i] * ((i)*0.8) + .5);
      }
      d = SumAiDi / SumAi;
      Error = d0 - d;
      // Rangefinder
      RFdistance = rangefinderDistance(analogRead(RFRF), beta, gamma);
      if (RFdistance <= CraftApproachStop) {
        FWDCAP = false;
        REVCAP = true;
      }
      if (RFdistance <= CraftApproachSlow) {
        speedRadius = 50;
      }
      if (FWDCAP) {
        md.setM1Speed(speedRadius - (Kp * Error));
        md.setM2Speed(speedRadius + (Kp * Error));
      } else {
        Command = 0;
        digitalWrite(RENAF, LOW);
        md.setSpeeds(0, 0);
        DRIVING = false;
        PREDRIVING = true;
      }
      break;
    case 'r':
      REVCAP = true;
      digitalWrite(RENAR, HIGH);
      qtrR.read(sensorValues);
      SumAi = 0;
      SumAiDi = 0;
      for (int i = 0; i < 8; i++) {
        Sensor_value_unbiased[i] = sensorValues[i] - sensor_biasR[i];
        SumAi = SumAi + Sensor_value_unbiased[i];
        SumAiDi = SumAiDi + (Sensor_value_unbiased[i] * ((i)*0.8) + .5);
      }
      d = SumAiDi / SumAi;
      Error = d0 - d;
      // Rangefinder
      RFdistance = rangefinderDistance(analogRead(RFRR), beta, gamma);
      Serial.print("Range: ");
      Serial.print(RFdistance);
      Serial.println(" cm");
      if (RFdistance <= MineApproachStop) {
        FWDCAP = true;
        REVCAP = false;
      }
      if (RFdistance <= MineApproachSlow) {
        speedRadius = 50;
      }
      if (REVCAP) {
        md.setM1Speed(-speedRadius - (Kp * Error));
        md.setM2Speed(-speedRadius + (Kp * Error));
      } else {
        Command = 0;
        digitalWrite(RENAR, LOW);
        md.setSpeeds(0, 0);
        DRIVING = false;
        PREDRIVING = true;
      }
      break;
  }
  return dE;
}