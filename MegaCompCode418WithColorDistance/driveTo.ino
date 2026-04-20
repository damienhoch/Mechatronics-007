bool driveTo(char destination) {
  bool DONEDrive = false;
  switch (position) {                                  // Switch on the current position
    case 'S':                                          // Start
      if (destination == 'M' || destination == 'm') {  // Drive to Mine from Start
        if (drive_to_substate == 0) {
          if (DEBUGMODE) Serial3.println("Driving to Mine");
          if (lineFollow('R', 1000, autonomous_speed)) {
            position = 'M';
            DONEDrive = true;
            drive_to_substate = 0;
            if (DEBUGMODE) Serial3.println(position);
          }
        }
      } else if (destination == 't' || destination == 'T') {
        if (drive_to_substate == 0) {
          if (lineFollow('R', 1000, autonomous_speed)) drive_to_substate = 1;
        } else if (drive_to_substate == 1) {
          if (lineFollow('F', 30., autonomous_speed)) drive_to_substate = 2;
        } else if (drive_to_substate == 2) {
          if (turnDeg('L', 90., 0., DEBUGMODE)) drive_to_substate = 3;
        } else if (drive_to_substate == 3) {
          if (driveDistance('R', 63., autonomous_speed, DEBUGMODE)) drive_to_substate = 4;
        } else if (drive_to_substate == 4) {
          if (turnDeg('R', 90., 0., DEBUGMODE)) drive_to_substate = 5;
        } else if (drive_to_substate == 5) {
          if (lineFollow('R', 1000., autonomous_approach_speed)) {
            drive_to_substate = 0;
            DONEDrive = true;
            position = 'T';
          }
        }
      } else if (destination == 'a' || destination == 'A') {  // Drive to Right Tree from Left Mine
        if (drive_to_substate == 0) {
          if (lineFollow('R', 1000, autonomous_speed)) drive_to_substate = 1;
        } else if (drive_to_substate == 1) {
          if (lineFollow('F', 30., autonomous_speed)) drive_to_substate = 2;
        } else if (drive_to_substate == 2) {
          if (turnDeg('L', 90., 0., DEBUGMODE)) drive_to_substate = 3;
        } else if (drive_to_substate == 3) {
          if (driveDistance('F', 126., autonomous_speed, DEBUGMODE)) drive_to_substate = 4;
        } else if (drive_to_substate == 4) {
          if (turnDeg('R', 90., 0., DEBUGMODE)) drive_to_substate = 5;
        } else if (drive_to_substate == 5) {
          if (lineFollow('R', 1000., autonomous_approach_speed)) {
            drive_to_substate = 0;
            DONEDrive = true;
            position = 'A';
          }
        }
      } else if (destination == 'k' || destination == 'K') {  // Drive to Right Mine from Left mine
        if (drive_to_substate == 0) {
          if (lineFollow('R', 1000, autonomous_speed)) drive_to_substate = 1;
        } else if (drive_to_substate == 1) {
          if (lineFollow('F', 30., autonomous_speed)) drive_to_substate = 2;
        } else if (drive_to_substate == 2) {
          if (turnDeg('L', 90., 0., DEBUGMODE)) drive_to_substate = 3;
        } else if (drive_to_substate == 3) {
          if (driveDistance('F', 63., autonomous_speed, DEBUGMODE)) drive_to_substate = 4;
        } else if (drive_to_substate == 4) {
          if (turnDeg('R', 90., 0., DEBUGMODE)) drive_to_substate = 5;
        } else if (drive_to_substate == 5) {
          if (lineFollow('R', 1000., autonomous_approach_speed)) {
            drive_to_substate = 0;
            DONEDrive = true;
            position = 'K';
          }
        }
      }
      break;
    case 'M':                                          // Left Mine
      if (destination == 't' || destination == 'T') {  // Drive to Left Tree from Left Mine
        if (drive_to_substate == 0) {
          if (lineFollow('F', 25., autonomous_speed)) drive_to_substate = 1;
        } else if (drive_to_substate == 1) {
          if (turnDeg('L', 90., 0., DEBUGMODE)) drive_to_substate = 2;
        } else if (drive_to_substate == 2) {
          if (driveDistance('R', 63., autonomous_speed, DEBUGMODE)) drive_to_substate = 3;
        } else if (drive_to_substate == 3) {
          if (turnDeg('R', 90., 0., DEBUGMODE)) drive_to_substate = 4;
        } else if (drive_to_substate == 4) {
          if (lineFollow('R', 1000., autonomous_approach_speed)) {
            drive_to_substate = 0;
            DONEDrive = true;
            position = 'T';
          }
        }
      } else if (destination == 'a' || destination == 'A') {  // Drive to Right Tree from Left Mine
        if (drive_to_substate == 0) {
          if (lineFollow('F', 25., autonomous_speed)) drive_to_substate = 1;
        } else if (drive_to_substate == 1) {
          if (turnDeg('L', 90., 0., DEBUGMODE)) drive_to_substate = 2;
        } else if (drive_to_substate == 2) {
          if (driveDistance('F', 126., autonomous_speed, DEBUGMODE)) drive_to_substate = 3;
        } else if (drive_to_substate == 3) {
          if (turnDeg('R', 90., 0., DEBUGMODE)) drive_to_substate = 4;
        } else if (drive_to_substate == 4) {
          if (lineFollow('R', 1000., autonomous_approach_speed)) {
            drive_to_substate = 0;
            DONEDrive = true;
            position = 'A';
          }
        }
      } else if (destination == 'k' || destination == 'K') {  // Drive to Right Mine from Left mine
        if (drive_to_substate == 0) {
          if (lineFollow('F', 25., autonomous_speed)) drive_to_substate = 1;
        } else if (drive_to_substate == 1) {
          if (turnDeg('L', 90., 0., DEBUGMODE)) drive_to_substate = 2;
        } else if (drive_to_substate == 2) {
          if (driveDistance('F', 63., autonomous_speed, DEBUGMODE)) drive_to_substate = 3;
        } else if (drive_to_substate == 3) {
          if (turnDeg('R', 90., 0., DEBUGMODE)) drive_to_substate = 4;
        } else if (drive_to_substate == 4) {
          if (lineFollow('R', 1000., autonomous_approach_speed)) {
            drive_to_substate = 0;
            DONEDrive = true;
            position = 'K';
          }
        }
      } else if (destination == 'c' || destination == 'C') {  // Drive to Crafting from Left Mine
        if (DEBUGMODE) Serial3.println("Driving to Crafting from Mine");
        if (drive_to_substate == 0) {
          if (lineFollow('F', 25., autonomous_speed)) drive_to_substate = 1;
        } else if (drive_to_substate == 1) {
          if (turnDeg('L', 90., 0., DEBUGMODE)) drive_to_substate = 2;
        } else if (drive_to_substate == 2) {
          if (driveDistance('F', 33., autonomous_speed, DEBUGMODE)) drive_to_substate = 3;
        } else if (drive_to_substate == 3) {
          if (turnDeg('R', 90., 0., DEBUGMODE)) drive_to_substate = 4;
        } else if (drive_to_substate == 4) {
          if (lineFollow('R', 5, 10)) drive_to_substate = 5;
        } else if (drive_to_substate == 5) {
          if (lineFollow('F', 1000., autonomous_approach_speed)) {
            drive_to_substate = 0;
            DONEDrive = true;
            position = 'C';
          }
        }
      } else if (destination == 'b' || destination == 'B') {  // Drive to CHest from Left Mine
        if (drive_to_substate == 0) {
          if (turnDeg('L', 90., 30., autonomous_speed)) drive_to_substate = 1;
        } else if (drive_to_substate == 1) {
          if (driveDistance('F', 92, autonomous_speed, DEBUGMODE)) drive_to_substate = 2;
        } else if (drive_to_substate == 2) {
          if (turnDeg('R', 90., 0., DEBUGMODE)) drive_to_substate = 3;
        } else if (drive_to_substate == 3) {
          if (driveDistance('F', 0., autonomous_speed, DEBUGMODE)) {
            drive_to_substate = 0;
            DONEDrive = true;
            position = 'B';
          }
        }
      }
      break;
    case 'T':                                          // Left Tree
      if (destination == 'm' || destination == 'M') {  // Drive to Left Mine from Left Tree
        if (drive_to_substate == 0) {
          if (lineFollow('F', 25., autonomous_speed)) drive_to_substate = 1;
        } else if (drive_to_substate == 1) {
          if (turnDeg('R', 90., 0., DEBUGMODE)) drive_to_substate = 2;
        } else if (drive_to_substate == 2) {
          if (driveDistance('R', 65., autonomous_speed, DEBUGMODE)) drive_to_substate = 3;
        } else if (drive_to_substate == 3) {
          if (turnDeg('L', 90., 0., DEBUGMODE)) drive_to_substate = 4;
        } else if (drive_to_substate == 4) {
          if (lineFollow('R', 1000., autonomous_approach_speed)) {
            drive_to_substate = 0;
            DONEDrive = true;
            position = 'M';
          }
        }
      } else if (destination == 'k' || destination == 'K') {  // Drive to right Mine from Left Tree
        if (drive_to_substate == 0) {
          if (lineFollow('F', 25., autonomous_speed)) drive_to_substate = 1;
        } else if (drive_to_substate == 1) {
          if (turnDeg('R', 90., 0., DEBUGMODE)) drive_to_substate = 2;
        } else if (drive_to_substate == 2) {
          if (driveDistance('R', 126., autonomous_speed, DEBUGMODE)) drive_to_substate = 3;
        } else if (drive_to_substate == 3) {
          if (turnDeg('L', 90., 0., DEBUGMODE)) drive_to_substate = 4;
        } else if (drive_to_substate == 4) {
          if (lineFollow('R', 1000., autonomous_approach_speed)) {
            drive_to_substate = 0;
            DONEDrive = true;
            position = 'K';
          }
        }
      } else if (destination == 'a' || destination == 'A') {  // Drive to right tree from Left Tree
        if (drive_to_substate == 0) {
          if (lineFollow('F', 25., autonomous_speed)) drive_to_substate = 1;
        } else if (drive_to_substate == 1) {
          if (turnDeg('R', 90., 0., DEBUGMODE)) drive_to_substate = 2;
        } else if (drive_to_substate == 2) {
          if (driveDistance('R', 185., autonomous_speed, DEBUGMODE)) drive_to_substate = 3;
        } else if (drive_to_substate == 3) {
          if (turnDeg('L', 90., 0., DEBUGMODE)) drive_to_substate = 4;
        } else if (drive_to_substate == 4) {
          if (lineFollow('R', 1000., autonomous_approach_speed)) {
            drive_to_substate = 0;
            DONEDrive = true;
            position = 'A';
          }
        }
      } else if (destination == 'c' || destination == 'C') {  // Drive to Crafting from Left Tree
        if (drive_to_substate == 0) {
          if (turnDeg('L', 90., 30., autonomous_speed)) drive_to_substate = 1;
        } else if (drive_to_substate == 1) {
          if (driveDistance('F', 50, autonomous_speed, DEBUGMODE)) drive_to_substate = 2;
        } else if (drive_to_substate == 2) {
          if (turnDeg('R', 90., 0., DEBUGMODE)) drive_to_substate = 3;
        } else if (drive_to_substate == 3) {
          if (lineFollow('R', 10, autonomous_approach_speed)) drive_to_substate = 4;
        } else if (drive_to_substate == 4) {
          if (lineFollow('F', 1000., autonomous_approach_speed)) {
            drive_to_substate = 0;
            DONEDrive = true;
            position = 'C';
          }
        }
      }
      break;
    case 'C':                                          // Crafting
      if (destination == 'm' || destination == 'M') {  // Drive to Left Mine from Crafting
        if (drive_to_substate == 0) {
          if (lineFollow('R', 15, autonomous_speed)) drive_to_substate = 1;
        } else if (drive_to_substate == 1) {
          if (turnDeg('L', 90., 0., DEBUGMODE)) drive_to_substate = 2;
        } else if (drive_to_substate == 2) {
          if (driveDistance('R', 33., autonomous_speed, DEBUGMODE)) drive_to_substate = 3;
        } else if (drive_to_substate == 3) {
          if (turnDeg('R', 90., 0., DEBUGMODE)) drive_to_substate = 4;
        } else if (drive_to_substate == 4) {
          if (lineFollow('R', 1000., autonomous_approach_speed)) {
            drive_to_substate = 0;
            DONEDrive = true;
            position = 'M';
          }
        }
      } else if (destination == 'k' || destination == 'K') {  // Drive to right Mine from Crafting
        if (drive_to_substate == 0) {
          if (lineFollow('R', 15, autonomous_speed)) drive_to_substate = 1;
        } else if (drive_to_substate == 1) {
          if (turnDeg('L', 90., 0., DEBUGMODE)) drive_to_substate = 2;
        } else if (drive_to_substate == 2) {
          if (driveDistance('F', 33., autonomous_speed, DEBUGMODE)) drive_to_substate = 3;
        } else if (drive_to_substate == 3) {
          if (turnDeg('R', 90., 0., DEBUGMODE)) drive_to_substate = 4;
        } else if (drive_to_substate == 4) {
          if (lineFollow('R', 1000., autonomous_approach_speed)) {
            drive_to_substate = 0;
            DONEDrive = true;
            position = 'K';
          }
        }
      } else if (destination == 'b' || destination == 'B') {  // Drive to Chest from Crafting
        if (drive_to_substate == 0) {
          if (lineFollow('R', 40., autonomous_speed)) drive_to_substate = 1;
        } else if (drive_to_substate == 1) {
          if (turnDeg('L', 90., 0., DEBUGMODE)) drive_to_substate = 2;
        } else if (drive_to_substate == 2) {
          if (driveDistance('F', 106, autonomous_speed, DEBUGMODE)) drive_to_substate = 3;
        } else if (drive_to_substate == 3) {
          if (turnDeg('R', 90., 0., DEBUGMODE)) drive_to_substate = 4;
        } else if (drive_to_substate == 4) {
          if (driveDistance('F', 0, autonomous_speed, DEBUGMODE)) {
            drive_to_substate = 0;
            DONEDrive = true;
            position = 'B';
          }
        }
      } else if (destination == 't' || destination == 'T') {  // Drive to Left Tree from Crafting
        if (drive_to_substate == 0) {
          if (lineFollow('R', 15, autonomous_speed)) drive_to_substate = 1;
        } else if (drive_to_substate == 1) {
          if (turnDeg('L', 90., 0., DEBUGMODE)) drive_to_substate = 2;
        } else if (drive_to_substate == 2) {
          if (driveDistance('R', 93, autonomous_speed, DEBUGMODE)) drive_to_substate = 3;
        } else if (drive_to_substate == 3) {
          if (turnDeg('R', 90., 0., DEBUGMODE)) drive_to_substate = 4;
        } else if (drive_to_substate == 4) {
          if (lineFollow('R', 1000., autonomous_approach_speed)) {
            drive_to_substate = 0;
            DONEDrive = true;
            position = 'T';
          }
        }
      } else if (destination == 'a' || destination == 'A') {  // Drive to right Tree from Crafting
        if (drive_to_substate == 0) {
          if (lineFollow('R', 15, autonomous_speed)) drive_to_substate = 1;
        } else if (drive_to_substate == 1) {
          if (turnDeg('L', 90., 0., DEBUGMODE)) drive_to_substate = 2;
        } else if (drive_to_substate == 2) {
          if (driveDistance('F', 95, autonomous_speed, DEBUGMODE)) drive_to_substate = 3;
        } else if (drive_to_substate == 3) {
          if (turnDeg('R', 90., 0., DEBUGMODE)) drive_to_substate = 4;
        } else if (drive_to_substate == 4) {
          if (lineFollow('R', 1000., autonomous_approach_speed)) {
            drive_to_substate = 0;
            DONEDrive = true;
            position = 'A';
          }
        }
      }
      break;
    case 'B':                                          // Chest
      if (destination == 'm' || destination == 'M') {  // Drive to Left Mine from Chest
        if (drive_to_substate == 0) {
          if (driveDistance('R', 35, autonomous_speed, DEBUGMODE)) drive_to_substate = 1;
        } else if (drive_to_substate == 1) {
          if (turnDeg('L', 90., 0., DEBUGMODE)) drive_to_substate = 2;
        } else if (drive_to_substate == 2) {
          if (driveDistance('R', 122, autonomous_speed, DEBUGMODE)) drive_to_substate = 3;
        } else if (drive_to_substate == 3) {
          if (turnDeg('R', 90., 0., DEBUGMODE)) drive_to_substate = 4;
        } else if (drive_to_substate == 4) {
          if (lineFollow('R', 1000., autonomous_approach_speed)) {
            drive_to_substate = 0;
            DONEDrive = true;
            position = 'M';
          }
        }
      } else if (destination == 'k' || destination == 'K') {  // Drive to right Mine from Chest
        if (drive_to_substate == 0) {
          if (driveDistance('R', 35, autonomous_speed, DEBUGMODE)) drive_to_substate = 1;
        } else if (drive_to_substate == 1) {
          if (turnDeg('L', 90., 0., DEBUGMODE)) drive_to_substate = 2;
        } else if (drive_to_substate == 2) {
          if (driveDistance('R', 72, autonomous_speed, DEBUGMODE)) drive_to_substate = 3;
        } else if (drive_to_substate == 3) {
          if (turnDeg('R', 90., 0., DEBUGMODE)) drive_to_substate = 4;
        } else if (drive_to_substate == 4) {
          if (lineFollow('R', 1000., autonomous_approach_speed)) {
            drive_to_substate = 0;
            DONEDrive = true;
            position = 'K';
          }
        }
      } else if (destination == 'c' || destination == 'C') {  // Drive to Crafting from Chest
        if (drive_to_substate == 0) {
          if (driveDistance('R', 35, autonomous_speed, DEBUGMODE)) drive_to_substate = 2;
        } else if (drive_to_substate == 2) {
          if (turnDeg('L', 90., 0., DEBUGMODE)) drive_to_substate = 3;
        } else if (drive_to_substate == 3) {
          if (driveDistance('R', 103, autonomous_speed, DEBUGMODE)) drive_to_substate = 4;
        } else if (drive_to_substate == 4) {
          if (turnDeg('R', 90., 0., DEBUGMODE)) drive_to_substate = 5;
        } else if (drive_to_substate == 5) {
          if (lineFollow('F', 1000., autonomous_approach_speed)) {
            drive_to_substate = 0;
            DONEDrive = true;
            position = 'C';
          }
        }
      } else if (destination == 't' || destination == 'T') {  // Drive to Left Tree from Chest
        if (drive_to_substate == 0) {
          if (driveDistance('R', 35, autonomous_speed, DEBUGMODE)) drive_to_substate = 2;
        } else if (drive_to_substate == 2) {
          if (turnDeg('L', 90., 0., DEBUGMODE)) drive_to_substate = 3;
        } else if (drive_to_substate == 3) {
          if (driveDistance('R', 193, autonomous_speed, DEBUGMODE)) drive_to_substate = 4;
        } else if (drive_to_substate == 4) {
          if (turnDeg('R', 90., 0., DEBUGMODE)) drive_to_substate = 5;
        } else if (drive_to_substate == 5) {
          if (lineFollow('R', 1000., autonomous_approach_speed)) {
            drive_to_substate = 0;
            DONEDrive = true;
            position = 'T';
          }
        }
      }
      break;
    case 'K':                                          // Right Mine (Kennecott)
      if (destination == 't' || destination == 'T') {  // Drive to Left Tree from Right Mine
        if (drive_to_substate == 0) {
          if (lineFollow('F', 25., autonomous_speed)) drive_to_substate = 1;
        } else if (drive_to_substate == 1) {
          if (turnDeg('L', 90., 0., DEBUGMODE)) drive_to_substate = 2;
        } else if (drive_to_substate == 2) {
          if (driveDistance('R', 126., autonomous_speed, DEBUGMODE)) drive_to_substate = 3;
        } else if (drive_to_substate == 3) {
          if (turnDeg('R', 90., 0., DEBUGMODE)) drive_to_substate = 4;
        } else if (drive_to_substate == 4) {
          if (lineFollow('R', 1000., autonomous_approach_speed)) {
            drive_to_substate = 0;
            DONEDrive = true;
            position = 'T';
          }
        }
      } else if (destination == 'a' || destination == 'A') {  // Drive to Right Tree from Right Mine
        if (drive_to_substate == 0) {
          if (lineFollow('F', 25., autonomous_speed)) drive_to_substate = 1;
        } else if (drive_to_substate == 1) {
          if (turnDeg('L', 90., 0., DEBUGMODE)) drive_to_substate = 2;
        } else if (drive_to_substate == 2) {
          if (driveDistance('F', 63., autonomous_speed, DEBUGMODE)) drive_to_substate = 3;
        } else if (drive_to_substate == 3) {
          if (turnDeg('R', 90., 0., DEBUGMODE)) drive_to_substate = 4;
        } else if (drive_to_substate == 4) {
          if (lineFollow('R', 1000., autonomous_approach_speed)) {
            drive_to_substate = 0;
            DONEDrive = true;
            position = 'A';
          }
        }
      } else if (destination == 'm' || destination == 'M') {  // Drive to left Mine from right mine
        if (drive_to_substate == 0) {
          if (lineFollow('F', 25., autonomous_speed)) drive_to_substate = 1;
        } else if (drive_to_substate == 1) {
          if (turnDeg('L', 90., 0., DEBUGMODE)) drive_to_substate = 2;
        } else if (drive_to_substate == 2) {
          if (driveDistance('R', 63., autonomous_speed, DEBUGMODE)) drive_to_substate = 3;
        } else if (drive_to_substate == 3) {
          if (turnDeg('R', 90., 0., DEBUGMODE)) drive_to_substate = 4;
        } else if (drive_to_substate == 4) {
          if (lineFollow('R', 1000., autonomous_approach_speed)) {
            drive_to_substate = 0;
            DONEDrive = true;
            position = 'M';
          }
        }
      } else if (destination == 'c' || destination == 'C') {  // Drive to Crafting from Right Mine
        if (DEBUGMODE) Serial3.println("Driving to Crafting from Mine");
        if (drive_to_substate == 0) {
          if (lineFollow('F', 25., autonomous_speed)) drive_to_substate = 1;
        } else if (drive_to_substate == 1) {
          if (turnDeg('L', 90., 0., DEBUGMODE)) drive_to_substate = 2;
        } else if (drive_to_substate == 2) {
          if (driveDistance('R', 33., autonomous_speed, DEBUGMODE)) drive_to_substate = 3;
        } else if (drive_to_substate == 3) {
          if (turnDeg('R', 90., 0., DEBUGMODE)) drive_to_substate = 4;
        } else if (drive_to_substate == 4) {
          if (lineFollow('F', 1000., autonomous_approach_speed)) {
            drive_to_substate = 0;
            DONEDrive = true;
            position = 'C';
          }
        }
      } else if (destination == 'b' || destination == 'B') {  // Drive to CHest from right Mine
        if (drive_to_substate == 0) {
          if (turnDeg('L', 90., 30., autonomous_speed)) drive_to_substate = 1;
        } else if (drive_to_substate == 1) {
          if (driveDistance('F', 35, autonomous_speed, DEBUGMODE)) drive_to_substate = 2;
        } else if (drive_to_substate == 2) {
          if (turnDeg('R', 90., 0., DEBUGMODE)) drive_to_substate = 3;
        } else if (drive_to_substate == 3) {
          if (driveDistance('F', 0., autonomous_speed, DEBUGMODE)) {
            drive_to_substate = 0;
            DONEDrive = true;
            position = 'B';
          }
        }
      }
      break;
    case 'A':                                          // Right Tree (Aspen)
      if (destination == 'm' || destination == 'M') {  // Drive to Left Mine from Right Tree
        if (drive_to_substate == 0) {
          if (lineFollow('F', 25., autonomous_speed)) drive_to_substate = 1;
        } else if (drive_to_substate == 1) {
          if (turnDeg('R', 90., 0., DEBUGMODE)) drive_to_substate = 2;
        } else if (drive_to_substate == 2) {
          if (driveDistance('F', 126., autonomous_speed, DEBUGMODE)) drive_to_substate = 3;
        } else if (drive_to_substate == 3) {
          if (turnDeg('L', 90., 0., DEBUGMODE)) drive_to_substate = 4;
        } else if (drive_to_substate == 4) {
          if (lineFollow('R', 1000., autonomous_approach_speed)) {
            drive_to_substate = 0;
            DONEDrive = true;
            position = 'M';
          }
        }
      } else if (destination == 'k' || destination == 'K') {  // Drive to right Mine from Right Tree
        if (drive_to_substate == 0) {
          if (lineFollow('F', 25., autonomous_speed)) drive_to_substate = 1;
        } else if (drive_to_substate == 1) {
          if (turnDeg('R', 90., 0., DEBUGMODE)) drive_to_substate = 2;
        } else if (drive_to_substate == 2) {
          if (driveDistance('F', 63., autonomous_speed, DEBUGMODE)) drive_to_substate = 3;
        } else if (drive_to_substate == 3) {
          if (turnDeg('L', 90., 0., DEBUGMODE)) drive_to_substate = 4;
        } else if (drive_to_substate == 4) {
          if (lineFollow('R', 1000., autonomous_approach_speed)) {
            drive_to_substate = 0;
            DONEDrive = true;
            position = 'K';
          }
        }
      } else if (destination == 't' || destination == 'T') {  // Drive to left tree from right Tree
        if (drive_to_substate == 0) {
          if (lineFollow('F', 25., autonomous_speed)) drive_to_substate = 1;
        } else if (drive_to_substate == 1) {
          if (turnDeg('R', 90., 0., DEBUGMODE)) drive_to_substate = 2;
        } else if (drive_to_substate == 2) {
          if (driveDistance('F', 187., autonomous_speed, DEBUGMODE)) drive_to_substate = 3;
        } else if (drive_to_substate == 3) {
          if (turnDeg('L', 90., 0., DEBUGMODE)) drive_to_substate = 4;
        } else if (drive_to_substate == 4) {
          if (lineFollow('R', 1000., autonomous_approach_speed)) {
            drive_to_substate = 0;
            DONEDrive = true;
            position = 'T';
          }
        }
      } else if (destination == 'c' || destination == 'C') {  // Drive to Crafting from Right Tree
        if (drive_to_substate == 0) {
          if (turnDeg('R', 90., 30., autonomous_speed)) drive_to_substate = 1;
        } else if (drive_to_substate == 1) {
          if (driveDistance('F', 50, autonomous_speed, DEBUGMODE)) drive_to_substate = 2;
        } else if (drive_to_substate == 2) {
          if (turnDeg('L', 90., 0., DEBUGMODE)) drive_to_substate = 3;
        } else if (drive_to_substate == 3) {
          if (lineFollow('R', 10, autonomous_approach_speed)) drive_to_substate = 4;
        } else if (drive_to_substate == 4) {
          if (lineFollow('F', 1000., autonomous_approach_speed)) {
            drive_to_substate = 0;
            DONEDrive = true;
            position = 'C';
          }
        }
      }
      break;
  }
  return DONEDrive;
}