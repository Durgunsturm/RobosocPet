#define SONAR_TRIG_LEFT 13
#define SONAR_ECHO_LEFT 12

#define SONAR_TRIG_RIGHT 2
#define SONAR_ECHO_RIGHT 3

unsigned long int echo_time;
unsigned long int echo_start;

void setup() {

  pinMode(SONAR_TRIG_LEFT, OUTPUT);
  pinMode(SONAR_ECHO_LEFT, INPUT);

  pinMode(SONAR_TRIG_RIGHT, OUTPUT);
  pinMode(SONAR_ECHO_RIGHT, INPUT);
  digitalWrite(SONAR_TRIG_RIGHT, 1);
  digitalWrite(SONAR_TRIG_LEFT, 1);


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

  delay(5);
  int distanceLeft = sonarGetDistance(SONAR_TRIG_LEFT, SONAR_ECHO_LEFT);
  delay(5);
  int distanceRight = sonarGetDistance(SONAR_TRIG_RIGHT, SONAR_ECHO_RIGHT);

  Serial.print(distanceLeft);
  Serial.print("   ");
  Serial.print(distanceRight);
  Serial.print("   ");

  Serial.println(70.0*((distanceLeft - distanceRight)/(distanceLeft + distanceRight)));

}
