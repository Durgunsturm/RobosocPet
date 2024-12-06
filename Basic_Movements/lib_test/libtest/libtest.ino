#include <petlib.h>

void setup() {
  Serial.begin(115200);

}

void loop() {
  for (int i = 0; i < 255; i++) {
    setLeft(i);
    setRight(i);
    Serial.println(i);
  }
  for (int i = 0; i > -255; i--) {
    setLeft(i);
    setRight(i);
    Serial.println(i);
  }

}
