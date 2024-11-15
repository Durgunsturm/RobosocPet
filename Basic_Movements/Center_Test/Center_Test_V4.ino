/* UNFINISHED BTW
Left sonar - For an example
GND -> Ground breadboard (blue wires)
Echo -> Pin 3
Trig -> Pin 2
Vcc -> Power breadboard

Front sonar - Not working for some reason
GND -> Ground breadboard
Echo -> Pin A0
Trig -> Pin A1
Vcc -> Power breadboard

*/

#define LEFT_PWM 11
#define LEFT_FWD 8
#define LEFT_REV 7

#define RIGHT_PWM 9
#define RIGHT_FWD 5
#define RIGHT_REV 4

#define SONAR_TRIG_LEFT 2
#define SONAR_ECHO_LEFT 3

#define SONAR_TRIG_RIGHT 13
#define SONAR_ECHO_RIGHT 12

#define SONAR_TRIG_FOR A1 //Forward connected to analogue pins as there is no space
#define SONAR_ECHO_FOR A0 //I think analogue pins can be treated like digital pins

#define LOOP_PERIOD 20

#define FWD_FAC 100
#define TURN_FAC 50

#define DEBUG 0

unsigned long int echo_time, echo_start;

unsigned long int distanceLeft, distanceRight, distanceForward = 0;

unsigned long int pMillis;

unsigned int sensor = 0;

void setLeft(int speed) {
  if (!speed) {
    digitalWrite(LEFT_FWD, 0);
    digitalWrite(LEFT_REV, 0); //Zero motion passed to motors
  } else if (speed > 0) {
    digitalWrite(LEFT_FWD, 1); //Forward on
    digitalWrite(LEFT_REV, 0);
  } else {
    digitalWrite(LEFT_FWD, 0);
    digitalWrite(LEFT_REV, 1); //Reverse on
  }
  analogWrite(LEFT_PWM, fmin(abs(speed), 127)); //Caps speed at 127 to avoid overflow errors
}

void setRight(int speed) {
  if (!speed) {
    digitalWrite(RIGHT_FWD, 0);
    digitalWrite(RIGHT_REV, 0);
  } else if (speed > 0) {
    digitalWrite(RIGHT_FWD, 1);
    digitalWrite(RIGHT_REV, 0);
  } else {
    digitalWrite(RIGHT_FWD, 0);
    digitalWrite(RIGHT_REV, 1);
  }
  analogWrite(RIGHT_PWM, fmin(abs(speed), 127));
}

void setup() {

  pinMode(SONAR_TRIG_LEFT, OUTPUT);
  pinMode(SONAR_ECHO_LEFT, INPUT);

  pinMode(SONAR_TRIG_RIGHT, OUTPUT);
  pinMode(SONAR_ECHO_RIGHT, INPUT);

  pinMode(SONAR_TRIG_FOR, OUTPUT);
  pinMode(SONAR_ECHO_FOR, INPUT);

  digitalWrite(SONAR_TRIG_RIGHT, 1);
  digitalWrite(SONAR_TRIG_LEFT, 1);
  digitalWrite(SONAR_TRIG_FOR, 1);


  Serial.begin(115200);

  pMillis = millis();
  
}

unsigned long int sonarGetDistance(int trig, int echo) {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  return pulseIn(echo, HIGH, 10000);
}

void loop() {
  if (millis() - pMillis >= LOOP_PERIOD) {
    pMillis = millis();
    if (sensor == 0) { //Left check
      distanceLeft = sonarGetDistance(SONAR_TRIG_LEFT, SONAR_ECHO_LEFT);
      if (distanceLeft == 0){distanceLeft = 20000;} //Sets undetected distance to very large
      sensor++;
    } else if (sensor == 1) { //Right check
      distanceRight = sonarGetDistance(SONAR_TRIG_RIGHT, SONAR_ECHO_RIGHT);
      if (distanceRight == 0){distanceRight = 20000;} //Sets undetected distance to very large
      sensor++;
    } else if (sensor == 2) { //Forward check
      distanceForward = sonarGetDistance(SONAR_TRIG_FOR, SONAR_ECHO_FOR);
      if (distanceForward == 0){distanceForward = 20000;} //Sets undetected distance to very large
      sensor++;
    } else { //Sets the turning direction
      sensor = 0;
      long int diff = distanceLeft - distanceRight;
      long int sum = distanceLeft + distanceRight;
      if (distanceForward < 1000) { //Reverses from an obstacle infront
        //Temporary distance back assigned
        setLeft(0); //For some reason this is not being triggered
        setRight(0);
      } else if (sum) {
        float turn = diff / (float)sum;
        if (DEBUG) {
          Serial.print("L: ");
          Serial.print(distanceLeft);
          Serial.print(", R: ");
          Serial.print(distanceRight);
          Serial.print(", T: ");
          Serial.println(turn * TURN_FAC);
        } else { //Turns away from obstacles
          if (distanceLeft < distanceRight) {
            turn = 20000.0 / distanceLeft;
          } else {
            turn = (-20000.0) / distanceRight;
          }
            setLeft((2*turn) + 75.0);
            setRight(75.0-(2*turn));
          }
          
          /* //Testing
          setLeft(-50);
          setRight(-50);
          */
      } else { //No turn
        if (DEBUG) {
          Serial.print("L: ");
          Serial.print(distanceLeft);
          Serial.print(", R: ");
          Serial.println(distanceRight);
        } else {
          setLeft(0);
          setRight(0);
        }
      }
    }
  }
}