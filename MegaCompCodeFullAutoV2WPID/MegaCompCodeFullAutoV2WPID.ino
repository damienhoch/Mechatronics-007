#include <VarSpeedServo.h>
#include <DualTB9051FTGMotorShieldUnoMega.h>
#include <QTRSensors.h>
#include <Encoder.h>
VarSpeedServo TowerServo;
VarSpeedServo MiningServo;
VarSpeedServo DispenseServo;

//////////////////////////////////////////////////////////////
// DEBUG
bool DEBUGMODE = false;
int autonomous_speed = 30;
int drive_to_substate = 0;
int crafting_substate = 0;
int numAttempts = 0;
const int attemptThreshold = 100;
char MetaState = 0;
int desiredNumOre = 3;
int desiredNumWood = 2;
int maxNumBlocks = 7;  // Change depending on how many we can actually fit
char goal = 'P';
char defaultTree = 'T';  // Tree: T = left A = right (Aspen)
char defaultMine = 'M';  // Mine: M = left K = right
int craftingRangefinderTol = 1;
int craftPauseTime = 50;
int CraftingDistance = 2;

//////////////////////////////////////////////////////////////
//Communication Variables
char Command = 0;
char direction;
double distAngle;    // Distance / Angle input
double speedRadius;  // Speed / Radius of turn
char desiredOre[] = { '0', 'Y', 'R', 'B' };

//////////////////////////////////////////////////////////////
// Line Following
// Reflectance Array
QTRSensors qtrF;                     // create a FWD reflectance sensor object
QTRSensors qtrR;                     // create a REV reflectance sensor object
const uint8_t SensorCount = 8;       // # of sensors in reflectance array
uint16_t sensorValues[SensorCount];  //reflectance sensor readings
float sensor_biasF[8] = { 192, 144, 140, 140, 140, 140, 92 };
float sensor_biasR[8] = { 240, 136, 136, 140, 140, 140, 188 };
int Sensor_value_unbiased[8];

float SumAi = 0;
float SumAiDi = 0;
float d;
float d0 = 2.8;
float Error;
double elapsed_distance = 0;  // Distance travelled while Line following

const int RENAF = 23;  // FWD Reflectance array enable pin
const int RENAR = 22;  // REV Reflectance array enable pin

// Rangefinder
const int RFRF = A3;   // Rangefinder FWD read pin
const int RFRR = A4;   // Rangefinder REV read pin
float beta = 4.99;     // Rangefinder calibration constant
float gamma = -1.4;    // Rangefinder calibration constant
float RFdistance = 0;  // (cm)

//////////////////////////////////////////////////////////////
// Servos
const int MiningServoPin = 41;
const int TowerServoPin = 40;
const int DispenseServoPin = A2;
int MineDelay = 150;  // Amount of time to wait between mining strokes
int mineStrokes;
const int mineServoRest = 45;
const int mine1 = 160;
const int mine2 = 140;
int loadDelay = 500;
const int dispense1 = 80;
const int dispense2 = 0;

//////////////////////////////////////////////////////////////
// General Booleans
bool FWDCAP = true;  // Space to move ahead of the robot
bool REVCAP = true;  // Space to move behind the robot
bool TOWERDOWN = false;
bool BLOCKMINED = false;
bool SENSING = true;
bool PREDRIVING = true;  // Bool to instantiate
bool DRIVING = true;
bool DONEWITHCOMMAND = false;
bool MINEFAIL = false;
bool START = false;  // Stop recieving comms, start driving autonomously


//////////////////////////////////////////////////////////////
// Mining
const int BeltENA1 = 53;
const int BeltENA2 = 52;
const int BeltPWM = 46;
const int LS1Pin = 43;  // Block loading limit switch
const int LS2Pin = 42;  // Block unloading limit switch
bool LS1 = false;
bool LS2 = false;
const int numHitsWood[4] = { 5, 4, 2, 1 };
const int numHitsStone[4] = { 10, 5, 3, 2 };
const int numHitsIron[4] = { 11, 10, 5, 3 };
const int numHitsDiamond[4] = { 11, 11, 10, 5 };
int currentAxe = 0;  //
bool MINEABLE = true;
// Time constants
double t = 0.;
double tStart = 0.;
double loadTime = 1750.0;  // Load time before considered failure. (ms)
//char currentBlocks[] = { 'W', 'W', 'W', 'W', 'W', 'W', 'R' };  // Shield test
//char currentBlocks[] = { 'Y', 'Y', 'Y', 'W', 'W', 'W', 'R' };  // Pickaxe test
char currentBlocks[] = { 'E', 'E', 'E', 'E', 'E', 'E', 'E' };  // Blocks on the robot.
int numBlocks = 0;

