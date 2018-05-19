#ifndef H_RTCLOCK
#define H_RTCLOCK

#include <Arduino.h>
#include <Wire.h>
#include "Views.h"
#include "utils.h"

#define DATE_INDENT LINDENT+10
#define TIME_INDENT LINDENT+15
#define DATE_ROW ROW2_HIGH * 3
#define TIME_ROW ROW2_HIGH * 4

struct STime {
  byte s;
  byte i;
  byte h;
  byte d;
  byte dt;
  byte m;
  byte y;
};

/*
void RTClockBegin();
void RTClockViewInit();
void RTClockViewUpdate();
void RTClockPrintPart(byte *bBuff, byte iNow, int x, int y, bool yr = false);
*/

class RTClock : public Views {
  private:
    void printPart(byte *bBuff, byte iNow, int x, int y, bool yr = false);
    STime buffTime;
    STime sTime;
    void read();
    bool setDigit(byte *b, byte digit, byte bMax, byte bMin = 0x00 );
    //void setAtCurrPos(byte bDigit);
    void updateTime();
    void underlinePosition(byte clr = CLR_YELLOW);
  public:
    byte setPos = 0;
    RTClock();
    bool done = false; // indicates when changes are done
    void begin();
    void viewInit();
    void viewUpdate();
    //byte getNrOfDays(byte by, byte bm);
    void updateSetView(byte bDigit);
};

#endif
