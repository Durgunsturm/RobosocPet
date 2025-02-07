#include <MPU6050.h>
#include "petlib.h"

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

int forwardBias = 80;

int16_t gx, gy, gz;

MPU6050* imu;

void setup() {

  Serial.begin(115200);

  imu = new MPU6050();

  Wire.begin();
  Serial.println("Initializing IMU...");
  imu->initialize();
  Serial.println("Testing IMU connection...");
  if(imu->testConnection() ==  false){
    Serial.println("IMU connection failed");
    while(true);
  } else {
    Serial.println("IMU connection successful");
  }

  imu->setXGyroOffset(0);
  imu->setYGyroOffset(0); 
  imu->setZGyroOffset(0);
  
}

void loop() {
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
    if (erorIntegrator > mI) {
      errorIntegrator = mI;
    } else if (errorIntegrator < -mI) {
      errorIntegrator = -mI;
    }
  }
  pError = error;

  headingCorrectionGain = (error * kP) + (errorIntegrator * kI) + (errorRate * kD);

  /*Print the obtained data on the defined format*/
    Serial.print(gx); Serial.print(", ");
    Serial.print(gy); Serial.print(", ");
    Serial.println(gz);
    setLeft(forwardBias + headingCorrectionGain);
    setRight(forwardBias - headingCorrectionGain);

    delay(10);
}
