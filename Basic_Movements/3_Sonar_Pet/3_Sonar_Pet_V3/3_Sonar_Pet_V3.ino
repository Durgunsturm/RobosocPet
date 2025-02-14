#include <petlib.h>

#define LOOP_PERIOD 20

#define FWD_FAC 130 //150
#define TURN_FAC 2 //0.4 

#define DEBUG 1

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

unsigned int prevChoice = -1; //Previous choice to allow for new behaviour to be set
unsigned int decideLeft = 0; //Left choice for movement
unsigned int decideRight = 0;

void setup() {

  sonarInit();
  motorInit();


  Serial.begin(115200); //Baud rate

  randomSeed(analogRead(0)); //Generates a random seed

  //pMillis = millis();
  //timer = millis();
  
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
            prevChoice = choice;
            setLeft(150);
            setRight(-150);
            break;
          case 1: //Anticlockwise Spin
            prevChoice = choice;
            setLeft(-150);
            setRight(150);
            break;
          case 2: //Struggles to move on the spot
            prevChoice = choice;
            setLeft(random(-255,256));
            setRight(random(-255,256));
            break;
          case 3: //Moves in a random direction
            if (prevChoice != choice){
                prevChoice = choice; //Stops random motion from being re-generated
                decideLeft = random(-255,256);
                decideRight = random(-255,256);
            }
            setLeft(decideLeft);
            setRight(decideRight);
            break;
          case 4: //Freezes
            prevChoice = choice;
            setLeft(0);
            setRight(0);
            break;
          default: //It just moves forward
            prevChoice = choice;
            setLeft(FWD_FAC);
            setRight(FWD_FAC);
            break;
        }
      }
    }
  }
}