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
  // sTime = {99, 99, 99, 0, 0, 0, 0};
  this->dtBuff = DateTime();
  this->updateTime();
}
/*
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

  sTime = {sRd[0], sRd[1], sRd[2], sRd[3], sRd[4], sRd[5], sRd[6]};
}
*/

void RTClock::viewUpdate(){
  //this->read();
  this->updateTime();
}

void RTClock::updateTime(){
  const byte chWidth = 13;
  DateTime dtNow = this->now();
  this->printPart(this->dtBuff.hour(), dtNow.hour(), TIME_INDENT, TIME_ROW);
  this->printPart(this->dtBuff.minute(), dtNow.minute(), TIME_INDENT+chWidth*3, TIME_ROW);
  this->printPart(this->dtBuff.second(), dtNow.second(), TIME_INDENT+chWidth*6, TIME_ROW);
  this->printPart(this->dtBuff.year(), dtNow.year(), DATE_INDENT, DATE_ROW, true);
  this->printPart(this->dtBuff.month(), dtNow.month(), DATE_INDENT+chWidth*5, DATE_ROW);
  this->printPart(this->dtBuff.day(), dtNow.day(), DATE_INDENT+chWidth*8, DATE_ROW);

  if (this->dtBuff.dayOfWeek() != dtNow.dayOfWeek()) {
    char dayName[10];
    switch (dtNow.dayOfWeek()) {
      case 1: strcpy(dayName, "Luni"); break;
      case 2: strcpy(dayName, "Marti"); break;
      case 3: strcpy(dayName, "Miercuri"); break;
      case 4: strcpy(dayName, "Joi"); break;
      case 5: strcpy(dayName, "Vineri"); break;
      case 6: strcpy(dayName, "Sambata"); break;
      default: strcpy(dayName, "Duminica"); break;    
    }
    this->Text(dayName, DATE_INDENT, DATE_ROW-20, CLR_WHITE, 10);
  }
  this->dtBuff = dtNow;
  //
  /*
  const byte chWidth = 13;
  this->printPart(&buffTime.h, sTime.h, TIME_INDENT, TIME_ROW);
  this->printPart(&buffTime.i, sTime.i, TIME_INDENT+chWidth*3, TIME_ROW);
  this->printPart(&buffTime.s, sTime.s, TIME_INDENT+chWidth*6, TIME_ROW);
  this->printPart(&buffTime.y, sTime.y, DATE_INDENT, DATE_ROW, true);
  this->printPart(&buffTime.m, sTime.m, DATE_INDENT+chWidth*5, DATE_ROW);
  this->printPart(&buffTime.dt, sTime.dt, DATE_INDENT+chWidth*8, DATE_ROW);
  
  /*
  const byte chWidth = 13;
  this->printPart(&buffTime.h, sTime.h, TIME_INDENT, TIME_ROW);
  this->printPart(&buffTime.i, sTime.i, TIME_INDENT+chWidth*3, TIME_ROW);
  this->printPart(&buffTime.s, sTime.s, TIME_INDENT+chWidth*6, TIME_ROW);
  this->printPart(&buffTime.y, sTime.y, DATE_INDENT, DATE_ROW, true);
  this->printPart(&buffTime.m, sTime.m, DATE_INDENT+chWidth*5, DATE_ROW);
  this->printPart(&buffTime.dt, sTime.dt, DATE_INDENT+chWidth*8, DATE_ROW);

  if (buffTime.d == sTime.d) return;
  char dayName[10];
  switch (bcdToDec(sTime.d)) {
    case 1: strcpy(dayName, "Luni"); break;
    case 2: strcpy(dayName, "Marti"); break;
    case 3: strcpy(dayName, "Miercuri"); break;
    case 4: strcpy(dayName, "Joi"); break;
    case 5: strcpy(dayName, "Vineri"); break;
    case 6: strcpy(dayName, "Sambata"); break;
    default: strcpy(dayName, "Duminica"); break;    
  }
  this->Text(dayName, DATE_INDENT, DATE_ROW-20, CLR_WHITE, 10);
  buffTime.d = sTime.d;
  */
}

void RTClock::printPart(int iBuff, int iNow, int x, int y, bool yr = false) {
  if (iBuff == iNow) return;
  char nr[5];
  sprintf(nr, yr ? "%04d" : "%02d" , iNow);
  this->Text(nr, x, y, CLR_WHITE, yr ? 4 : 2);
}

/*
void RTClock::setAtCurrPos(byte bDigit){
  switch(this->setPos/2){
    case 0:
      this->setDigit(&this->sTime.y, bDigit, decToBcd(99));
      break;
    case 1:
      this->setDigit(&this->sTime.m, bDigit, decToBcd(12), 0x01);
      break;
    case 2:
      this->setDigit(&this->sTime.dt, bDigit, this->getNrOfDays(this->sTime.y, this->sTime.m), 0x01);
      break;
    case 3:
      this->setDigit(&this->sTime.h, bDigit, decToBcd(23));
      break;
    case 4:
      this->setDigit(&this->sTime.i, bDigit, decToBcd(59));
      break;
    case 5:
      this->setDigit(&this->sTime.s, bDigit, decToBcd(59));
      break;
  }
}

byte RTClock::getNrOfDays(byte by, byte bm) {
  int y = bcdToDec(by);
  int m = bcdToDec(bm);
  if (m == 4 || m == 6 || m == 9 || m == 11)
    return decToBcd(30);
    
  else if (m == 2)
  {
    bool leapyear = (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);

    if (leapyear == 0)
        return decToBcd(28);
    else 
        return decToBcd(29);
  }

  return decToBcd(31);  
}*/
