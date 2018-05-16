#include "Temperature.h"
#include "Views.h"

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

uint8_t deviceCount = 0;

// Add 4 prepared sensors to the bus
// use the UserDataWriteBatch demo to prepare 4 different labeled sensors
struct
{
  uint8_t id;
  DeviceAddress addr;
} T[4];

/**
 * Get temperature by Sensor ID
 */
float Temperature::getTempByID(uint8_t id)
{
  for (uint8_t index = 0; index < deviceCount; index++)
  {
    if (T[index].id == id)
    {
      return sensors.getTempC(T[index].addr);
    }
  }
  return -999;
}

/**
 * Temperature function
 */
void Temperature::sensorsSetup(){
  sensors.begin();
  // count devices
  deviceCount = sensors.getDeviceCount();
  // Read ID's per sensor
  // and put them in T array
  for (uint8_t index = 0; index < deviceCount; index++)
  {
    // go through sensors
    sensors.getAddress(T[index].addr, index);
    T[index].id = sensors.getUserData(T[index].addr);
  }
}

/**
 * Temperature View Mode 1:
 */
void Temperature::view1Init() 
{
  char ttl[8] = {'T', 'E', 'M' , 'P', ' ', (char) 247, 'C'};
  this->init(ttl);
}

void Temperature::drawTemp(uint8_t row, uint8_t col, uint8_t id) {
  int iTemp = getTempByID(id);
  byte clr;
  if (iTemp > 30) {
    clr = CLR_RED;
  }
  else if (iTemp > 15) {
    clr = CLR_YELLOW;
  }  
  else if (iTemp > 8) {
    clr = CLR_SILVER;
  }
  else if (iTemp > 0) {
    clr = CLR_WHITE;
  }
  else {
    clr = CLR_BLUE;
  }

  char tmp[6];
  String sT = String("");
  sT += iTemp;
  if(iTemp == -999)  {
    sT = "ERR"; 
    clr = CLR_RED;
  }
  sT.toCharArray(tmp, 6);
  //strcpy(tmp, 0xF8 + 'C');
  this->TextRow2(tmp, clr, col, row);
}

void Temperature::view1Update() {
  sensors.requestTemperatures();
  this->drawTemp(1, 0, 201);
  drawTemp(2, 0, 202);
  drawTemp(3, 0, 203);
}
//Sketch uses 16560 bytes (53%) of program storage space. Maximum is 30720 bytes.
//Global variables use 694 bytes (33%) of dynamic memory, leaving 1354 bytes for local variables. Maximum is 2048 bytes.