// Color Sensor
const int colorPins[4] = { 47, 49, 48, 50 };
int readPin = 51;
int numSamples = 3;
int thresholdLow[][3] = { { 60, 0, 0 }, { 30, 30, 0 }, { 0, 10, 50 } };  // cols: R G B rows: Red Yellow Blue
int thresholdHigh[][3] = { { 100, 30, 30 }, { 80, 60, 40 }, { 20, 40, 100 } };
char blockColor;

// Hall Effect Sensor
const float HeThreshold = 1.;  // V
const int HeRead = A5;
float HeVal = 0;
bool SilverFish = false;
float HeVals[5] = { 0 };
const float quiescent = 2.3;
bool NOSHIELD = true;


//////////////////////////////////////////////////////////////
// Driving

// Motor
DualTB9051FTGMotorShieldUnoMega md;
int m1c = 0, m2c = 0;  //declare and initialize motor commands
double Kp = 100;       //Proportional Gain for Line Following
double KiLF = 0;
double base_speed = 100;     //Nominal speed of robot
int MineApproachStop = 3.5;  // Stop distance from the wall (cm)
int MineApproachSlow = 9;    // Distance at which you start slowing at wall approach.
int CraftApproachStop = 2;   // Stop distance from the wall (cm)
int CraftApproachSlow = 7;   // Distance at which you start slowing at wall approach.

// Motor Encoders
Encoder leftEnc(20, 21);   // encoder 1
Encoder rightEnc(18, 19);  // encoder 2
long counts1, counts2;
double t_old = 0.;
double t0 = 0.;
double print_time = 0.;
char position = 'S';
bool DRIVINGDISTANCEUNTILRANGEFINDER = false;

// Constants
double GearRatio = 70.;  // gear ratio
int countsPerRev = 64.;  // encoder counts per Rev
double rw = 5.;          // wheel radius in cm
double D = 28.;          // distance between wheels in cm
const int min_motor_speed = 25;

// Trajectory Following
double KpL = 80;  // Proportional gain for Linear (FWD/BWD) trajectory following
double KpT = 80;  // Proportional gain for TURNING trajectory following
double Ki = 400;  // Integral gain
double Kd = 0.9;  // Derivative gain
bool ISARC;       // Turn in place or drive in arc
double integral1 = 0, integral2 = 0;
double dErrordt1 = 0;
double dErrordt2 = 0;


double theta1, theta1_old = 0, omega1;  //position and velocity of wheel 1
float omega1fOld, omega2fOld = 0;
double theta2, theta2_old = 0, omega2;      //position and velocity of wheel 2
double omega2f, omega1f = 0, alpha = 0.05;  // filtered velocity and filter weight
float theta1_des = 0, theta2_des = 0;       // desired position of wheels
double theta1_final = 0;
double theta2_final = 0;  // final desired position of wheels
float omega1_des = 0;
float omega2_des = 0;
float arrival_threshold = 0.02;
float distance_arrival_threshold = 0.5;
float base_omega = 4;  // Base speed for turning

