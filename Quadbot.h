#include <Servo.h>
#include "SimpleIK.h"
#include "Types.h"

#ifndef _Quadbot_h_
#define _Quadbot_h_

typedef struct {
  int min;
  int max;
} ServoMinMax;

ServoMinMax defaultMinMax = {500, 2400};

const int numberOfServos = 8; // Number of servos
const int servoPins[] = {16, 5, 4, 0, 2, 14, 12, 13};
const int initPos[] = {0, 80, 170, 80, 10, 80, 170, 80};
const int armJoints[4][2] = {
// arm, shoulder
  {0, 1},
  {2, 3},
  {4, 5},
  {6, 7}
};

const double shoulderLen = 41.5;
const double groundClearance = 72.0;
SimpleIK simpleIk(groundClearance, shoulderLen);

ServoMinMax ranges[numberOfServos] = {
  defaultMinMax,
  defaultMinMax,
  defaultMinMax,
  defaultMinMax,
  defaultMinMax,
  defaultMinMax,
  defaultMinMax,
  defaultMinMax,
};

class Quadbot {

  private:

  Servo servo[numberOfServos]; // Servo object
  double armPos[numberOfServos] = { 0, 90, 180, 90,  0, 90, 180, 90};
  int offsets[numberOfServos] =   {45,  0, -25,  0, 25,  0, -20,  0};

  public:

  Quadbot() {
  }

  double* getArmPos() {
    return armPos;
  }

  void setup() {
    Serial.println("Setting up servos");
    for (int i = 0; i < numberOfServos; i++) {
      int servoAngle = initPos[i] + offsets[i];
      Serial.printf("Attaching %d and writing %d, %d, %d\n", i, servoAngle, ranges[i].min, ranges[i].max);
      
      servo[i].attach(servoPins[i]); //, ranges[i].min, ranges[i].max);
      servo[i].write(servoAngle);
    }
  }

  void reset() {
    for (int i = 0; i < numberOfServos; i++) {
      armPos[i] = initPos[i] + offsets[i];
      Serial.printf("Moving servo %d to %f\n", i, armPos[i]);
      servo[i].write(armPos[i]);
    }
  }

  Indicies moveServoTo(int legIndex, double x, double z) {
    int armIndex = armJoints[legIndex][0];
    int shoulderIndex = armJoints[legIndex][1];

    double ikShoulderAngle = simpleIk.calcTheta(x, z);
    double ikArmAngle = simpleIk.calcAlpha(x, z);
    double shoulderAngle = map(ikShoulderAngle, 0, 90, 0, 180);
    double armAngle = map(ikArmAngle, 0, 90, 0, 180);

    double armFrom = armPos[armIndex];
    double shoulderFrom = armPos[shoulderIndex];
    if (legIndex % 2 == 0) {
      armPos[armIndex] = 180 - armAngle;
      armPos[shoulderIndex] = 180 - shoulderAngle;
    } else {
      armPos[armIndex] = armAngle;
      armPos[shoulderIndex] = shoulderAngle;      
    }

//    Serial.printf("ikShoulderAngle: %f, ikArmAngle: %f\n", ikShoulderAngle, ikArmAngle);
//    Serial.printf("armAngle: %f, shoulderAngle: %f\n", armPos[armIndex], armPos[shoulderIndex]);
//    Serial.printf("Move shoulder %d to %f and arm %d to %f\n", shoulderIndex, shoulderAngle, armIndex, armAngle);

    servo[shoulderIndex].write((int) armPos[shoulderIndex]);
    servo[armIndex].write((int) armPos[armIndex]);

    Indicies idx = {armIndex, shoulderIndex, armFrom, armPos[armIndex], shoulderFrom, armPos[shoulderIndex]};
    return idx;
  }

  void setServoAngle(ServoAngle angle) {
    servo[angle.index].write(angle.angle);
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

    // ServoIK movements[numSteps] = {
    //   {0, 77, 77}, {1, 40, 40}
      
    // }

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

  void moveArms() {
    int subtractors[numberOfServos] = {
      0, 0, 180, 0, 0, 180, 180, 180
    };

    for (int i = 0; i < numberOfServos; i++) {
      for (int j = 0; j <= 180; j += 5) {
        servo[i].write(abs(j - subtractors[i]));
      }
      delay(3);

      for (int k = 180; k >= 0; k -= 5) {
        servo[i].write(abs(k - subtractors[i]));
      }
      delay(3);
    }

    reset();
  }

  void moveArmOne() {
    Serial.println("Moving arm 1");
    double m = -2.73494;

    for (int i = 0; i < 4; i++) {
      for (double x = 0; x <= 41.5; x += 0.5) {
         double y = m * x + 113.5;
  
         Indicies idx = moveServoTo(0, x, y);
         Serial.printf("Moved arm %d from %f to %f\n", idx.armIndex, idx.armFrom, idx.armTo);
         Serial.printf("Moved shoulder %d from %f to %f\n", idx.shoulderIndex, idx.shoulderFrom, idx.shoulderTo);
         delay(5);
      }
  
      for (double x = 0; x <= 41.5; x += 0.5) {
         double y = m * x + 113.5;
  
         Indicies idx = moveServoTo(0, y, x);
         Serial.printf("Moved arm %d from %f to %f\n", idx.armIndex, idx.armFrom, idx.armTo);
         Serial.printf("Moved shoulder %d from %f to %f\n", idx.shoulderIndex, idx.shoulderFrom, idx.shoulderTo);
         delay(5);
      }
    }
  }
};

#endif
