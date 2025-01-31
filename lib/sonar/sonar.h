#define SONAR_TRIG_LEFT 2
#define SONAR_ECHO_LEFT 3

#define SONAR_TRIG_RIGHT 13
#define SONAR_ECHO_RIGHT 12

#define SONAR_TRIG_FOR A0 //Forward connected to analogue pins as there is no space
#define SONAR_ECHO_FOR A1 //I think analogue pins can be treated like digital pins

#define SONAR_TIMEOUT 20000

unsigned long int sonarGetDistance(int trig, int echo);
void sonarInit();