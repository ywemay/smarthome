#include "utils.h"

byte bcdToDec(byte b) {
  return ( ((b >> 4)*10) + (b%16) );  
}

byte decToBcd(byte b) {
  return ( ((b/10) << 4) + (b%10) );  
}

