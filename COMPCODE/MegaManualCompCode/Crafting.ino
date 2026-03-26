bool craft(char tool) {
  bool DONECRAFTING = false;
  switch (tool) {
    case 'p':
    case 'P':
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
        if(lineFollow('R',5,10)){
          dispense();
          crafting_substate = 0;
          DONECRAFTING = true;
        }
      }
      break;
  }
  return DONECRAFTING;
}