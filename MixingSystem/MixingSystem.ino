#include "WeightSensor.h"
#include "Valve.h"
#include <mcp_can.h>
#include <SPI.h>

#define CAN0_INT 2
#define ADRESS 0x20
#define TIMER 100

WeightSensor* weightSensor;
Valve* drainValve;
MCP_CAN CAN0(10);

uint8_t cupPresend = 0;
uint8_t state = 0;
unsigned long lastUpdate = 0;

void setup() {
  Serial.begin(9600);
  if (CAN0.begin(MCP_ANY, CAN_100KBPS, MCP_16MHZ) == CAN_OK) Serial.println("MCP2515 Initialized Successfully!");
  else Serial.println("Error Initializing MCP2515...");
  CAN0.setMode(MCP_NORMAL);

  weightSensor = new WeightSensor();
  weightSensor->Calibrate();
  drainValve = new Valve(9);
  drainValve->Close();
}

void loop() {
  readData();
  if (millis() > lastUpdate + TIMER) {
    lastUpdate = millis();
    switch (state)
    {
      case 0://wait for Msg
        drainValve->Close();
        
        break;
      case 1://cal sensor.
        drainValve->Close();
        weightSensor->Calibrate();
        state = 2;
        break;
      case 2://send data.
        sendData(weightSensor->GetLoad(), 0x00);
        break;
      case 3://drain.
      if(cupPresend)
      {
        drainValve->Open();
        if (weightSensor->GetLoad() > 1)
        {
          drainValve->Open();
          sendData(weightSensor->GetLoad(), 0x01);
        }
        else
        {
          drainValve->Close();
          sendData(weightSensor->GetLoad(), 0x02);
          state = 0;
          break;
        }
      }
      else
      {
      drainValve->Close();
       sendData(weightSensor->GetLoad(), 0x03);
       Serial.println("Wait for cup");
      }
        break;
    }
    
  }

}
/* status = 
 *  send weight = 0x00
 *  emtying = 0x01
 *  emty = 0x02
 *  no cup = 0x03
*/
void readData()
{
  long unsigned int rxId;
  unsigned char len = 0;
  unsigned char rxBuf[4];
  if (!digitalRead(CAN0_INT))                        // If CAN0_INT pin is low, read receive buffer
  {
    CAN0.readMsgBuf(&rxId, &len, rxBuf);      // Read data: len = data length, buf = data byte(s)

    if (rxId == 0x10 && state == 0) // if msg from ui
    {
      state = 1;
    }
    if (rxId == 0x30) // msg from toevoer
    {
      if (rxBuf[0] == 0x01 && state == 2)
      {
        state = 3;
      }
    }
    if(rxId == 0x40)
    {
     cupPresend = rxBuf[0];
      Serial.print("Cup receive event: ");
      Serial.println(rxBuf[0]);
     }
  }
}
void sendData(uint16_t weight, uint8_t SystemStatus)
{
  byte data[3] = {SystemStatus, weight & 0xFF , weight >> 8 & 0xFF};
  byte sndStat = CAN0.sendMsgBuf(ADRESS, 0, 3, data);
  if (sndStat == CAN_OK) {
    Serial.print("Send weight(");
    Serial.print(weight);
    Serial.print(") and status (");
    Serial.print(SystemStatus);
    Serial.println(")");
  } else {
    Serial.println("Error Sending Message...");
  }
}

