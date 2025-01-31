#include "sonar.h"

unsigned long int sonarGetDistance(int trig, int echo) {
  digitalWrite(trig, 0);
  delayMicroseconds(2);
  digitalWrite(trig, 1);
  delayMicroseconds(10);
  digitalWrite(trig, 0);
  return pulseIn(echo, 1, SONAR_TIMEOUT);
}

void sonarInit() {
    pinMode(SONAR_TRIG_LEFT, OUTPUT);
    pinMode(SONAR_ECHO_LEFT, INPUT);

    pinMode(SONAR_TRIG_RIGHT, OUTPUT);
    pinMode(SONAR_ECHO_RIGHT, INPUT);

    pinMode(SONAR_TRIG_FOR, OUTPUT);
    pinMode(SONAR_ECHO_FOR, INPUT);
    
}