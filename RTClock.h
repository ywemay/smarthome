#ifndef H_RTCLOCK
#define H_RTCLOCK

#include <Arduino.h>
#include <Wire.h>
#include "Views.h"
#include "utils.h"

#define DATE_INDENT LINDENT+10
#define TIME_INDENT LINDENT+15
#define DATE_ROW ROW2_HIGH * 2 * 2
#define TIME_ROW ROW2_HIGH * 2 * 3

struct STime {
  byte h;
  byte m;
  byte s;
};

struct SDate {
  byte y;
  byte m;
  byte dt; //date
  byte d; // day
};

/*
void RTClockBegin();
void RTClockViewInit();
void RTClockViewUpdate();
void RTClockPrintPart(byte *bBuff, byte iNow, int x, int y, bool yr = false);
*/

class RTClock : public Views {
  private:
    byte setPos = 0;
    void printPart(byte *bBuff, byte iNow, int x, int y, bool yr = false);
    STime buffTime;
    SDate buffDate;
    STime sTime;
    SDate sDate;
    void read();
    byte setDigit(byte b, byte digit, int pos = 0);
    void setAtCurrPos(byte bDigit);
  public:
    RTClock();
    void begin();
    void viewInit();
    void viewUpdate(bool *bSet = false, byte bDigit = 0);
};

#endif
