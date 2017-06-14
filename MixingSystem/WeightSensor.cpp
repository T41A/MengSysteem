#include "WeightSensor.h"
WeightSensor::WeightSensor()
{
hx711 = new Q2HX711(A0, A1);
}
bool WeightSensor::Calibrate(void)
{
  nullSetpoint = 0;
  for(int i = 0; i < 5; i++)
  {
    nullSetpoint =  (hx711->read()/100);
    delay(10);
  }
  //nullSetpoint = nullSetpoint / 5;
 // Serial.println(nullSetpoint);
  return true;
}
int WeightSensor::GetLoad(void)
{
  int temp = (hx711->read()/100)-nullSetpoint;
  return map((temp),0,4379,0,250);
  
}

