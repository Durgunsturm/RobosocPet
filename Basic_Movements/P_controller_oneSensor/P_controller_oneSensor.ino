#define LEFT_PWM 11
#define LEFT_FWD 7
#define LEFT_REV 8

#define RIGHT_PWM 9
#define RIGHT_FWD 4
#define RIGHT_REV 5

#define SONAR_TRIG 2
#define SONAR_ECHO 3

unsigned long int echo_time;
unsigned long int echo_start;

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
  analogWrite(LEFT_PWM, abs(speed));
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
  analogWrite(RIGHT_PWM, abs(speed));
}

void echo_isr() {
  echo_time = micros() - echo_start;
  Serial.println(echo_time);
}

void setup() {
  // put your setup code here, to run once:
  //MOTOR A DIR
  pinMode(7,OUTPUT);
  pinMode(8,OUTPUT);

  //MOTOR B DIR 
  pinMode(5,OUTPUT);
  pinMode(4,OUTPUT);

  //SPEED CTRL
  pinMode(9,OUTPUT);
  pinMode(11,OUTPUT);

  pinMode(SONAR_TRIG, OUTPUT);
  pinMode(SONAR_ECHO, INPUT);
  digitalWrite(SONAR_TRIG, 1);

  Serial.begin(115200);
  
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
  int distance = sonarGetDistance(SONAR_TRIG, SONAR_ECHO);
  int target = 1500;
  Serial.println(distance);
  if (distance) {
    setLeft(min(target - distance, 1000)*-0.2);
    setRight(min(target - distance, 1000)*-0.2);
  } else {
    setLeft(0);
    setRight(0);
  }
  //Serial.println((target - distance)*-0.001);
}
