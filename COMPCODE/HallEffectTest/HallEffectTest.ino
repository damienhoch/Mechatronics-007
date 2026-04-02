float Sense = 0;
const int BeltENA1 = 53;
const int BeltENA2 = 52;
const int BeltPWM = 11;
void setup() {
  pinMode(A5, INPUT);
  Serial.begin(57600);
  digitalWrite(BeltENA1, HIGH);
  digitalWrite(BeltENA2, LOW);
  analogWrite(BeltPWM, 255);
}

void loop() {
  Sense = abs(map(analogRead(A5), 0, 1023, 0, 500) / 100.0 - 2.5);
  Serial.print("Hall Effect Sensor Reading: ");
  Serial.print(Sense);
  Serial.println(" V");
  delay(20);
}
