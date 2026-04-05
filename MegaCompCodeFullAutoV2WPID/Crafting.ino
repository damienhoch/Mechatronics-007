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
              Serial3.print("Desired Block: ");
              Serial3.print(desiredBlocks[crafting_substate]);
              Serial3.print("Actual Block: ");
              Serial3.println(currentBlocks[0]);
              Serial3.print("Actual Blocks: ");
              Serial3.println(currentBlocks);
            }
            while (!turnDeg('R', 80, 0, DEBUGMODE)) {
            }
            dispense();
            while (!turnDeg('L', 80, 0, DEBUGMODE)) {
            }
          } else {
            if (crafting_substate == 0) {
              //if (lineFollow('F', 100, 10)) {
              CraftingDistance = 2;
              if (DEBUGMODE) {
                Serial3.print("Desired Block: ");
                Serial3.print(desiredBlocks[crafting_substate]);
                Serial3.print("Actual Block: ");
                Serial3.println(currentBlocks[0]);
                Serial3.print("Actual Blocks: ");
                Serial3.println(currentBlocks);
              }
              dispense();
              crafting_substate = 1;
            } else if (crafting_substate == 1) {
              if (turnDeg('r', 10, 0, DEBUGMODE)) {
                if (DEBUGMODE) {
                  Serial3.print("Desired Block: ");
                  Serial3.print(desiredBlocks[crafting_substate]);
                  Serial3.print("Actual Block: ");
                  Serial3.println(currentBlocks[0]);
                  Serial3.print("Actual Blocks: ");
                  Serial3.println(currentBlocks);
                }
                crafting_substate = 2;
                dispense();
              }
            } else if (crafting_substate == 2) {
              if (turnDeg('l', 20, 0, DEBUGMODE)) {
                if (DEBUGMODE) {
                  Serial3.print("Desired Block: ");
                  Serial3.print(desiredBlocks[crafting_substate]);
                  Serial3.print("Actual Block: ");
                  Serial3.println(currentBlocks[0]);
                  Serial3.print("Actual Blocks: ");
                  Serial3.println(currentBlocks);
                }
                dispense();
                while ((turnDeg('r', 10, 0, DEBUGMODE))) {
                  delay(5);
                }
                crafting_substate = 3;
                delay(5);
              }
            } else if (crafting_substate == 3) {
              if (lineFollow('R', 7, autonomous_approach_speed)) {
                CraftingDistance = CraftingDistance + 6;
                crafting_substate = 4;
                dispense();
              }
            } else if (crafting_substate == 4) {
              if (CraftingDistance == 8) {
                if (lineFollow('R', 7, autonomous_approach_speed)) {
                  dispense();
                  crafting_substate = 0;
                  DONECRAFTING = true;
                  currentAxe = currentAxe + 1;
                }
              } else {
                if (lineFollow('R', 14, 10)) {
                  dispense();
                  crafting_substate = 0;
                  DONECRAFTING = true;
                  currentAxe = currentAxe + 1;
                }
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
            while (!turnDeg('R', 80, 0, DEBUGMODE)) {
            }
            dispense();
            if (DEBUGMODE) {
              Serial3.print("Desired Block: ");
              Serial3.print(desiredBlocks[crafting_substate]);
              Serial3.print(" Actual Block: ");
              Serial3.println(currentBlocks[0]);
            }
            while (!turnDeg('L', 80, 0, DEBUGMODE)) {
            }
          } else {
            if (crafting_substate == 0) {
              //if (lineFollow('F', 100, 10)) {
              CraftingDistance = 2;
              dispense();
              crafting_substate = 1;
              //}
              if (DEBUGMODE) {
                Serial3.print("Desired Block: ");
                Serial3.print(desiredBlocks[crafting_substate]);
                Serial3.print(" Actual Block: ");
                Serial3.println(currentBlocks[0]);
              }
            } else if (crafting_substate == 1) {
              if (lineFollow('r', 7, 10)) {
                CraftingDistance = CraftingDistance + 7;
                crafting_substate = 2;
                dispense();
                if (DEBUGMODE) {
                  Serial3.print("Desired Block: ");
                  Serial3.print(desiredBlocks[crafting_substate]);
                  Serial3.print(" Actual Block: ");
                  Serial3.println(currentBlocks[0]);
                }
              }
            } else if (crafting_substate == 2) {
              if (turnDeg('r', 10, 0, DEBUGMODE)) {
                crafting_substate = 3;
                dispense();
                if (DEBUGMODE) {
                  Serial3.print("Desired Block: ");
                  Serial3.print(desiredBlocks[crafting_substate]);
                  Serial3.print(" Actual Block: ");
                  Serial3.println(currentBlocks[0]);
                }
              }
            } else if (crafting_substate == 3) {
              if (turnDeg('l', 20, 0, DEBUGMODE)) {
                dispense();
                crafting_substate = 4;
                if (DEBUGMODE) {
                  Serial3.print("Desired Block: ");
                  Serial3.print(desiredBlocks[crafting_substate]);
                  Serial3.print(" Actual Block: ");
                  Serial3.println(currentBlocks[0]);
                }
              }
            } else if (crafting_substate == 4) {
              if (lineFollow('R', 6, 10)) {
                CraftingDistance = CraftingDistance + 6;
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
              if (CraftingDistance == 15) {
                dispense();
                crafting_substate = 0;
                DONECRAFTING = true;
                NOSHIELD = false;
              } else {
                dispense();
                dispense();
                crafting_substate = 0;
                DONECRAFTING = true;
                NOSHIELD = false;
              }
            }
          }
        }
      }
      break;
  }
  return DONECRAFTING;
}