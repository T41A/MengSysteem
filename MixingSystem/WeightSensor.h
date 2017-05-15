#ifndef WEIGHTSENSOR_H
#define WEIGHTSENSOR_H
#include <Q2HX711.h>
class WeightSensor
{
public:
  bool Calibrate(void);
  int GetLoad(void);
private:

};

#endif