void setup() {
  // initialize reflectance sensor
  qtrF.setTypeRC();
  qtrF.setSensorPins((const uint8_t[]){ 25, 27, 29, 31, 33, 35, 37, 39 }, SensorCount);
  qtrR.setTypeRC();
  qtrR.setSensorPins((const uint8_t[]){ 24, 26, 28, 30, 32, 34, 36, 38 }, SensorCount);
  pinMode(RENAF, OUTPUT);
  pinMode(RENAR, OUTPUT);

  // Initialize Hall Effect Sensor
  pinMode(HeRead, INPUT);

  // Initialize Color Sensor
  pinMode(colorPins[0], OUTPUT);
  pinMode(colorPins[1], OUTPUT);
  pinMode(colorPins[2], OUTPUT);
  pinMode(colorPins[3], OUTPUT);
  pinMode(readPin, INPUT);

  // Initialize Limit Switches
  pinMode(LS1Pin, INPUT_PULLUP);
  pinMode(LS2Pin, INPUT_PULLUP);

  // Servo Setup
  MiningServo.attach(MiningServoPin);
  TowerServo.attach(TowerServoPin);
  DispenseServo.attach(DispenseServoPin);

  // Set Mining Servo to zero position
  MiningServo.write(mineServoRest, 50, true);

  // Set tower servo to zero position
  TowerServo.write(90, 50, true);

  // Set Dispense Servo to zero position
  DispenseServo.write(dispense1, 50, true);

  // Drive Motor setup
  md.init();
  md.enableDrivers();

  // Belt motor setup
  pinMode(BeltENA2, OUTPUT);
  pinMode(BeltPWM, OUTPUT);
  pinMode(BeltENA1, OUTPUT);

  // Open serial communications with computer and wait for port to open:
  Serial.begin(57600);  // make sure to also select this baud rate in your Serial Monitor window

  // Open serial communications with the other Arduino board
  Serial3.begin(115200);  // this needs to match the mySerial baud rate in UnoSending
  // for wireless comms, it also needs to match the Xbee firmware setting of 115200
}

