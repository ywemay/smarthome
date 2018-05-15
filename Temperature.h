#ifndef H_TEMPERATURE
#define H_TEMPERATURE

#include <OneWire.h>  // One wire communication for Dallas (usually Temperature) Sensors
#include <DallasTemperature.h>

#define ONE_WIRE_BUS      2
#define TEMPERATURE_PRECISION 1

float getTempByID(int id);
void sensorsSetup();
void TemperatureView1Init();
void TemperatureView1Update();

#endif
