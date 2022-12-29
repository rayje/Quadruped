#ifndef _SimpleIK_h_
#define _SimpleIK_h_

class SimpleIK {

  private:
  double _groundClearance;
  double _shoulderLen;

  public:

  SimpleIK(double groundClearance, double shoulderLen) {
    _groundClearance = groundClearance;
    _shoulderLen = shoulderLen;
  }
    
  double calcTheta(double x, double z) {
    return atan2(x, z) * 180 / PI;
  }

  double calcAlpha(double x, double z) {
    double dist = sqrt(pow(x, 2) + pow(z, 2));
    double d = dist - _shoulderLen;
    double yOffset = sqrt(pow(d, 2) + pow(_groundClearance, 2)) - _groundClearance;
    
    double a = _groundClearance;
    double b = _groundClearance - yOffset;
    double c = dist - _shoulderLen;
    
    double angleInRad = acos((pow(a, 2) + pow(b, 2) - pow(c, 2)) / (2 * a * b));
    return angleInRad * 180 / PI;
  }

};

#endif