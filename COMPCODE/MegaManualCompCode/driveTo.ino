bool driveTo(char destination) {
  bool DONEDrive = false;
  if (DEBUGMODE) Serial.println("Driving Called");
  switch (position) {                                  // Switch on the current position
    case 'S':                                          // Start
      if (destination == 'M' || destination == 'm') {  // Drive to Mine from Start
        if (drive_to_substate == 0) {
          if (DEBUGMODE) Serial.println("Driving to Mine");
          if (lineFollow('R', 1000, autonomous_speed)) {
            position = 'M';
            DONEDrive = true;
            drive_to_substate = 0;
            if (DEBUGMODE) Serial.println(position);
          }
        }
      }
      break;
    case 'M':                                          // Mine
      if (destination == 't' || destination == 'T') {  // Drive to Tree from Mine
        if (drive_to_substate == 0) {
          if (lineFollow('F', 30., autonomous_speed)) drive_to_substate = 1;
        } else if (drive_to_substate == 1) {
          if (turnDeg('L', 90., 0., DEBUGMODE)) drive_to_substate = 2;
        } else if (drive_to_substate == 2) {
          if (driveDistance('R', 60., 20., DEBUGMODE)) drive_to_substate = 3;
        } else if (drive_to_substate == 3) {
          if (turnDeg('R', 90., 0., DEBUGMODE)) drive_to_substate = 4;
        } else if (drive_to_substate == 4) {
          if (lineFollow('R', 1000., autonomous_speed)) {
            drive_to_substate = 0;
            DONEDrive = true;
            position = 'T';
          }
        }
      } else if (destination == 'c' || destination == 'C') {  // Drive to Crafting from Mine
        if (DEBUGMODE) Serial.println("Driving to Crafting from Mine");
        if (drive_to_substate == 0) {
          if (lineFollow('F', 30., autonomous_speed)) drive_to_substate = 1;
        } else if (drive_to_substate == 1) {
          if (turnDeg('L', 90., 0., DEBUGMODE)) drive_to_substate = 2;
        } else if (drive_to_substate == 2) {
          if (driveDistance('F', 30., autonomous_speed, DEBUGMODE)) drive_to_substate = 3;
        } else if (drive_to_substate == 3) {
          if (turnDeg('R', 90., 0., DEBUGMODE)) drive_to_substate = 4;
        } else if (drive_to_substate == 4) {
          if (lineFollow('F', 1000., autonomous_speed)) {
            drive_to_substate = 0;
            DONEDrive = true;
            position = 'C';
          }
        }
      } else if (destination == 'b' || destination == 'B') {  // Drive to CHest from Mine
        if (drive_to_substate == 0) {
          if (turnDeg('L', 90., 30., autonomous_speed)) drive_to_substate = 1;
        } else if (drive_to_substate == 1) {
          if (driveDistance('F', 92, autonomous_speed, DEBUGMODE)) drive_to_substate = 2;
        } else if (drive_to_substate == 2) {
          if (turnDeg('R', 90., 0., DEBUGMODE)) drive_to_substate = 3;
        } else if (drive_to_substate == 3) {
          if (lineFollow('F', 10., autonomous_speed)) drive_to_substate = 4;
        } else if (drive_to_substate == 4) {
          if (driveDistance('F', 0., autonomous_speed, DEBUGMODE)) {
            drive_to_substate = 0;
            DONEDrive = true;
            position = 'B';
          }
        }
      }
      break;
    case 'T':                                          // Tree
      if (destination == 'm' || destination == 'M') {  // Drive to Mine from Tree
        if (drive_to_substate == 0) {
          if (lineFollow('F', 30., autonomous_speed)) drive_to_substate = 1;
        } else if (drive_to_substate == 1) {
          if (turnDeg('R', 90., 0., DEBUGMODE)) drive_to_substate = 2;
        } else if (drive_to_substate == 2) {
          if (driveDistance('R', 60., autonomous_speed, DEBUGMODE)) drive_to_substate = 3;
        } else if (drive_to_substate == 3) {
          if (turnDeg('L', 90., 0., DEBUGMODE)) drive_to_substate = 4;
        } else if (drive_to_substate == 4) {
          if (lineFollow('R', 1000., autonomous_speed)) {
            drive_to_substate = 0;
            DONEDrive = true;
            position = 'M';
          }
        }
      } else if (destination == 'c' || destination == 'C') {  // Drive to Crafting from Tree
        if (drive_to_substate == 0) {
          if (turnDeg('L', 90., 30., autonomous_speed)) drive_to_substate = 1;
        } else if (drive_to_substate == 1) {
          if (driveDistance('F', 50, autonomous_speed, DEBUGMODE)) drive_to_substate = 2;
        } else if (drive_to_substate == 2) {
          if (turnDeg('R', 90., 0., DEBUGMODE)) drive_to_substate = 3;
        } else if (drive_to_substate == 3) {
          if (lineFollow('R', 10, autonomous_speed)) drive_to_substate = 4;
        } else if (drive_to_substate == 4) {
          if (lineFollow('F', 1000., autonomous_speed)) {
            drive_to_substate = 0;
            DONEDrive = true;
            position = 'C';
          }
        }
      }
      break;
    case 'C':                                          // Crafting
      if (destination == 'm' || destination == 'M') {  // Drive to Mine from Crafting
        if (drive_to_substate == 0) {
          if (lineFollow('R', 15, autonomous_speed)) drive_to_substate = 1;
        } else if (drive_to_substate == 1) {
          if (turnDeg('L', 90., 0., DEBUGMODE)) drive_to_substate = 2;
        } else if (drive_to_substate == 2) {
          if (driveDistance('R', 30., autonomous_speed, DEBUGMODE)) drive_to_substate = 3;
        } else if (drive_to_substate == 3) {
          if (turnDeg('R', 90., 0., DEBUGMODE)) drive_to_substate = 4;
        } else if (drive_to_substate == 4) {
          if (lineFollow('R', 1000., autonomous_speed)) {
            drive_to_substate = 0;
            DONEDrive = true;
            position = 'M';
          }
        }
      } else if (destination == 'b' || destination == 'B') {  // Drive to Chest from Crafting
        if (drive_to_substate == 0) {
          if (lineFollow('R', 60., autonomous_speed)) drive_to_substate = 1;
        } else if (drive_to_substate == 1) {
          if (turnDeg('R', 90., 0., DEBUGMODE)) drive_to_substate = 2;
        } else if (drive_to_substate == 2) {
          if (driveDistance('R', 92, autonomous_speed, DEBUGMODE)) drive_to_substate = 3;
        } else if (drive_to_substate == 3) {
          if (turnDeg('L', 90., 0., DEBUGMODE)) drive_to_substate = 4;
        } else if (drive_to_substate == 4) {
          if (lineFollow('F', 10., autonomous_speed)) drive_to_substate = 5;
        } else if (drive_to_substate == 5) {
          if (driveDistance('F', 0., autonomous_speed, DEBUGMODE)) {
            drive_to_substate = 0;
            DONEDrive = true;
            position = 'B';
          }
        }
      } else if (destination == 't' || destination == 'T') {  // Drive to Tree from Crafting
        if (drive_to_substate == 0) {
          if (lineFollow('R', 15, autonomous_speed)) drive_to_substate = 1;
        } else if (drive_to_substate == 1) {
          if (turnDeg('L', 90., 0., DEBUGMODE)) drive_to_substate = 2;
        } else if (drive_to_substate == 2) {
          if (driveDistance('R', 92, autonomous_speed, DEBUGMODE)) drive_to_substate = 3;
        } else if (drive_to_substate == 3) {
          if (turnDeg('R', 90., 0., DEBUGMODE)) drive_to_substate = 4;
        } else if (drive_to_substate == 4) {
          if (lineFollow('R', 1000., autonomous_speed)) {
            drive_to_substate = 0;
            DONEDrive = true;
            position = 'T';
          }
        }
      }
      break;
    case 'B':                                          // Chest
      if (destination == 'm' || destination == 'M') {  // Drive to Mine from Chest
        if (drive_to_substate == 0) {
          if (driveDistance('R', 35, autonomous_speed, DEBUGMODE)) drive_to_substate = 1;
        } else if (drive_to_substate == 1) {
          if (lineFollow('R', 10., autonomous_speed)) drive_to_substate = 2;
        } else if (drive_to_substate == 2) {
          if (turnDeg('L', 90., 0., DEBUGMODE)) drive_to_substate = 3;
        } else if (drive_to_substate == 3) {
          if (driveDistance('R', 122, autonomous_speed, DEBUGMODE)) drive_to_substate = 4;
        } else if (drive_to_substate == 4) {
          if (turnDeg('R', 90., 0., DEBUGMODE)) drive_to_substate = 5;
        } else if (drive_to_substate == 5) {
          if (lineFollow('R', 1000., autonomous_speed)) {
            drive_to_substate = 0;
            DONEDrive = true;
            position = 'M';
          }
        }
      } else if (destination == 'c' || destination == 'C') {  // Drive to Crafting from Chest
        if (drive_to_substate == 0) {
          if (driveDistance('R', 35, autonomous_speed, DEBUGMODE)) drive_to_substate = 1;
        } else if (drive_to_substate == 1) {
          if (lineFollow('R', 5, autonomous_speed)) drive_to_substate = 2;
        } else if (drive_to_substate == 2) {
          if (turnDeg('R', 90., 0., DEBUGMODE)) drive_to_substate = 3;
        } else if (drive_to_substate == 3) {
          if (driveDistance('F', 92, autonomous_speed, DEBUGMODE)) drive_to_substate = 4;
        } else if (drive_to_substate == 4) {
          if (turnDeg('L', 90., 0., DEBUGMODE)) drive_to_substate = 5;
        } else if (drive_to_substate == 5) {
          if (lineFollow('F', 1000., autonomous_speed)) {
            drive_to_substate = 0;
            DONEDrive = true;
            position = 'C';
          }
        }
      } else if (destination == 't' || destination == 'T') {  // Drive to Tree from Chest
        if (drive_to_substate == 0) {
          if (driveDistance('R', 35, autonomous_speed, DEBUGMODE)) drive_to_substate = 1;
        } else if (drive_to_substate == 1) {
          if (lineFollow('R', 10., autonomous_speed)) drive_to_substate = 2;
        } else if (drive_to_substate == 2) {
          if (turnDeg('L', 90., 0., DEBUGMODE)) drive_to_substate = 3;
        } else if (drive_to_substate == 3) {
          if (driveDistance('R', 183, autonomous_speed, DEBUGMODE)) drive_to_substate = 4;
        } else if (drive_to_substate == 4) {
          if (turnDeg('R', 90., 0., DEBUGMODE)) drive_to_substate = 5;
        } else if (drive_to_substate == 5) {
          if (lineFollow('R', 1000., autonomous_speed)) {
            drive_to_substate = 0;
            DONEDrive = true;
            position = 'T';
          }
        }
      }
      break;
  }
  return DONEDrive;
}