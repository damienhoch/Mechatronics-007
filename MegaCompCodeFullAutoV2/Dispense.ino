void dispense() {
  LS2 = true;
  int LS2Counter = 0;
  digitalWrite(BeltENA1, HIGH);
  digitalWrite(BeltENA2, LOW);
  analogWrite(BeltPWM, 255);
  while (LS2) {
    LS2 = digitalRead(LS2Pin);
  }
  if (DEBUGMODE) Serial.println("Block ready to dispense");
  delay(10);  // switch bounce
  // Block ready to dispense
  while (LS2Counter<5) {
    LS2 = digitalRead(LS2Pin);
    if(LS2 == false){
      LS2Counter = LS2Counter + 1;
    }
  }
  // Block Dispensed
  if (DEBUGMODE) Serial.println("Block Dispensed");
  digitalWrite(BeltENA1, LOW);
  digitalWrite(BeltENA2, LOW);
  analogWrite(BeltPWM, 0);
  numBlocks = numBlocks - 1;
  
  incrementLoaderArray();
  if(position == 'C' || position == 'c') delay(craftPauseTime);
}