#include <MPU6050.h>
#include "petlib.h"

unsigned long int pMillis;

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

  /*Print the obtained data on the defined format*/
    Serial.print(gx); Serial.print(", ");
    Serial.print(gy); Serial.print(", ");
    Serial.println(gz);
    setLeft(80+(gz*0.01));
    setRight(80+(gz*-0.01));
    delay(50);
}
