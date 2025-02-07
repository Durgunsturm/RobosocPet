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