#define LEFT_PWM 11
#define LEFT_FWD 7
#define LEFT_REV 8

#define RIGHT_PWM 9
#define RIGHT_FWD 4
#define RIGHT_REV 5


void setLeft(int speed) {
  if (!speed) {
    digitalWrite(LEFT_FWD, 0);
    digitalWrite(LEFT_REV, 0);
  } else if (speed > 0) {
    digitalWrite(LEFT_FWD, 1);
    digitalWrite(LEFT_REV, 0);
  } else {
    digitalWrite(LEFT_FWD, 0);
    digitalWrite(LEFT_REV, 1);
  }
  analogWrite(LEFT_PWM, abs(speed));
}

void setRight(int speed) {
  if (!speed) {
    digitalWrite(RIGHT_FWD, 0);
    digitalWrite(RIGHT_REV, 0);
  } else if (speed > 0) {
    digitalWrite(RIGHT_FWD, 1);
    digitalWrite(RIGHT_REV, 0);
  } else {
    digitalWrite(RIGHT_FWD, 0);
    digitalWrite(RIGHT_REV, 1);
  }
  analogWrite(RIGHT_PWM, abs(speed));
}


void setup() {
  // put your setup code here, to run once:
  //MOTOR A DIR
  pinMode(7,OUTPUT);
  pinMode(8,OUTPUT);

  //MOTOR B DIR 
  pinMode(5,OUTPUT);
  pinMode(4,OUTPUT);

  //SPEED CTRL
  pinMode(9,OUTPUT);
  pinMode(11,OUTPUT);

  Serial.begin(115200);
  
}


void loop() {
  setRight(100);
  setLeft(100);
}
