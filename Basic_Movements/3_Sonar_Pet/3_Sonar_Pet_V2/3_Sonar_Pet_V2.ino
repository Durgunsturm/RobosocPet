#define LEFT_PWM 11
#define LEFT_FWD 8
#define LEFT_REV 7

#define RIGHT_PWM 9
#define RIGHT_FWD 5
#define RIGHT_REV 4

#define SONAR_TRIG_LEFT 2
#define SONAR_ECHO_LEFT 3

#define SONAR_TRIG_RIGHT 13
#define SONAR_ECHO_RIGHT 12

#define SONAR_TRIG_FOR A0 //Forward connected to analogue pins as there is no space
#define SONAR_ECHO_FOR A1 //I think analogue pins can be treated like digital pins

#define LOOP_PERIOD 20

#define FWD_FAC 130 //150
#define TURN_FAC 2 //0.4 

#define DEBUG 0

unsigned long int echo_time, echo_start;

unsigned long int distanceLeft, distanceRight, distanceForward = 0;

unsigned long int pMillis;

unsigned int sensor = 0;
int td = 1;

unsigned long int behaviourTime = 10000; //Time spent in a certain behaviour in ms
//Set choice to -1 to disable random behaviours
//Otherwise it is initialised to the first random behaviour
unsigned int choice = 0; //Decision for what idle behaviour should be used (generated randomly)
unsigned long int timer; //Length of time in a behaviour
int choiceNum = 8; //Number of choices of behaviour

void setLeft(int speed) {
  if (!speed) {
    digitalWrite(LEFT_FWD, 0);
    digitalWrite(LEFT_REV, 0); //Zero motion passed to motors
  } else if (speed > 0) {
    digitalWrite(LEFT_FWD, 1); //Forward on
    digitalWrite(LEFT_REV, 0);
  } else {
    digitalWrite(LEFT_FWD, 0);
    digitalWrite(LEFT_REV, 1); //Reverse on
  }
  analogWrite(LEFT_PWM, fmin(abs(speed), 255)); //Caps speed at 127 to avoid overflow errors
  Serial.println(fmin(abs(speed), 255));
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
  analogWrite(RIGHT_PWM, fmin(abs(speed), 255));
  Serial.println(fmin(abs(speed), 255));
}

void setup() {

  pinMode(SONAR_TRIG_LEFT, OUTPUT);
  pinMode(SONAR_ECHO_LEFT, INPUT);

  pinMode(SONAR_TRIG_RIGHT, OUTPUT);
  pinMode(SONAR_ECHO_RIGHT, INPUT);

  pinMode(SONAR_TRIG_FOR, OUTPUT);
  pinMode(SONAR_ECHO_FOR, INPUT);

  digitalWrite(SONAR_TRIG_RIGHT, 1);
  digitalWrite(SONAR_TRIG_LEFT, 1);
  digitalWrite(SONAR_TRIG_FOR, 1);


  Serial.begin(115200); //Baud rate

  randomSeed(analogRead(0)); //Generates a random seed

  pMillis = millis();
  timer = millis();
  
}

unsigned long int sonarGetDistance(int trig, int echo) {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  return pulseIn(echo, HIGH, 10000);
}

void loop() {
  if (millis() - pMillis >= LOOP_PERIOD) {
    pMillis = millis(); //Sets time of execution

    if (millis() - timer >= behaviourTime && choice != -1) { //Checks if behaviourTime has passed before reassigning a new one
      timer = millis();
      choice = random(choiceNum); //random(n) generates random number from 0 to n-1, this is used to change behaviour
    }

    if (sensor == 0) { //Left check
      distanceLeft = sonarGetDistance(SONAR_TRIG_LEFT, SONAR_ECHO_LEFT);
      if (distanceLeft == 0){distanceLeft = 20000;} //Sets undetected distance to very large
      sensor++;
    } else if (sensor == 1) { //Right check
      distanceRight = sonarGetDistance(SONAR_TRIG_RIGHT, SONAR_ECHO_RIGHT);
      if (distanceRight == 0){distanceRight = 20000;} //Sets undetected distance to very large
      sensor++;
    } else if (sensor == 2) { //Forward check
      distanceForward = sonarGetDistance(SONAR_TRIG_FOR, SONAR_ECHO_FOR);
      if (distanceForward == 0){distanceForward = 20000;} //Sets undetected distance to very large
      sensor++;
    } else { //Sets the turning direction
      sensor = 0;
      //long int sum = distanceLeft + distanceRight;

      if (DEBUG) { //Prints distances
        
        Serial.print("L: ");
        Serial.print(distanceLeft);
        Serial.print(", R: ");
        Serial.print(distanceRight);
        Serial.print(", F: ");
        Serial.println(distanceForward);

        Serial.print("Behaviour Choice: ");
        Serial.println(choice);
        
      }

      // Turning algorithms //
      else if (distanceForward < 2000) { //Reverses from an obstacle infront
        Serial.print("Obstacle ahead: "); //TESTING
        Serial.println(distanceForward); //TESTING

        setLeft(-255-td);
        setRight(-255+td);

      } else if (distanceLeft < 2000 || distanceRight < 2000) {//Turns away from obstacles
        Serial.println("Obstacle on the side"); //TESTING
        float turn;
        if (distanceLeft < distanceRight) {
          turn = 20000.0 / distanceLeft;
        } else {
          turn = (-20000.0) / distanceRight;
        }

        if (turn > 0) {
          td = 128; //Used to remember which side a wall was detected
        } else if (turn < 0) {
          td = -128;
        }
        setLeft((TURN_FAC*turn) + FWD_FAC);
        setRight(FWD_FAC -(TURN_FAC*turn));
          
          /* //Testing
          setLeft(-50);
          setRight(-50);
          */

      // Idle behaviour algorithms //
      } else { //No turn
        Serial.print("Running idle command: ");
        Serial.println(choice); //TESTING
        switch (choice){
          case 0: //Clockwise Spin
            setLeft(150);
            setRight(-150);
            break;
          case 1: //Anticlockwise Spin
            setLeft(-150);
            setRight(150);
            break;
          case 2: //Struggles to move on the spot
            setLeft(random(-255,256));
            setRight(random(-255,256));
            break;
          case 3: //Moves in a random direction
            //Not implemented
            setLeft(0);
            setRight(0);
            break;
          case 4: //Freezes
            setLeft(0);
            setRight(0);
            break;
          default: //It just moves forward
            setLeft(FWD_FAC);
            setRight(FWD_FAC);
            break;
        }
      }
    }
  }
}