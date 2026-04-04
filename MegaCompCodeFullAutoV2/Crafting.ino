bool craft(char tool, char ore) {
  bool DONECRAFTING = false;
  switch (tool) {
    case 'p':
    case 'P':
      {
        char desiredBlocks[] = { ore, ore, ore, 'W', 'W' };
        if (numBlocks > 0) {
          if (desiredBlocks[crafting_substate] != currentBlocks[0]) {
            if (DEBUGMODE) {
              Serial.print("Desired Block: ");
              Serial.print(desiredBlocks[crafting_substate]);
              Serial.print("Actual Block: ");
              Serial.println(currentBlocks[0]);
              Serial.print("Actual Blocks: ");
              Serial.println(currentBlocks);
            }
            while (!turnDeg('R', 80, 0, DEBUGMODE)) {
            }
            dispense();
            while (!turnDeg('L', 80, 0, DEBUGMODE)) {
            }
          } else {
            if (crafting_substate == 0) {
              if (DEBUGMODE) {
                Serial.print("Desired Block: ");
                Serial.print(desiredBlocks[crafting_substate]);
                Serial.print("Actual Block: ");
                Serial.println(currentBlocks[0]);
                Serial.print("Actual Blocks: ");
                Serial.println(currentBlocks);
              }
              dispense();
              crafting_substate = 1;
            } else if (crafting_substate == 1) {
              if (turnDeg('r', 10, 0, DEBUGMODE)) {
                if (DEBUGMODE) {
                  Serial.print("Desired Block: ");
                  Serial.print(desiredBlocks[crafting_substate]);
                  Serial.print("Actual Block: ");
                  Serial.println(currentBlocks[0]);
                  Serial.print("Actual Blocks: ");
                  Serial.println(currentBlocks);
                }
                crafting_substate = 2;
                dispense();
              }
            } else if (crafting_substate == 2) {
              if (turnDeg('l', 20, 0, DEBUGMODE)) {
                if (DEBUGMODE) {
                  Serial.print("Desired Block: ");
                  Serial.print(desiredBlocks[crafting_substate]);
                  Serial.print("Actual Block: ");
                  Serial.println(currentBlocks[0]);
                  Serial.print("Actual Blocks: ");
                  Serial.println(currentBlocks);
                }
                dispense();
                while ((turnDeg('r', 10, 0, DEBUGMODE)))
                  ;
                crafting_substate = 3;
              }
            } else if (crafting_substate == 3) {
              if (lineFollow('R', 6, 10)) {
                crafting_substate = 4;
                dispense();
              }
            } else if (crafting_substate == 4) {
              RFdistance = rangefinderDistance(analogRead(RFRF), beta, gamma);
              if (DEBUGMODE) {
                Serial.print("RF DISTANCE: ");
                Serial.println(RFdistance);
              }
              while (craftingRangefinderTol < fabs(RFdistance - (CraftApproachStop + 6))) {
                RFdistance = rangefinderDistance(analogRead(RFRF), beta, gamma);
                if (DEBUGMODE) {
                  Serial.print("RF DISTANCE: ");
                  Serial.println(RFdistance);
                }
                if ((RFdistance - (CraftApproachStop + 6)) < 0) {
                  lineFollow('R', fabs(RFdistance - (CraftApproachStop + 6.)), 10);
                } else {
                  lineFollow('F', fabs(RFdistance - (CraftApproachStop + 6.)), 10);
                }
              }
              dispense();
              crafting_substate = 0;
              DONECRAFTING = true;
              currentAxe = currentAxe + 1;
            }
          }
        }
      }
      break;
    case 's':
    case 'S':
      {
        char desiredBlocks[] = { 'W', 'W', 'W', 'W', 'W', 'W', 'R' };
        if (numBlocks > 0) {
          if (desiredBlocks[crafting_substate] != currentBlocks[0]) {
            if (DEBUGMODE) {
              Serial.print("Desired Block: ");
              Serial.print(desiredBlocks[crafting_substate]);
              Serial.print("Actual Block: ");
              Serial.println(currentBlocks[0]);
              Serial.print("Actual Blocks: ");
              Serial.println(currentBlocks);
            }
            while (!turnDeg('R', 80, 0, DEBUGMODE)) {
            }
            dispense();
            while (!turnDeg('L', 80, 0, DEBUGMODE)) {
            }
          } else {
            if (crafting_substate == 0) {
              if (DEBUGMODE) {
                Serial.print("Desired Block: ");
                Serial.print(desiredBlocks[crafting_substate]);
                Serial.print("Actual Block: ");
                Serial.println(currentBlocks[0]);
                Serial.print("Actual Blocks: ");
                Serial.println(currentBlocks);
              }
              dispense();
              crafting_substate = 1;
            } else if (crafting_substate == 1) {
              if (lineFollow('r', 6, 10)) {
                if (DEBUGMODE) {
                  Serial.print("Desired Block: ");
                  Serial.print(desiredBlocks[crafting_substate]);
                  Serial.print("Actual Block: ");
                  Serial.println(currentBlocks[0]);
                  Serial.print("Actual Blocks: ");
                  Serial.println(currentBlocks);
                }
                crafting_substate = 2;
                dispense();
              }
            } else if (crafting_substate == 2) {
              if (turnDeg('r', 10, 0, DEBUGMODE)) {
                if (DEBUGMODE) {
                  Serial.print("Desired Block: ");
                  Serial.print(desiredBlocks[crafting_substate]);
                  Serial.print("Actual Block: ");
                  Serial.println(currentBlocks[0]);
                  Serial.print("Actual Blocks: ");
                  Serial.println(currentBlocks);
                }
                crafting_substate = 3;
                dispense();
              }
            } else if (crafting_substate == 3) {
              if (turnDeg('l', 20, 0, DEBUGMODE)) {
                if (DEBUGMODE) {
                  Serial.print("Desired Block: ");
                  Serial.print(desiredBlocks[crafting_substate]);
                  Serial.print("Actual Block: ");
                  Serial.println(currentBlocks[0]);
                  Serial.print("Actual Blocks: ");
                  Serial.println(currentBlocks);
                }
                dispense();
                crafting_substate = 4;
              }
            } else if (crafting_substate == 4) {
              if (lineFollow('R', 6, 10)) {
                while ((turnDeg('r', 10, 0, DEBUGMODE))) {
                }

                crafting_substate = 5;
                dispense();
              }
            } else if (crafting_substate == 5) {
              if (turnDeg('l', 20, 0, DEBUGMODE)) {
                dispense();
                while (turnDeg('r', 10, 0, DEBUGMODE)) {
                }
                crafting_substate = 6;
              }
            } else if (crafting_substate == 6) {
              RFdistance = rangefinderDistance(analogRead(RFRF), beta, gamma);
              if (DEBUGMODE) {
                Serial.print("RF DISTANCE: ");
                Serial.println(RFdistance);
              }
              while (craftingRangefinderTol < fabs(RFdistance - (CraftApproachStop + 10))) {
                RFdistance = rangefinderDistance(analogRead(RFRF), beta, gamma);
                if (DEBUGMODE) {
                  Serial.print("RF DISTANCE: ");
                  Serial.println(RFdistance);
                }
                if ((RFdistance - (CraftApproachStop + 10)) < 0) {
                  lineFollow('R', fabs(RFdistance - (CraftApproachStop + 10.)), 10);
                } else {
                  lineFollow('F', fabs(RFdistance - (CraftApproachStop + 10.)), 10);
                }
              }
              dispense();
              crafting_substate = 0;
              DONECRAFTING = true;
              NOSHIELD = false;
            }
          }
        }
      }
      break;
  }
  return DONECRAFTING;
}