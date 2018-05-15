#ifndef H_UTILS
#define H_UTILS

#include <Arduino.h>

struct TColor{
  byte r;
  byte g;
  byte b;
};

byte bcdToDec(byte b);
byte decToBcd(byte b);
TColor Color2RGB(char * clr);

#endif
