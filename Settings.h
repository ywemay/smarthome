#ifndef H_SETTINGS
#define H_SETTINGS

#include <Arduino.h>
#include "Views.h"
#include <EEPROM.h>

class Settings : public Views {
  private:
    char pass[5];
    unsigned int keysRead = 0;
    bool access = false;
    byte bMode = -1;
    void showProcessing(byte bKey);
    byte page=0;
  public:
    bool done = false;
    Settings();
    void show(byte bKey);
    void passwordView(byte bK);
};
#endif
