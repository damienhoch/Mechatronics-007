bool driveDistance(char dir, double dist, double speed, bool VERBOSE) {
  bool DONE = false;
  bool SLOWING = false;
  if (PREDRIVING) {
    //Direction
    if (dist == 0) {
      DRIVINGDISTANCEUNTILRANGEFINDER = true;
      dist = 1000;
    }
    dist = ((dir == 'f') || (dir == 'F')) ? (dist) : (-dist);

    // Zero
    leftEnc.write(0);
    rightEnc.write(0);
    elapsed_distance = 0;
    theta1_old = 0;
    theta2_old = 0;
    omega1fOld = 0;
    omega2fOld = 0;
    theta1_des = 0;
    theta2_des = 0;
    print_time = 0;
    integral1 = 0;
    integral2 = 0;
    // Calculate Trajectories
    theta1_final = dist / rw;
    theta2_final = dist / rw;
    omega1_des = fabs(theta1_final * speed / dist) * (dist > 0 ? 1 : -1);
    omega2_des = fabs(theta2_final * speed / dist) * (dist > 0 ? 1 : -1);
    t0 = micros() / 1000000.;
    t_old = 0.;
    // Give the trajectory a head start so error != 0 immediately
    theta1_des = omega1_des * 0.02;  // seed with one ~20ms step
    theta2_des = omega2_des * 0.02;
    // Bools
    PREDRIVING = false;
    DRIVING = true;
  }
  if (DRIVING) {
    float V1m, V2m;
    float deltaT;
    if (DRIVINGDISTANCEUNTILRANGEFINDER) {
      // Rangefinder
      RFdistance = rangefinderDistance(analogRead(RFRF), beta, gamma);
      if (RFdistance <= CraftApproachStop) {
        DRIVING = false;
        DRIVINGDISTANCEUNTILRANGEFINDER = false;
      }
      if (RFdistance <= CraftApproachSlow) {
        SLOWING = true;
      }
    }

    t = micros() / 1000000. - t0;
    deltaT = t - t_old;  // sample time

    counts1 = leftEnc.read();
    counts2 = rightEnc.read();

    // calculate your position and velocity here
    theta1 = (counts1) / (GearRatio * countsPerRev) * 2 * PI;
    omega1 = (theta1 - theta1_old) / deltaT;
    omega1f = (omega1 * alpha) + ((1 - alpha) * omega1fOld);
    theta2 = (counts2) / (GearRatio * countsPerRev) * 2 * PI;
    omega2 = (theta2 - theta2_old) / deltaT;
    omega2f = (omega2 * alpha) + ((1 - alpha) * omega2fOld);
    // add your trajectory design here
    if (fabs(theta1_des) < fabs(theta1_final)) {
      theta1_des += omega1_des * deltaT;
    }
    if (fabs(theta2_des) < fabs(theta2_final)) {
      theta2_des += omega2_des * deltaT;
    }
    // add your control laws here
    integral1 += (theta1_des - theta1) * deltaT;
    integral2 += (theta2_des - theta2) * deltaT;

    V1m = KpL * (theta1_des - theta1) + Ki * integral1;
    V2m = KpL * (theta2_des - theta2) + Ki * integral2;

    // Uncomment these four lines in section 4.4
    if (SLOWING) {
      V1m = constrain(V1m, -5, 5);
      V2m = constrain(V2m, -5, 5);
    } else {
      V1m = constrain(V1m, -10, 10);
      V2m = constrain(V2m, -10, 10);
    }
    m1c = 400. * V1m / 10.;
    m2c = 400. * V2m / 10.;
    if (fabs(theta1 - theta1_final) > arrival_threshold) {
      if (m1c > 0 && m1c < min_motor_speed) m1c = min_motor_speed;
      if (m1c < 0 && m1c > -min_motor_speed) m1c = -min_motor_speed;
    }
    if (fabs(theta2 - theta2_final) > arrival_threshold) {
      if (m2c > 0 && m2c < min_motor_speed) m2c = min_motor_speed;
      if (m2c < 0 && m2c > -min_motor_speed) m2c = -min_motor_speed;
    }

    md.setSpeeds(m1c, m2c);  // send motor commands

    // Check if we've arrived
    DRIVING = ((fabs(theta1 - theta1_final) <= arrival_threshold) && (fabs(theta2 - theta2_final) <= arrival_threshold)) ? false : true;  // If both wheel are close enough, set DRIVING to false

    if (VERBOSE && (t - print_time) > 0.25) {  // non-blocking way to delay printing
      // print any variables of interest inside this if statement
      Serial.print("Driving distance: ");
      Serial.println(dist);
      Serial.print(counts1);
      Serial.print(' ');
      Serial.print(theta1);
      Serial.print(' ');
      Serial.print(theta1_des);
      Serial.print('\t');
      Serial.print(counts2);
      Serial.print(' ');
      Serial.print(theta2);
      Serial.print(' ');
      Serial.println(theta2_des);

      print_time = t;
    }

    t_old = t;
    theta1_old = theta1;
    theta2_old = theta2;
    omega1fOld = omega1f;
    omega2fOld = omega2f;
  } else {
    md.setSpeeds(0, 0);
    PREDRIVING = true;
    DONE = true;
  }
  return DONE;
}