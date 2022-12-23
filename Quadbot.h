#include <Servo.h>
#ifndef _Quadbot_h_
#define _Quadbot_h_

const int numberOfServos = 8; // Number of servos
const int servoPins[] = {16, 5, 4, 0, 2, 14, 12, 13};

class Quadbot {

  private:

  Servo servo[numberOfServos]; // Servo object

  public:

  Quadbot() {

  }

  void setup() {
    servo[0].attach(servoPins[0]);
    servo[0].write(90);
  }

  void servoUp() {
    servo[0].write(150);
  }

  void servoDown() {
    servo[0].write(75);
  }

  void moveServoTo(int servoIndex, int position) {
    servo[servoIndex].write(position);
  }
};

#endif