#include <HardwareSerial.h>

#ifndef _SimpleIK_h_
#define _SimpleIK_h_


#define PI 3.14159265359

class _SimpleIK {

  private:
  
    double _groundClearance;
    double _shoulderLen;
    HardwareSerial *Serial;
  
    double calcDist(int x, int z);

  public:
    
    _SimpleIK();
    
    void init(HardwareSerial *serial, double groundClearance, double shoulderLen);

    double calcTheta(int x, int z);

    double calcAlpha(int x, int y, int z);

    double getGroundClearance();

};

extern _SimpleIK SimpleIK;

#endif