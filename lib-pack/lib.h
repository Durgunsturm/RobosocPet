 
#pragma once

#define LEFT_PWM 11
#define LEFT_FWD 8
#define LEFT_REV 7

#define RIGHT_PWM 9
#define RIGHT_FWD 5
#define RIGHT_REV 4

void set_motor_pwm(int fwd_pin, int rev_pin, int pwm_pin, char value, char reversed);
void setLeft(int speed);
void setRight(int speed);Test
header
file