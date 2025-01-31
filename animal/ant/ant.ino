#include <petlib.h>

#define WANDER_FREQ 0.3
#define WANDER_SCALE 0.125
#define MOTOR_BIAS 1.156
#define WANDER_SPEED 90
#define TAU 6.2832

float turn = 0.0;
float speed = 0.0;

void setup() {
  sonarInit();
  motorInit();
  Serial.begin(9600);
}

void loop() {
  float forward = sonarGetDistance(SONAR_TRIG_FOR, SONAR_ECHO_FOR);
  float right = sonarGetDistance(SONAR_TRIG_RIGHT, SONAR_ECHO_RIGHT);
  float left = sonarGetDistance(SONAR_TRIG_LEFT, SONAR_ECHO_LEFT);

  if (right == 0) {
    right = 20000.0;
  }
  if (left == 0) {
    left = 20000.0;
  }
  turn = (750.0/left)-(750.0/right)+(sin((millis() / 1000.0) * WANDER_FREQ * TAU) * WANDER_SCALE);

  if (forward  == 0) {
    speed = WANDER_SPEED * 1.25;
  } else if (forward < 1000) {
    speed = -90;
    turn = 0.5;
  } else if (forward < 3000) {
    speed = WANDER_SPEED * 0.65;
    turn += 0.25;
  } else {
    speed = WANDER_SPEED;
  }

  setLeft((speed+(turn*speed*2.0))*MOTOR_BIAS);
  setRight(speed-(turn*speed*2.0));

  delay(10);

}
