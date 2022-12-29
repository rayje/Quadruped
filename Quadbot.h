#include <Servo.h>
#include "SimpleIK.h"

#ifndef _Quadbot_h_
#define _Quadbot_h_


const int numberOfServos = 8; // Number of servos
const int servoPins[] = {16, 5, 4, 0, 2, 14, 12, 13};
const int initPos[] = {0, 90, 180, 90, 0, 90, 180, 90};
const int armJoints[4][2] = {
// arm, shoulder
  {0, 1},
  {2, 3},
  {4, 5},
  {6, 7}
};

class Quadbot {

  private:

  Servo servo[numberOfServos]; // Servo object
  double shoulderLen = 41.5;
  double groundClearance = 72;
  int armPos[numberOfServos] = {0, 90, 180, 90, 0, 90, 180, 90};

  public:

  Quadbot() {
    SimpleIK.init(&Serial, groundClearance, shoulderLen);
  }

  int* getArmPos() {
    return armPos;
  }

  void setup() {
    for (int i = 0; i < numberOfServos; i++) {
      servo[i].attach(servoPins[i]);
      servo[i].write(initPos[i]);
    }
  }

  void reset() {
    for (int i = 0; i < numberOfServos; i++) {
      armPos[i] = initPos[i];
      servo[i].write(armPos[i]);
    }
  }

  void moveServoTo(int legIndex, int x, int y, int z) {
    int armIndex = armJoints[legIndex][0];
    int shoulderIndex = armJoints[legIndex][1];

    double ikShoulderAngle = SimpleIK.calcTheta(x, z);
    double ikArmAngle = SimpleIK.calcAlpha(x, y, z);
    
    double shoulderAngle = map(ikShoulderAngle, 0, 90, 0, 180);
    double armAngle = map(ikArmAngle, 0, 90, 0, 180);

    armPos[armIndex] = armAngle;
    armPos[shoulderIndex] = shoulderAngle;

    servo[shoulderIndex].write(armPos[armIndex]);
    servo[armIndex].write(armPos[shoulderIndex]);
  }

  void moveServoMax() {
    servo[1].write(180);
    servo[3].write(0);
    servo[5].write(180);
    servo[7].write(0);
  }

  void moveServoMin() {
    servo[1].write(0);
    servo[3].write(180);
    servo[5].write(0);
    servo[7].write(180);
  }

  void moveLegMax() {
    servo[0].write(180);
    servo[2].write(0);
    servo[4].write(180);
    servo[6].write(0);
  }

  void moveLegMin() {
    servo[0].write(0);
    servo[2].write(180);
    servo[4].write(0);
    servo[6].write(180);
  }

  void forward(int forwardSteps, int delayMs) {
    //              1       2        3       4
    // initial -  {  0, 90, 180,  90,  0, 90, 180, 90}
    
    int numSteps = 6;
    int steps[numSteps][numberOfServos] = {
    //    1   |    2    |   3    |    4
      {  0,  0, 180, 180,  0,  90, 180, 90},
      {180, 90, 180, 180,  0,  90, 180, 90},
      {  0, 90, 180,  90, 90,  90, 180, 180},
      {  0, 90, 180,  90,  0,   0, 180, 180},
      {  0, 90, 180,  90,  0,   0,  90, 90},
      {  0, 0, 90,  90,  0,   90, 180, 90},
    };

    for (int k = 0; k < forwardSteps; k++) {
      for (int i = 0; i < numSteps; i++) {
        for (int j = 0; j < numberOfServos; j++) {
          servo[j].write(steps[i][j]);
        }
        delay(delayMs);
      }
    }
  }

  void moveMicro(int servoIndex, int microSeconds) {
    servo[servoIndex].writeMicroseconds(microSeconds);
  }
};

#endif
