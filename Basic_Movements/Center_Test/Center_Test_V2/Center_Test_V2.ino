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

#define LOOP_PERIOD 20

#define FWD_FAC 75
#define TURN_FAC 200

#define DEBUG 0

unsigned long int echo_time, echo_start;

unsigned long int distanceLeft, distanceRight = 0;

unsigned long int pMillis;

unsigned int sensor = 0;

void setLeft(int speed) {
  if (!speed) {
    digitalWrite(LEFT_FWD, 0);
    digitalWrite(LEFT_REV, 0);
  } else if (speed > 0) {
    digitalWrite(LEFT_FWD, 1);
    digitalWrite(LEFT_REV, 0);
  } else {
    digitalWrite(LEFT_FWD, 0);
    digitalWrite(LEFT_REV, 1);
  }
  analogWrite(LEFT_PWM, fmin(abs(speed), 127));
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
  digitalWrite(SONAR_TRIG_RIGHT, 1);
  digitalWrite(SONAR_TRIG_LEFT, 1);


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
    if (sensor == 0) {
      distanceLeft = sonarGetDistance(SONAR_TRIG_LEFT, SONAR_ECHO_LEFT);
      sensor++;
    } else if (sensor == 1) {
      distanceRight = sonarGetDistance(SONAR_TRIG_RIGHT, SONAR_ECHO_RIGHT);
      sensor++;
    } else {
      sensor = 0;
      long int diff = distanceLeft - distanceRight;
      long int sum = distanceLeft + distanceRight;
      if (sum) {
        float turn = diff / (float)sum;
        if (DEBUG) {
          Serial.print("L: ");
          Serial.print(distanceLeft);
          Serial.print(", R: ");
          Serial.print(distanceRight);
          Serial.print(", T: ");
          Serial.println(turn * TURN_FAC);
        } else {
          setLeft(FWD_FAC - (turn * TURN_FAC));
          setRight(FWD_FAC + (turn * TURN_FAC));
        }
      } else {
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
