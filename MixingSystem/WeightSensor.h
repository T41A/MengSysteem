#ifndef WEIGHTSENSOR_H
#define WEIGHTSENSOR_H
#include <Q2HX711.h>
class WeightSensor
{
public:
  WeightSensor(void);
  bool Calibrate(void);
  int GetLoad(void);
private:
  Q2HX711* hx711;
  uint32_t nullSetpoint;
};

#endif
