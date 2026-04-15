void missLoad() {
  if (DEBUGMODE) Serial3.println("TIMED OUT");
  //MINEFAIL = true;
  digitalWrite(BeltENA1, LOW);
  digitalWrite(BeltENA2, LOW);
  analogWrite(BeltPWM, 0);
  MiningServo.write(mineServoRest);
  bool WAITING = true;
  while (WAITING) {
    if (lineFollow('F', 10, 10)) break;
  }
  while (WAITING) {
    if (turnDeg('L', 90, 0, DEBUGMODE)) break;
  }
  while (WAITING) {
    if (driveDistance('R', 30, 150, DEBUGMODE)) break;
  }
  while (WAITING) {
    if (driveDistance('F', 35, 150, DEBUGMODE)) break;
  }
  while (WAITING) {
    if (turnDeg('R', 90, 0, DEBUGMODE)) break;
  }
  while (WAITING) {
    if (lineFollow('F', 10, 10)) break;
  }
  while (WAITING) {
    if (lineFollow('R', 50, 10)) break;
  }
}