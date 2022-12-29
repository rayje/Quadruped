#include "math.h"
#include "SimpleIK.h"

_SimpleIK SimpleIK;

_SimpleIK::_SimpleIK() {
  _groundClearance = 0.0;
  _shoulderLen = 0.0;
}

void _SimpleIK::init(HardwareSerial *serial, double groundClearance, double shoulderLen) {
  _groundClearance = groundClearance;
  _shoulderLen = shoulderLen;
  Serial = serial;
}

double _SimpleIK::getGroundClearance() {
  return _groundClearance;
}
  
double _SimpleIK::calcTheta(int x, int z) {
  return atan2(x, z) * 180 / PI;
}

double _SimpleIK::calcDist(int x, int z) {
  return sqrt((x * x) + (z * z));
}

double _SimpleIK::calcAlpha(int x, int y, int z) {

  float adjustedY = y;
  if (y == _groundClearance) {
    adjustedY = _groundClearance - 0.1;
  }
  
  double dist = calcDist(x, z);
  double theta = calcTheta(x, z);
  Serial->printf("dist: %f, theta: %f\n", dist, theta);

  double yOffset = _groundClearance - adjustedY;
  double d = dist - _shoulderLen;
  double R = sqrt((d * d) + (yOffset * yOffset));

  double a = R;
  double b = yOffset;
  double c = d;
  Serial->printf("R: %f, yOffset: %f, d: %f\n", R, yOffset, d);
  
  double angleInRad = acos((pow(a, 2) + pow(b, 2) - pow(c, 2)) / (2 * a * b));
  Serial->printf("Angle In Rad: %f\n", angleInRad);
  return angleInRad * 180 / PI;
}