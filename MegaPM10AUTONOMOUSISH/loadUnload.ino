void loadUnload(char dir) {
  switch (dir) {
    case 'f':
    case 'F':
      digitalWrite(BeltENA1, HIGH);
      digitalWrite(BeltENA2, LOW);
      analogWrite(BeltPWM, map(speedRadius, 100, 400, 100, 255));
      break;
    case 'r':
    case 'R':
      digitalWrite(BeltENA1, LOW);
      digitalWrite(BeltENA2, HIGH);
      analogWrite(BeltPWM, map(speedRadius, 100, 400, 100, 255));
      break;
  };
}