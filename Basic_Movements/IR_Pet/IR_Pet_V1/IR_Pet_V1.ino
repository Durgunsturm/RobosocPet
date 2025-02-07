#include "VL53L1X.h"
#include "petlib.h"

#define SONAR_TRIG_LEFT 13
#define SONAR_ECHO_LEFT 12

#define SONAR_TRIG_RIGHT 2
#define SONAR_ECHO_RIGHT 3

#define HIGH_SPEED 1
#define RANGE_MODE VL53L1X::Medium

unsigned long int echo_time, echo_start;

unsigned long int distanceLeft, distanceRight = 0;

unsigned long int pMillis;

unsigned int sensor = 0;

unsigned long int distanceIRForward; //Distance detected by forward IR
#define fwdSpd 130

#define DEBUG 0

VL53L1X* ir;

void setup() {

  pinMode(SONAR_TRIG_LEFT, OUTPUT);
  pinMode(SONAR_ECHO_LEFT, INPUT);

  pinMode(SONAR_TRIG_RIGHT, OUTPUT);
  pinMode(SONAR_ECHO_RIGHT, INPUT);
  digitalWrite(SONAR_TRIG_RIGHT, 1);
  digitalWrite(SONAR_TRIG_LEFT, 1);


  Serial.begin(115200);

  ir = new VL53L1X();

  Wire.begin();
  ir->setTimeout(500);
  ir->init();
  ir->setDistanceMode(RANGE_MODE);
  int budget = HIGH_SPEED ? 33000 : 140000;
  ir->setMeasurementTimingBudget(budget);
  ir->startContinuous(50);
  Serial.println("...Init...");
  
}

void loop() {
    ir->read();
    distanceIRForward = ir->ranging_data.range_mm;

    if (DEBUG){
        Serial.println(distanceIRForward);
    } else {
        if (distanceIRForward > 1000 || true){ //Forward
            setLeft(fwdSpd);
            setRight(fwdSpd);
        } else { //Reverse
            setLeft(fwdSpd*-1);
            setRight(fwdSpd*-1);
        }
    }
}
