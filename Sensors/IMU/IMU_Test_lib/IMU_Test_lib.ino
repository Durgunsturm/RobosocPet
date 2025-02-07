#include "petlib.h"

headingCorrector* hc;
double headingCorrectionGain;
int forwardSpeed = 80;

double sum = 0;
int count = 0;

void setup() {
  Serial.begin(115200);
  initImu();
  hc = new headingCorrector(0.02, 0.0, 0.0, 0.0);
  
}

void loop() {
  sum += gz;
  count++;
  headingCorrectionGain = hc->update();
  Serial.println(sum/count);
  setLeft(forwardSpeed - headingCorrectionGain);
  setRight(forwardSpeed + headingCorrectionGain);
  delay(10);
}