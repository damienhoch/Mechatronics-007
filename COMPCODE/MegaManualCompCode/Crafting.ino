bool craft(char tool, char level) {
  bool DONECRAFTING = false;
  switch (tool) {
    case 'p':
    case 'P':
      char desiredBlocks[] = {level, level, level, 'W', 'W'};
      if (numBlocks > 0) {
        if (desiredBlocks[crafting_substate] == currentBlocks[strlen(currentBlocks) - 1]) {
          if (crafting_substate == 0) {
            dispense();
            crafting_substate = 1;
          } else if (crafting_substate == 1) {
            if (turnDeg('r', 10, 0, DEBUGMODE)) {
              crafting_substate = 2;
              dispense();
            }
          } else if (crafting_substate == 2) {
            if (turnDeg('l', 20, 0, DEBUGMODE)) {
              crafting_substate = 3;
              dispense();
            }
          } else if (crafting_substate == 3) {
            if (turnDeg('r', 10, 0, DEBUGMODE)) crafting_substate = 4;
          } else if (crafting_substate == 4) {
            if (lineFollow('R', 5, 10)) {
              crafting_substate = 5;
              dispense();
            }
          } else if (crafting_substate == 5) {
            if (lineFollow('R', 5, 10)) {
              dispense();
              crafting_substate = 0;
              DONECRAFTING = true;
              currentAxe = currentAxe + 1;
            }
          }
        } else{
          while(!turnDeg('R', 80, 0, DEBUGMODE));
          dispense();
          while(!turnDeg('L', 80, 0, DEBUGMODE));
        }
      }
      break;
    case 's':
    case 'S':

      break;
  }
  return DONECRAFTING;
}