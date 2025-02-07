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