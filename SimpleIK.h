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
    double d = sqrt(sq(x) + sq(z)) - _shoulderLen;
    return asin(d / _groundClearance) * 180 / PI;
  }

};

#endif