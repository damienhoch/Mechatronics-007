bool lineFollow(char dir, double distAngle, double speedRaw) {
  bool DONE = false;
  if (PREDRIVING) {
    leftEnc.write(0);
    rightEnc.write(0);
    elapsed_distance = 0;
    PREDRIVING = false;
    DRIVING = true;
    distAngle = (distAngle == 0) ? 1000 : distAngle;  // If distance is 0, line follow until rangedinders detect wall
    theta1_final = distAngle / rw;
  }
  if (DRIVING && (elapsed_distance - distAngle <= arrival_threshold)) {
    double speed = speedRaw*10.;
    counts1 = leftEnc.read();
    counts2 = rightEnc.read();
    float d1 = (counts1) / (GearRatio * countsPerRev) * 2 * PI * rw;
    float d2 = (counts2) / (GearRatio * countsPerRev) * 2 * PI * rw;
    if (DEBUGMODE){
      Serial.print(d1);
      Serial.print("\t");
      Serial.println(d2);
    }
    elapsed_distance = fabs((d1 + d2) / 2.);  // Average elapsed distance
    switch (dir) {
      case 'F':
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
          speed = 50;
        }
        if (FWDCAP) {
          md.setM1Speed(speed - (Kp * Error));
          md.setM2Speed(speed + (Kp * Error));
        } else {
          digitalWrite(RENAF, LOW);
          md.setSpeeds(0, 0);
          DRIVING = false;
          PREDRIVING = true;
          DONE = true;
        }
        break;
      case 'r':
      case 'R':
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
        if (DEBUGMODE) {
          Serial.print("Range: ");
          Serial.print(RFdistance);
          Serial.println(" cm");
        }
        if (RFdistance <= MineApproachStop) {
          FWDCAP = true;
          REVCAP = false;
        }
        if (RFdistance <= MineApproachSlow) {
          speed = 50;
        }
        if (REVCAP) {
          md.setM1Speed(-speed - (Kp * Error));
          md.setM2Speed(-speed + (Kp * Error));
        } else {
          digitalWrite(RENAR, LOW);
          md.setSpeeds(0, 0);
          DRIVING = false;
          PREDRIVING = true;
          DONE = true;
        }
        break;
    }
  } else {
    md.setSpeeds(0, 0);
    DRIVING = false;
    PREDRIVING = true;
    DONE = true;
  }
  return DONE;
}