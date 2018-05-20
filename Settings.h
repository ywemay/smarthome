#ifndef H_SETTINGS
#define H_SETTINGS

#include <Arduino.h>
#include "Views.h"
#include <EEPROM.h>
//#include "RTClock.h"
#include <RTClib.h>

class Settings : public Views {
  private:
    
    unsigned int keysRead = 0;  // Number of characters read from keyboard
    
    bool access = false;  // Flag to point if user got access by typing the password
    bool isInit = false;  // Flag to inform the form or menu was initiated
    byte page=0;          // The settings current page
    byte bKey;            // The digit received from keyboard
    
    void showProcessing();

    char pass[5];             // Password as provided by user
    void passwordForm();
    
    void getMainMenu();

    char buff[14];             // A buffer to read user input into
    char buff2[14];
    void getDateForm();
    void changePassForm();
    void SetClock();
    void SetPassword();
    
    int getNrOfDays();
  public:
    bool done = false;
    Settings();
    void show(byte bKey);
    
};
#endif
