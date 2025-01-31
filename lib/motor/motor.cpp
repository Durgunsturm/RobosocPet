#include "lib.h"

void set_motor_pwm(int fwd_pin, int rev_pin, int pwm_pin, int value, char reversed) {
    if (value == 0) {
    digitalWrite(fwd_pin, 0);
    digitalWrite(rev_pin, 0);
    Serial.print("stopped ");
  } else if (reversed) {
    digitalWrite(fwd_pin, 0);
    digitalWrite(rev_pin, 1);
    Serial.print("fwd ");
  } else {
    digitalWrite(fwd_pin, 1);
    digitalWrite(rev_pin, 0);
    Serial.print("rev ");
  }
  Serial.print((int)fmin(abs(value), 255));
  Serial.print(" ");
  Serial.println(value);
  analogWrite(pwm_pin, (char)fmin(abs(value), 255));
}

void setLeft(int speed) {
    set_motor_pwm(LEFT_FWD, LEFT_REV, LEFT_PWM, abs(speed), speed < 0);
}

void setRight(int speed) {
    set_motor_pwm(RIGHT_FWD, RIGHT_REV, RIGHT_PWM, abs(speed), speed < 0);
}

void motorInit() {
  pinMode(LEFT_FWD, OUTPUT);
  pinMode(LEFT_REV, OUTPUT);
  pinMode(LEFT_PWM, OUTPUT);

  pinMode(RIGHT_FWD, OUTPUT);
  pinMode(RIGHT_REV, OUTPUT);
  pinMode(RIGHT_PWM, OUTPUT);
}