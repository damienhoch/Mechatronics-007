// Rangefinder
const int RFRF = A3; // Rangefinder FWD read pin
const int RFRR = A4; // Rangefinder REV read pin
int previousAverage = 0;
float alpha = 0.1; // Digital Filtering value
float beta = 4.99; // Rangefinder calibration constant
float gamma = -1.4; // Rangefinder calibration constant
float RFdistance = 0; // (cm)
float Kr = 1; //Proportional gain for wall approaching
bool Rangefinder = false; // turn on and off rangefinder section
float RAW = 0;
float RAWV = 0;

// Servo
bool servo = true;
#include <VarSpeedServo.h>
VarSpeedServo TowerServo;
VarSpeedServo MiningServo;
char command = 0;
int angle = 0;
int servoSpeed = 100;
// Servo Pins
const int MiningServoPin = 41;
const int TowerServoPin = 40;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);
  // Servo Setup
  MiningServo.attach(MiningServoPin);
  TowerServo.attach(TowerServoPin);
  // Set Mining Servo to zero position
  MiningServo.write(90,255,true);
  //delay(200);

  // Set tower servo to zero position
  TowerServo.write(90,255,false);

  if(servo){
    Serial.println("Enter Servo name (T or M) followed by 3 digits of position (ie 020 or 160)");
    Serial.println("You can also simply type s or S to stop");
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Rangefinder){
  RAW = analogRead(RFRR);
  RAWV = (RAW / 1024) * 5; // Convert to voltage
  RFdistance = rangefinderDistance(RAWV,beta,gamma);
  Kr = constrain(RFdistance,0,15)/15;
  Serial.print(Kr);
  Serial.println(" kr");
  delay(200);
  }
  if(servo){
    if(Serial.available()){
    delay(20);
    if(Serial.available()>=4){
      command = char(Serial.read());
      int hundreds = Serial.read() - '0';
      int tens     = Serial.read() - '0';
      int ones     = Serial.read() - '0';
      angle = constrain(100*hundreds + 10*tens + ones, 0, 180);
      Serial.print("Servo: ");
      Serial.print(command);
      Serial.print(" going to: ");
      Serial.print(angle);
      Serial.println(" degrees");
      Serial.read();
    }
    else if(Serial.available()==3){
      command = 'S';
      Serial.read();
      Serial.read();
      Serial.read();
    }
    else{
      Serial.read();
      Serial.read();
    }
    }
    switch(command){
      case 's':
      case 'S':
      command = 0;
      break;
      case('m'):
      case('M'):
      MiningServo.write(angle,servoSpeed,false);
      command = 0;
      break;
      case 't':
      case 'T':
      TowerServo.write(angle,servoSpeed,false);
      command = 0;
      break;
    }
  }

}



float weightedRecursiveAverage(float newValue, float previousAverage, float alpha) {
  // alpha is your weighting factor (0 < alpha < 1)
  // alpha closer to 1 = more weight on new data (faster response)
  // alpha closer to 0 = more weight on history (smoother output)
  return alpha * newValue + (1.0 - alpha) * previousAverage;
}
float rangefinderDistance(float Voltage, float beta, float gamma) {
  // 
  return beta * pow(Voltage, gamma);
}