#include "lib.h"

void set_motor_pwm(int fwd_pin, int rev_pin, int pwm_pin, int value) {
    if (!value) {
    digitalWrite(fwd_pin, 0);
    digitalWrite(rev_pin, 0);
  } else if (value > 0) {
    digitalWrite(fwd_pin, 1);
    digitalWrite(rev_pin, 0);
  } else {
    digitalWrite(fwd_pin, 0);
    digitalWrite(rev_pin, 1);
  }
  analogWrite(pwm_pin, fmin(abs(value), 255));
}

void setLeft(int speed) {
    set_motor_pwm(LEFT_FWD, LEFT_REV, LEFT_PWM, speed);
}

void setRight(int speed) {
    set_motor_pwm(RIGHT_FWD, RIGHT_REV, RIGHT_PWM, speed);
}