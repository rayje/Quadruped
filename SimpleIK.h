#ifndef _SimpleIK_h_
#define _SimpleIK_h_

class SimpleIK {


  private:
  double _groundClearance;
  double _shoulderLen;

  double calcDist(double x, double z) {
    return sqrt((x * x) + (z * z));
  }

  public:

  SimpleIK(double groundClearance, double shoulderLen) {
    _groundClearance = groundClearance;
    _shoulderLen = shoulderLen;
  }

  double _getGroundClearance() {
    return _groundClearance;
  }
    
  double calcTheta(double x, double z) {
    return atan2(x, z) * 180 / PI;
  }

  double calcAlpha(double x, double y, double z) {
    double adjustedY = y;
    if (y == _groundClearance) {
      adjustedY = _groundClearance - 0.1;
    }
    
    double dist = calcDist(x, z);
    Serial.printf("dist: %f\n", dist);
    
    double a = _groundClearance;
    double b = _groundClearance - adjustedY;
    double c = dist - _shoulderLen;

    Serial.printf("a: %f, b: %f, c: %f\n", a, b, c);
    
    double angleInRad = acos((pow(a, 2) + pow(b, 2) - pow(c, 2)) / (2 * a * b));
    Serial.printf("Angle In Rad: %f\n", angleInRad);
    return angleInRad * 180 / PI;
  }

};

#endif