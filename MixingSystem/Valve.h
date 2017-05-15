#ifndef VALVE_H
#define VALVE_H
#include <arduino.h>
class Valve
{
public:
  Valve(int Pin);
  void Open(void);
  void Close(void);
private:
  int valvePin;
  bool isOpen;
};

#endif