void loop() {
  switch (MetaState) {
    case 0:  // Receiving Strategic Comms (STRATCOM)
      comms(DEBUGMODE);
      if (START) {
        TowerServo.write(180);
        TOWERDOWN = true;
        delay(10);
        MetaState = 1;
        if (currentAxe == 0 || (currentAxe == 1 && !NOSHIELD)) {
          goal = 'P';
        } else if (currentAxe == 1 && NOSHIELD) {
          goal = 'S';
        } else {
          goal = 'M';
          MetaState = 7;
        }
      }
      break;
    case 1:  // Starting, drive to mine
      DONEWITHCOMMAND = driveTo(defaultMine);
      switch (goal) {
        case 'P':
          if (DONEWITHCOMMAND) MetaState = 2;
          break;
        case 'S':
          if (DONEWITHCOMMAND) MetaState = 3;
          break;
      }

      break;
    case 2:  // Mining while we need ore and have space
      DispenseServo.write(dispense1);
      if (desiredNumOre > 0 && numBlocks < maxNumBlocks) {
        mine();
        if (desiredOre[currentAxe + 1] == currentBlocks[numBlocks - 1]) {
          desiredNumOre = desiredNumOre - 1;
        }
      }
      if (goal == 'P') {
        if (desiredNumOre == 0 && numBlocks <= maxNumBlocks - desiredNumWood) {
          MetaState = 3;  // Ore Aquired, getting wood
        } else if ((numBlocks > maxNumBlocks - desiredNumWood && goal == 'P' && desiredNumOre < 3) || (goal == 'S' && desiredNumOre == 1)) {
          MetaState = 5;  // Drive to crafting
        }
      } else if (goal == 'S') {
        if (desiredNumOre == 0 || numBlocks == maxNumBlocks) {
          MetaState = 5;  // Drive to Crafting
        }
      }
      break;
    case 3:  // Driving to tree
      DONEWITHCOMMAND = driveTo(defaultTree);
      if (DONEWITHCOMMAND) MetaState = 4;
      break;
    case 4:  // Mining wood while we need wood and have space
      DispenseServo.write(dispense1);
      if (desiredNumWood > 0 && numBlocks < maxNumBlocks) {
        mine();
        desiredNumWood = desiredNumWood - 1;
      } else {
        if (goal == 'P') MetaState = 5;
        if (goal == 'S') MetaState = 1;
      }
      break;
    case 5:  // Drive to crafting
      DONEWITHCOMMAND = driveTo('C');
      if (DONEWITHCOMMAND) MetaState = 6;
      break;
    case 6:
      DispenseServo.write(dispense2);
      DONEWITHCOMMAND = craft(goal, desiredOre[currentAxe + 1]);
      if (numBlocks == 0 && !DONEWITHCOMMAND) {  // Out of blocks, not done crafting
        CraftingDistance = 2;
        if (desiredNumOre > 0) {  // Need ore, drive back to mine
          MetaState = 1;
        } else if (desiredNumWood > 0) {  // Need wood, drive back to tree
          MetaState = 3;
        }
      } else if (DONEWITHCOMMAND) {         // Successfully crafted
        if (currentAxe == 1 && NOSHIELD) {  // Stone axe made, make a shield
          goal = 'S';
          desiredNumOre = 1;
          desiredNumWood = 6;
          MetaState = 3;
        } else if (currentAxe == 1) {
          goal = 'P';
          desiredNumOre = 3;
          desiredNumWood = 2;
          MetaState = 1;
        } else if (currentAxe == 2) {  // Mining as much as possible
          goal = 'M';
          if (numBlocks == 0) MetaState = 7;
          else MetaState = 9;
        }
      }
      break;
    case 7:
      DONEWITHCOMMAND = driveTo(defaultMine);
      if (DONEWITHCOMMAND) MetaState = 8;
      break;
    case 8:
      DispenseServo.write(dispense1);
      if (numBlocks < maxNumBlocks) {
        mine();
      } else {
        MetaState = 9;
      }
      break;
    case 9:
      DONEWITHCOMMAND = driveTo("B");
      if (DONEWITHCOMMAND) MetaState = 10;
      break;
    case 10:
      DispenseServo.write(dispense2);
      if (numBlocks > 0) {
        dispense();
      } else {
        MetaState = 7;
      }
      break;
    default:
      break;
  }
  if (MetaState == 0) {
    switch (Command) {
      case 's':
      case 'S':  // Stop current action
        md.setM1Speed(0);
        md.setM2Speed(0);
        analogWrite(BeltPWM, 0);
        digitalWrite(BeltENA1, LOW);
        digitalWrite(BeltENA2, LOW);
        MiningServo.write(mineServoRest);
        DispenseServo.write(dispense1);
        Command = 0;
        DRIVING = false;
        PREDRIVING = true;
        DRIVINGDISTANCEUNTILRANGEFINDER = false;
        drive_to_substate = 0;
        crafting_substate = 0;
        break;
      case 'd':
      case 'D':  // drive to specific target
        DONEWITHCOMMAND = driveTo(direction);
        if (DONEWITHCOMMAND) Command = 0;
        break;
      case 'e':
      case 'E':  // Drive in straight line using encoders
        Command = driveDistance(direction, distAngle, speedRadius, DEBUGMODE) ? 0 : Command;
        break;
      case 't':
      case 'T':  // Set default Tree
        switch (direction) {
          case 'L':
          case 'l':
            defaultTree = 'T';
            Serial3.println("Defaulting to left tree");
            break;
          case 'r':
          case 'R':
            defaultTree = 'A';
            Serial3.println("Defaulting to right tree");
            break;
          default:
            defaultTree = 'T';
            break;
        }
        break;
      case 'l':
      case 'L':  // Line follow F/R
        DONEWITHCOMMAND = lineFollow(direction, distAngle, speedRadius);
        if (DONEWITHCOMMAND) Command = 0;
        break;
      case 'x':
      case 'X':  // Drop Tower
        TowerServo.write(180);
        TOWERDOWN = true;
        break;
      case 'm':
      case 'M':  // Set default mine
        switch (direction) {
          case 'L':
          case 'l':
            defaultMine = 'M';
            Serial3.println("Defaulting to left mine");
            break;
          case 'r':
          case 'R':
            defaultMine = 'K';
            Serial3.println("Defaulting to right mine");
            break;
          default:
            defaultMine = 'M';
            break;
        }
        break;
      case 'u':
      case 'U':  // Load/Unload
        loadUnload(direction);
        break;
      case 'c':
      case 'C':
        {  // Crafting
          DispenseServo.write(dispense2);
          bool DONECRAFT = craft(direction, desiredOre[currentAxe + 1]);
          Command = DONECRAFT ? 0 : Command;
          break;
        }
      case 'v':
      case 'V':
        if (!DEBUGMODE) {
          DEBUGMODE = true;
          Serial3.println("DEBUGMODE ON");
        } else {
          DEBUGMODE = false;
          Serial3.println("DEBUGMODE OFF");
        }
        Command = 0;
        break;
      case 'p':
      case 'P':
        currentAxe = int(distAngle);
        Serial3.print("Current Axe Level: ");
        Serial3.println(currentAxe);
        Command = 0;
        break;
      case 'g':
      case 'G':
        START = true;
        Command = 0;
        break;
      case 'n':
      case 'N':
        if (!NOSHIELD) {
          NOSHIELD = true;
          Serial3.println("NO PROTECTION (shield off)");
        } else {
          NOSHIELD = false;
          Serial3.println("FULL PROTECTION (shield on)");
        }
        Command = 0;
        break;
      default:
        break;
    }
  }
}