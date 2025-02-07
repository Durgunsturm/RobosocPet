#pragma once 
#include <Arduino.h> 
 
#include <MPU6050.h>

extern int16_t gx;
extern int16_t gy;
extern int16_t gz;
extern MPU6050* imu;

void initImu();

class headingCorrector {
public:
  unsigned long int pTime;
  double kP = 1.0;
  double kI = 0.0;
  double mI = 0.0;
  double kD = 0.0;
  double errorRate = 0.0;
  double errorIntegrator = 0.0;
  double pError = 0.0;
  double setPoint = 0.0;
  double error = 0.0;
  double dt = 0.0;
  unsigned long int time;
  double headingCorrectionGain;

  headingCorrector(double p_kP, double p_kI, double p_kD, double p_mI);
  double update();
}; 
#define LEFT_PWM 11
#define LEFT_FWD 8
#define LEFT_REV 7

#define RIGHT_PWM 9
#define RIGHT_FWD 5
#define RIGHT_REV 4

void set_motor_pwm(int fwd_pin, int rev_pin, int pwm_pin, int value, char reversed);
void setLeft(int speed);
void setRight(int speed);

void motorInit(); 
#define SONAR_TRIG_LEFT 2
#define SONAR_ECHO_LEFT 3

#define SONAR_TRIG_RIGHT 13
#define SONAR_ECHO_RIGHT 12

#define SONAR_TRIG_FOR A0 //Forward connected to analogue pins as there is no space
#define SONAR_ECHO_FOR A1 //I think analogue pins can be treated like digital pins

#define SONAR_TIMEOUT 20000

unsigned long int sonarGetDistance(int trig, int echo);
void sonarInit();