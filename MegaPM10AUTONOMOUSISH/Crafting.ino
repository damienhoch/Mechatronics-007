void craft() {
  LS2 = true;
  digitalWrite(BeltENA1, HIGH);
  digitalWrite(BeltENA2, LOW);
  analogWrite(BeltPWM, 255);
  while (LS2) {
    LS2 = digitalRead(LS2Pin);
  }
  if (DEBUGMODE) Serial.println("Block ready to dispense");
  delay(10);  // switch bounce
  // Block ready to dispense
  while (!LS2) {
    LS2 = digitalRead(LS2Pin);
  }
  // Block Dispensed
  if (DEBUGMODE) Serial.println("Block Dispensed");
  digitalWrite(BeltENA1, LOW);
  digitalWrite(BeltENA2, LOW);
  analogWrite(BeltPWM, 0);
  Command = 0;
}