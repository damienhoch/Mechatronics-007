void dispense() {
  LS2 = true;
  int Ls2Old = true;
  bool SwitchRead = true;
  t0 = millis();
  digitalWrite(BeltENA1, HIGH);
  digitalWrite(BeltENA2, LOW);
  analogWrite(BeltPWM, 255);
  while (SwitchRead) {
    t = millis();
    LS2 = digitalRead(LS2Pin);
    Ls2Old = LS2;
    if (!LS2) {
      delay(70);
      LS2 = digitalRead(LS2Pin);
      if (LS2 == Ls2Old) SwitchRead = false;
    }
    if ((t - t0) > dispenseTimout) SwitchRead = false;
  }
  delay(10);  // switch bounce
  // Block ready to dispense
  while (!SwitchRead) {
    t = millis();
    LS2 = digitalRead(LS2Pin);
    Ls2Old = LS2;
    if (LS2) {
      delay(70);
      LS2 = digitalRead(LS2Pin);
      if (LS2 == Ls2Old) SwitchRead = true;
    }
  }
  // Block Dispensed
  //if (DEBUGMODE) Serial3.println("Block Dispensed");
  digitalWrite(BeltENA1, LOW);
  digitalWrite(BeltENA2, LOW);
  analogWrite(BeltPWM, 0);
  numBlocks = numBlocks - 1;

  incrementLoaderArray();
  if (position == 'C' || position == 'c') delay(craftPauseTime);
}