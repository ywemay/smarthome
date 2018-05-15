#include "utils.h"

byte bcdToDec(byte b) {
  return ( ((b >> 4)*10) + (b%16) );  
}

byte decToBcd(byte b) {
  return ( ((b/10) << 4) + (b%10) );  
}

TColor Color2RGB(char * clr)
{
  TColor rez;
  long long number = strtol( &clr[1], NULL, 16);

  // Split them up into r, g, b values
  rez.b = number >> 16;
  rez.g = number >> 8 & 0xFF;
  rez.r = number & 0xFF;
}

