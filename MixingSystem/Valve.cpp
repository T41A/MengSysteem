#include "Valve.h"
Valve::Valve(int pin):valvePin(pin)
{
  pinMode(pin,OUTPUT);
}
void Valve::Open(void)
{
  if(isOpen == false)
  {
    digitalWrite(valvePin,HIGH);
    isOpen = true;
  }

}
void Valve::Close(void)
{
  if(isOpen == true)
  {
    digitalWrite(valvePin,LOW);
    isOpen = false;
  }
}
