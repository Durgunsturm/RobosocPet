#include "petlib.h" 
 
int16_t gx; 
int16_t gy; 
int16_t gz; 
MPU6050* imu; 
void initImu() { 
imu = new MPU6050(); 
Wire.begin(); 
Serial.println("Initializing IMU..."); 
imu->initialize(); 
Serial.println("Testing IMU connection..."); 
if(imu->testConnection() ==  false){ 
Serial.println("IMU connection failed"); 
} else { 
Serial.println("IMU connection successful"); 
} 
imu->setZGyroOffset(0.0); 
delay(500); 
Serial.println("IMU calibrating..."); 
double offset = 0.0; 
for (int i = 0; i < 500; i++) { 
imu->getRotation(&gx, &gy, &gz); 
offset += (gz+offset) * -0.0075; 
Serial.println(offset); 
delay(5); 
} 
Serial.print("Z axis offset: "); 
Serial.println(offset); 
imu->setZGyroOffset(offset); 
} 
headingCorrector::headingCorrector(double p_kP, double p_kI, double p_kD, double p_mI) { 
kP = p_kP; 
kI = p_kI; 
kD = p_kD; 
mI = p_mI; 
} 
double headingCorrector::update() { 
imu->getRotation(&gx, &gy, &gz); 
time = micros(); 
dt = (time - pTime) / 1000000.0; 
pTime = time; 
error = setPoint - gz; 
errorRate = (error - pError) / dt; 
if ((error >= 0 && pError <= 0.0) || (error <= 0 && pError >= 0)) { 
errorIntegrator = 0.0; 
} else { 
errorIntegrator += error * dt; 
if (errorIntegrator > mI) { 
errorIntegrator = mI; 
} else if (errorIntegrator < -mI) { 
errorIntegrator = -mI; 
} 
} 
pError = error; 
return (error * kP) + (errorIntegrator * kI) + (errorRate * kD); 
} 
 
void set_motor_pwm(int fwd_pin, int rev_pin, int pwm_pin, int value, char reversed) { 
if (value == 0) { 
digitalWrite(fwd_pin, 0); 
digitalWrite(rev_pin, 0); 
} else if (reversed) { 
digitalWrite(fwd_pin, 0); 
digitalWrite(rev_pin, 1); 
} else { 
digitalWrite(fwd_pin, 1); 
digitalWrite(rev_pin, 0); 
} 
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
