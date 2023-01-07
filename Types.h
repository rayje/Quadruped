#ifndef _Types_h_
#define _Types_h_

typedef struct {
  int armIndex;
  int shoulderIndex;
  double armFrom;
  double armTo;
  double shoulderFrom;
  double shoulderTo;
} Indicies;

typedef struct {
  int index;
  int angle;
} ServoAngle;

typedef struct {
  int index;
  int x;
  int z;
} ServoIK;

#endif
