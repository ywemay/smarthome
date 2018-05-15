#ifndef H_TEMPERATURE
#define H_TEMPERATURE

#include <OneWire.h>  // One wire communication for Dallas (usually Temperature) Sensors
#include <DallasTemperature.h>
#include "Views.h"

#define ONE_WIRE_BUS      2
#define TEMPERATURE_PRECISION 1

class Temperature : public Views {
  public:
    float getTempByID(uint8_t id);
    void sensorsSetup();
    void view1Init();
    void view1Update();
    void drawTemp(uint8_t row, uint8_t col, uint8_t id);
};


#endif
