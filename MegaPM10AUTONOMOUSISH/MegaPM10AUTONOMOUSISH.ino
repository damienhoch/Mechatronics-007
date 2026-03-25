#include <VarSpeedServo.h>
#include <DualTB9051FTGMotorShieldUnoMega.h>
#include <QTRSensors.h>
#include <Encoder.h>
VarSpeedServo TowerServo;
VarSpeedServo MiningServo;

//////////////////////////////////////////////////////////////
// DEBUG
bool DEBUGMODE = false;
int autonomous_speed = 3;
int drive_to_substate = 0;

//////////////////////////////////////////////////////////////
//Communication Variables
char Command = 0;
char direction;
double distAngle;    // Distance / Angle input
double speedRadius;  // Speed / Radius of turn

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
int MineDelay = 180;  // Amount of time to wait between mining strokes
int mineStrokes;
const int mineServoRest = 45;
const int mine1 = 160;
const int mine2 = 150;

//////////////////////////////////////////////////////////////
// General Booleans
bool FWDCAP = true;  // Space to move ahead of the robot
bool REVCAP = true;   // Space to move behind the robot
bool TOWERDOWN = false;
bool BLOCKMINED = false;
bool SENSING = true;
bool PREDRIVING = true;  // Bool to instantiate
bool DRIVING = true;
bool DONEWITHCOMMAND = false;

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
int currentAxe = 1;  // For PM 10, assume you start with a stone pickaxe
bool MINEABLE = true;
// Time constants
double t = 0.;
double tStart = 0.;

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
int m1c = 0, m2c = 0;       //declare and initialize motor commands
double Kp = 100;            //Proportional Gain for Line Following
double base_speed = 100;    //Nominal speed of robot
int MineApproachStop = 2.5;   // Stop distance from the wall (cm)
int MineApproachSlow = 7;   // Distance at which you start slowing at wall approach.
int CraftApproachStop = 2;  // Stop distance from the wall (cm)
int CraftApproachSlow = 5;  // Distance at which you start slowing at wall approach.

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
double KpL = 10;  // Proportional gain for Linear (FWD/BWD) trajectory following
double KpT = 2;   // Proportional gain for TURNING trajectory following
double Ki = 3;    // Integral gain
bool ISARC;       // Turn in place or drive in arc
double integral1 = 0, integral2 = 0;


double theta1, theta1_old = 0, omega1;  //position and velocity of wheel 1
float omega1fOld, omega2fOld = 0;
double theta2, theta2_old = 0, omega2;     //position and velocity of wheel 2
double omega2f, omega1f = 0, alpha = 0.1;  // filtered velocity and filter weight
float theta1_des = 0, theta2_des = 0;      // desired position of wheels
double theta1_final = 0;
double theta2_final = 0;  // final desired position of wheels
float omega1_des = 0;
float omega2_des = 0;
float arrival_threshold = 0.01;
float base_omega = 2;  // Base speed for turning

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
  // Set Mining Servo to zero position
  MiningServo.write(mineServoRest);

  // Set tower servo to zero position
  TowerServo.write(90);

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
  comms(DEBUGMODE);  // Check Serial Communication, set commands, inputs, etc
  // DEBUGMODE adds serial prints
  // Commands
  switch (Command) {
    case 's':
    case 'S':  // Stop current action
      md.setM1Speed(0);
      md.setM2Speed(0);
      analogWrite(BeltPWM, 0);
      digitalWrite(BeltENA1, LOW);
      digitalWrite(BeltENA2, LOW);
      MiningServo.write(mineServoRest);
      Command = 0;
      DRIVING = false;
      PREDRIVING = true;
      DRIVINGDISTANCEUNTILRANGEFINDER = false;
      drive_to_substate = 0;
      break;
    case 'd':
    case 'D':  // drive to specific target
      DONEWITHCOMMAND = driveTo(direction);
      if(DONEWITHCOMMAND) Command = 0;
      break;
    case 'e':
    case 'E':  // Drive in straight line using encoders
      Command = driveDistance(direction, distAngle, speedRadius, DEBUGMODE) ? Command : 0;
      break;
    case 't':
    case 'T':  // Turn, either in an arc or in place
      DONEWITHCOMMAND = turnDeg(ISARC, speedRadius, distAngle, DEBUGMODE);
      if (DONEWITHCOMMAND) Command = 0;
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
    case 'M':  // Mine
      mine();
      break;
    case 'u':
    case 'U':  // Load/Unload
      loadUnload(direction);
      break;
    case 'c':
    case 'C':  // Crafting unload
      craft();
      break;
    default:
      break;
  }
}