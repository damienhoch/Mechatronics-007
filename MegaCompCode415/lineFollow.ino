bool lineFollow(char dir, double distAngle, double speedRaw) {
  bool DONE = false;
  if (PREDRIVING) {
    //if (position == 'C' || position == 'c') {
    //KiLF = 0;
    //KdLF = 0;
    //} else {
    //KiLF = 200;
    //KdLF = 0.9;
    //}
    leftEnc.write(0);
    rightEnc.write(0);
    elapsed_distance = 0;
    PREDRIVING = false;
    DRIVING = true;
    distAngle = (distAngle == 0) ? 1000 : distAngle;  // If distance is 0, line follow until rangedinders detect wall
    theta1_final = distAngle / rw;
    integral1 = 0;
    dErrordt1 = 0;
    t0 = micros() / 1000.;
    t_old = t0;
    errorOld = 0;
  }
  if (DRIVING && (elapsed_distance - distAngle <= arrival_threshold)) {
    double speed = speedRaw * 10.;
    t = micros() / 1000000. - t0;
    deltaT = t - t_old;
    counts1 = leftEnc.read();
    counts2 = rightEnc.read();
    float d1 = (counts1) / (GearRatio * countsPerRev) * 2 * PI * rw;
    float d2 = (counts2) / (GearRatio * countsPerRev) * 2 * PI * rw;
    if (DEBUGMODE) {
      //Serial3.print(d1);
      //Serial3.print("\t");
      //Serial3.println(d2);
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
        dErrordt1 = (Error - errorOld) / deltaT;
        if ((Error > 0 && errorOld < 0) || (Error < 0 && errorOld > 0)) {
          integral1 = 0;
        }
        integral1 += Error * deltaT;
        integral1 = constrain(integral1, -0.5, 0.5);  // prevent windup
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
          md.setM1Speed(speed - Kp * Error - KiLF * integral1 - KdLF * dErrordt1);
          md.setM2Speed(speed + Kp * Error + KiLF * integral1 + KdLF * dErrordt1);
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
        dErrordt1 = (Error - errorOld) / deltaT;
        if ((Error > 0 && errorOld < 0) || (Error < 0 && errorOld > 0)) {
          integral1 = 0;
        }
        integral1 += Error * deltaT;
        integral1 = constrain(integral1, -0.5, 0.5);  // prevent windup
        // Rangefinder
        RFdistance = rangefinderDistance(analogRead(RFRR), beta, gamma);
        if (DEBUGMODE) {
          //Serial3.print("Range: ");
          //Serial3.print(RFdistance);
          //Serial3.println(" cm");
        }
        if (RFdistance <= MineApproachStop) {
          FWDCAP = true;
          REVCAP = false;
        }
        if (RFdistance <= MineApproachSlow) {
          speed = 50;
        }
        if (REVCAP) {
          md.setM1Speed(-speed - Kp * Error - KiLF * integral1 - KdLF * dErrordt1);
          md.setM2Speed(-speed + Kp * Error + KiLF * integral1 + KdLF * dErrordt1);
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
  t_old = t;
  errorOld = Error;
}