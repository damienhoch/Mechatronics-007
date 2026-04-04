bool driveDistance(char dir, double dist, double speed, bool VERBOSE) {
  bool DONE = false;
  bool SLOWING = false;
  if (PREDRIVING) {
    //Direction
    if (dist == 0) {
      if (VERBOSE) Serial3.println("DRIVING UNTIL RANGEFINDER DETECTS PREBONK CONDITION");
      DRIVINGDISTANCEUNTILRANGEFINDER = true;
      dist = 1000;
    } else {
      DRIVINGDISTANCEUNTILRANGEFINDER = false;
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
    dErrordt1 = 0;
    dErrordt2 = 0;

    // Calculate Trajectories
    theta1_final = dist / rw;
    theta2_final = dist / rw;
    omega1_des = fabs(theta1_final * speed / dist) * (dist > 0 ? 1. : -1.);
    omega2_des = fabs(theta2_final * speed / dist) * (dist > 0 ? 1. : -1.);
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
    // Rangefinder
    switch (dir) {
      case 'f':
      case 'F':
        RFdistance = rangefinderDistance(analogRead(RFRF), beta, gamma);
        if (RFdistance <= CraftApproachStop) {
          DRIVING = false;
          DRIVINGDISTANCEUNTILRANGEFINDER = false;
          md.setSpeeds(0, 0);
        }
        if (RFdistance <= CraftApproachSlow) {
          SLOWING = true;
        }
        break;
      case 'r':
      case 'R':
        RFdistance = rangefinderDistance(analogRead(RFRR), beta, gamma);
        if (RFdistance <= MineApproachStop) {
          DRIVING = false;
          DRIVINGDISTANCEUNTILRANGEFINDER = false;
          md.setSpeeds(0, 0);
        }
        if (RFdistance <= MineApproachSlow) {
          SLOWING = true;
        }
        break;
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
    dErrordt1 = omega1_des - omega1f;
    dErrordt2 = omega2_des - omega2f;
    integral1 += (theta1_des - theta1) * deltaT;
    integral2 += (theta2_des - theta2) * deltaT;

    V1m = KpL * (theta1_des - theta1) + Ki * integral1 + Kd * dErrordt1;
    V2m = KpL * (theta2_des - theta2) + Ki * integral2 + Kd * dErrordt2;

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
    

    md.setSpeeds(m1c, m2c);  // send motor commands

    // Check if we've arrived
    if(DRIVING) DRIVING = ((fabs(theta1 - theta1_final) <= distance_arrival_threshold) && (fabs(theta2 - theta2_final) <= distance_arrival_threshold)) ? false : true;  // If both wheel are close enough, set DRIVING to false

    if (VERBOSE && (t - print_time) > 0.25) {  // non-blocking way to delay printing
      // print any variables of interest inside this if statement
      Serial3.println(RFdistance);
      Serial3.print("Driving distance: ");
      Serial3.println(dist);
      Serial3.print(counts1);
      Serial3.print(' ');
      Serial3.print(theta1);
      Serial3.print(' ');
      Serial3.print(theta1_des);
      Serial3.print('\t');
      Serial3.print(counts2);
      Serial3.print(' ');
      Serial3.print(theta2);
      Serial3.print(' ');
      Serial3.println(theta2_des);

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