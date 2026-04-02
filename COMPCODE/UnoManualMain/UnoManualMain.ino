// Library and Object Setup
#include <SoftwareSerial.h>
SoftwareSerial Serial1(2, 3); // RX, TX


// Variable Intialization
int startFlag = 255;
char command;
char direction;
int distAngle; // Distance / Angle input
int speedRadius; // Speed / Radius of turn

void setup()  
{
  // Open serial communications with computer and wait for port to open:
  Serial.begin(57600);  // make sure to also select this baud rate in your Serial Monitor window

  // Wipe screen - ish
  Serial.print("\n\n\n\n\n\n\n\n\n");

  // Print Command Instructions
  Serial.println("\nHello! To send commands to the MEGA, type any of the following commands with spaces deliminating inputs:");
  Serial.println("Commands:\nS : Stop Current Action");
  Serial.println("E <F/R> <distance (cm)> <speed (cm/s)> : Drive in a straight line using encoders");
  Serial.println("L <F/R> <distance (cm)> <speed (cm/s)> : Line follow, if distance is 0, line follows until rangefinder detects a wall");
  Serial.println("T <L/R> : Set tree to default to");
  Serial.println("M <L/R> : Set default Mine to go to");
  Serial.println("U <F/R> <speed (1-4)> : Load/Unload blocks");
  Serial.println("X : Drop Tower");
  Serial.println("C : Craft Unload blocks");
  Serial.println("D <M/T/C/B> : Drive to specific location");
  Serial.println("V : Activate/Deactivate Verbose mode to debug");
  Serial.println("P <0> <0-4> : Pickaxe Level ");
  Serial.println("N : No shield?");
  Serial.println("G : Start driving autonomously");
  // Open serial communications with the other Arduino board
  Serial1.begin(115200);  // 115200
}



void loop(){
  if(Serial.available()){
    delay(20);
    String input = String(Serial.readStringUntil('\n'));
    Serial1.write(startFlag);
    Serial1.write(input.c_str());
    Serial1.write('\n');
    Serial.print("You sent: ");
    Serial.println(input);
  }
}
