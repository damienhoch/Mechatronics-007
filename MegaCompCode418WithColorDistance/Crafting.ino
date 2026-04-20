bool craft(char tool, char ore) {
  bool DONECRAFTING = false;
  switch (tool) {
    case 'p':
    case 'P':
      {
        char desiredBlocks[] = { ore, ore, ore, 'W', 'W' };
        if (numBlocks > 0) {
          if (desiredBlocks[crafting_substate] != currentBlocks[0]) {
            while (!lineFollow('R', 5, 10)) {
            }
            while (!turnDeg('R', 30, 0, DEBUGMODE)) {
            }
            while (numBlocks > 0 && desiredBlocks[crafting_substate] != currentBlocks[0]) {
              dispense();
            }
            while (!turnDeg('L', 30, 0, DEBUGMODE)) {
            }
            while (!lineFollow('F', 5, 10)) {
            }
            CraftApproachStop = -1;
            while (!driveDistance('F', 3, 10, DEBUGMODE)) {
            }
            CraftApproachStop = 2.1;
            delay(50);
          } else {
            if (crafting_substate == 0) {
              //if (lineFollow('F', 100, 10)) {
              delay(10);
              CraftApproachStop = -1;
              while (!driveDistance('F', 3, 10, DEBUGMODE)) {
              }
              CraftApproachStop = 2.1;
              delay(50);
              while (!(turnDeg('r', 10, 0, DEBUGMODE))) {
              }
              dispense();
              while (!turnDeg('l', 10, 0, DEBUGMODE)) {
              }
              crafting_substate = 1;
            } else if (crafting_substate == 1) {
              if (turnDeg('l', 10, 0, DEBUGMODE)) {
                crafting_substate = 2;
                delay(10);
                dispense();
              }
            } else if (crafting_substate == 2) {
              if (turnDeg('R', 10, 0, DEBUGMODE)) {
                dispense();
                crafting_substate = 3;
                delay(5);
              }
            } else if (crafting_substate == 3) {
              if (lineFollow('R', 6, autonomous_approach_speed)) {
                crafting_substate = 4;
                dispense();
              }
            } else if (crafting_substate == 4) {
              if (lineFollow('R', 6, autonomous_approach_speed)) {
                dispense();
                crafting_substate = 0;
                DONECRAFTING = true;
                currentAxe = currentAxe + 1;
                DispenseServo.write(dispense1);
              }
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
          //Serial3.println(currentBlocks);
          if (desiredBlocks[crafting_substate] != currentBlocks[0]) {
            while (!lineFollow('R', 5, 10)) {
            }
            while (!turnDeg('R', 30, 0, DEBUGMODE)) {
            }
            while (numBlocks > 0 && desiredBlocks[crafting_substate] != currentBlocks[0]) {
              dispense();
            }
            while (!turnDeg('L', 30, 0, DEBUGMODE)) {
            }
            while (!lineFollow('F', 5, 10)) {
            }
          } else {
            if (crafting_substate == 0) {
              delay(10);
              CraftApproachStop = -1;
              while (!driveDistance('F', 3, 10, DEBUGMODE)) {
              }
              CraftApproachStop = 2.1;
              delay(50);
              dispense();
              crafting_substate = 1;
              //}
            } else if (crafting_substate == 1) {
              if (lineFollow('r', 5, 10)) {
                while (!turnDeg('r', 10, 0, DEBUGMODE)) {
                }
                crafting_substate = 2;
                dispense();
              }
            } else if (crafting_substate == 2) {
              if (turnDeg('l', 20, 0, DEBUGMODE)) {
                crafting_substate = 3;
                dispense();
              }
            } else if (crafting_substate == 3) {
              if (turnDeg('r', 10, 0, DEBUGMODE)) {
                dispense();
                crafting_substate = 4;
              }
            } else if (crafting_substate == 4) {
              if (lineFollow('R', 6, 10)) {
                while ((!turnDeg('r', 10, 0, DEBUGMODE))) {
                }
                crafting_substate = 5;
                dispense();
              }
            } else if (crafting_substate == 5) {
              if (turnDeg('l', 20, 0, DEBUGMODE)) {
                dispense();
                while (!turnDeg('r', 10, 0, DEBUGMODE)) {
                }
                crafting_substate = 6;
              }
            } else if (crafting_substate == 6) {
              dispense();
              crafting_substate = 0;
              DONECRAFTING = true;
              NOSHIELD = false;
              DispenseServo.write(dispense1);
            }
          }
        }
      }
      break;
  }
  return DONECRAFTING;
}