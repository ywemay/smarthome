/** 
 * This clock is I2C bus clock.
 * A4 - SDA
 * A5 - SCL
 */

#include "RTClock.h"


RTClock::RTClock(){
}

void RTClock::begin() {
  #ifndef WIRE_BEGIN_CALLED //if not defined
  Wire.begin(); // will only call if it hasn't been called yet
  #endif
}

void RTClock::viewInit(){
  this->init("TIME");
  this->Text("-", DATE_INDENT+13*4, DATE_ROW, CLR_WHITE);
  this->Text("-", DATE_INDENT+13*7, DATE_ROW, CLR_WHITE);
  this->Text(":", TIME_INDENT+13*2, TIME_ROW, CLR_WHITE);
  this->Text(":", TIME_INDENT+13*5, TIME_ROW, CLR_WHITE);
  sTime = {99, 99, 99};
  sDate = {0, 0, 0, 0};
  buffTime = {99, 99, 99};
  buffDate = {0, 0, 0, 0};
}

void RTClock::read(){
  Wire.beginTransmission(0x68);
  Wire.write((uint8_t) 0x00);
  Wire.endTransmission();

  char sRd[9];
  int i = 0;
  Wire.requestFrom(0x68, 8);
  while(Wire.available()) {
    sRd[i] = (char) Wire.read();
    i++;
  }

  sTime = {sRd[2], sRd[1], sRd[0]};
  sDate = {sRd[6], sRd[5], sRd[4], sRd[3]};
}

void RTClock::viewUpdate(bool *bSet = false, byte bDigit = 0){
  //DateTime now = rtc.now();
  char dt[5];
  byte chWidth = 13;

  if (!*bSet || sDate.m == 0) {
    if (sDate.m == 0) this->setPos = 0;
    this->read();
  }
  else if (*bSet) {
    if (bDigit == '#') {
      *bSet = false;
    } else if (bDigit != '*') {
      this->setAtCurrPos(bDigit);
    }
  }
  
  this->printPart(&buffTime.h, sTime.h, TIME_INDENT, TIME_ROW);
  this->printPart(&buffTime.m, sTime.m, TIME_INDENT+chWidth*3, TIME_ROW);
  this->printPart(&buffTime.s, sTime.s, TIME_INDENT+chWidth*6, TIME_ROW);
  this->printPart(&buffDate.y, sDate.y, DATE_INDENT, DATE_ROW, true);
  this->printPart(&buffDate.m, sDate.m, DATE_INDENT+chWidth*5, DATE_ROW);
  this->printPart(&buffDate.dt, sDate.dt, DATE_INDENT+chWidth*8, DATE_ROW);

  if (buffDate.d == sDate.d) return;
  char dayName[10];
  switch (bcdToDec(sDate.d)) {
    case 1: strcpy(dayName, "Luni"); break;
    case 2: strcpy(dayName, "Marti"); break;
    case 3: strcpy(dayName, "Miercuri"); break;
    case 4: strcpy(dayName, "Joi"); break;
    case 5: strcpy(dayName, "Vineri"); break;
    case 6: strcpy(dayName, "Sambata"); break;
    default: strcpy(dayName, "Duminica"); break;    
  }
  this->Text(dayName, DATE_INDENT, DATE_ROW-20, CLR_WHITE, 3);
  buffDate.d = sDate.d;
}

void RTClock::printPart(byte *bBuff, byte iNow, int x, int y, bool yr = false) {
  if (*bBuff == iNow) return;
  char nr[5];
  sprintf(nr, yr ? "%04d" : "%02d" , yr ? 2000 + (int) bcdToDec(iNow) : bcdToDec(iNow));
  this->Text(nr, x, y, CLR_WHITE, yr ? 4 : 2);
  *bBuff = iNow;
}

byte RTClock::setDigit(byte b, byte digit, int pos = 0) {
  int d = digit - '0';
  if (pos == 0) {
    return decToBcd(d * 10 + bcdToDec(b)%10);
  }
  return decToBcd(bcdToDec(b) - bcdToDec(b)%10 + d); 
}

void RTClock::setAtCurrPos(byte bDigit){
  switch(this->setPos/2){
    case 0:
      this->sDate.y = this->setDigit(this->sDate.y, bDigit, this->setPos%2);
      break;
    case 1:
      this->sDate.m = this->setDigit(this->sDate.m, bDigit, this->setPos%2);
      break;
    case 2:
      this->sDate.dt = this->setDigit(this->sDate.dt, bDigit, this->setPos%2);
      break;
    case 3:
      this->sTime.h = this->setDigit(this->sTime.h, bDigit, this->setPos%2);
      break;
    case 4:
      this->sTime.m = this->setDigit(this->sTime.m, bDigit, this->setPos%2);
      break;
    case 5:
      this->sTime.s = this->setDigit(this->sTime.s, bDigit, this->setPos%2);
      break;
  }
  this->setPos++;
  if (this->setPos > 11) this->setPos = 0;
}
