// Library and Object Setup
#include <SoftwareSerial.h>
SoftwareSerial Serial1(2, 3);  // RX, TX


// Variable Intialization
int startFlag = 255;
char command;
char direction;
int distAngle;    // Distance / Angle input
int speedRadius;  // Speed / Radius of turn
bool MATCHSTART = true;
long globalTimer = 0;
long globalTimerT0 = 0;

void setup() {
  // Open serial communications with computer and wait for port to open:
  Serial.begin(57600);  // make sure to also select this baud rate in your Serial Monitor window

  // Wipe screen - ish
  Serial.print("\n\n\n\n\n\n\n\n\n");

  // Print Command Instructions
  Serial.println("\nHello! To set variables on the MEGA, type any of the following commands with spaces deliminating inputs:");
  Serial.println("V : Activate/Deactivate Verbose mode to debug");
  Serial.println("P <W/Y/R> : Pickaxe Level ");
  Serial.println("N : No shield?");
  Serial.println("T <L/R> : Set tree to default to");
  Serial.println("M <L/R> : Set default Mine to go to");
  Serial.println("A <L/R> : Set default Tree and mine at once to go to");
  Serial.println("E <L/R> : Set endgame mine left or right. Default is right");
  Serial.println("F <S> : Finish inturrupted crafting operation (Shield)");
  Serial.println("O <S/P/M/X> : Set goal, craft shield, pickaxe, mine, or just skip shield crafting altogether");
  Serial.println("R : Test color sensor");

  Serial.println("G : Go! Start driving autonomously");
  // Open serial communications with the other Arduino board
  Serial1.begin(115200);  // 115200
}



void loop() {
  if (!MATCHSTART) {
    globalTimer = millis() / 1000.;
    if (globalTimer - globalTimerT0 > 280) {  
      globalTimerT0 = millis() / 1000.;
      String input = "Z";
      Serial1.write(startFlag);
      Serial1.write(input.c_str());
      Serial1.write('\n');
      Serial.print("You sent: ");
      Serial.println(input);
      Serial.println("OUT OF TIME");
    }
  }
  if (Serial.available()) {
    delay(20);
    String input = String(Serial.readStringUntil('\n'));
    Serial1.write(startFlag);
    Serial1.write(input.c_str());
    Serial1.write('\n');
    Serial.print("You sent: ");
    Serial.println(input);
    if (input.charAt(0) == 'g' || input.charAt(0) == 'G'){
      if( MATCHSTART) {
      MATCHSTART = false;
      globalTimerT0 = millis() / 1000.;
      Serial.println("Match started!");
      } else{
        Serial.println("Reset run started!");
      }
    }
  }
  if (Serial1.available()) {
    delay(20);
    String input = String(Serial1.readStringUntil('\n'));
    Serial.println(input);
  }
}
