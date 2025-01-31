#include <petlib.h>

void setup() {
  Serial.begin(115200);
  sonarInit();
  motorInit();
}

void loop() {
  for (int i = 0; i < 255; i++) {
    setLeft(i);
    setRight(i);
    Serial.print(i);
    Serial.print(" ");
    Serial.println(sonarGetDistance(SONAR_TRIG_LEFT, SONAR_ECHO_LEFT));
    delay(10);
  }
  for (int i = 0; i > -255; i--) {
    setLeft(i);
    setRight(i);
    Serial.print(i);
    Serial.print(" ");
    Serial.println(sonarGetDistance(SONAR_TRIG_RIGHT, SONAR_ECHO_RIGHT));
    delay(10);
  }

}
