#define SONAR_TRIG_LEFT 13
#define SONAR_ECHO_LEFT 12

#define SONAR_TRIG_RIGHT 2
#define SONAR_ECHO_RIGHT 3

#define LOOP_PERIOD 50

unsigned long int echo_time, echo_start;

unsigned int distanceLeft, distanceRight;

unsigned long int pMillis;

unsigned int sensor = 0;

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
  if (millis() - pMillis > LOOP_PERIOD) {
    pMillis = millis();
    if (sensor == 0) {
      int distanceLeft = sonarGetDistance(SONAR_TRIG_LEFT, SONAR_ECHO_LEFT);
    } else if (sensor == 1) {
      int distanceRight = sonarGetDistance(SONAR_TRIG_RIGHT, SONAR_ECHO_RIGHT);
    } else {
      sensor = 0;

      Serial.print("L: ")
      Serial.print(distanceLeft);
      Serial.print(", R: ");
      Serial.print(distanceRight);
      Serial.println();
    }
}
