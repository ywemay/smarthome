/**
 * This unit will communicate with main security system unit 
 * and display data, log it.
 * 
 * The string readen from wire contains characters in order:
 * - Armed (A or I)
 * - Rise Alarm (A or N)
 * - Area 1
 * - Area 2
 * - Area 3
 * - Area 4
 */
#include "Temperature.h"
#include "Views.h"
#include "Security.h"
#include "RTClock.h"
#include <Wire.h>

#define DEBUG 1 // Comment to disable serial logging

int iMode;  // Current view mode
uint8_t displayMode, displayModeBuff;

RTClock rtc;
bool bSetMode = false;

struct SKey {
  byte state;
  byte key;
};

/**
 * Setup
 */
void setup() { 
  #ifdef DEBUG
   Serial.begin(9600);
  #endif
 
  ViewsBegin();
  rtc.begin();
  iMode = 0;
  
  sensorsSetup();
  displayMode = 2;
  displayModeBuff = -1;
  bSetMode = false;
}



/**
 * Main loop
 */
void loop() {

  int iKey;

  // device 2 is keyboard controller
  Wire.requestFrom(2, 3);

  char chRd[3] = "ERR";

  int i = 0;
  while (Wire.available()) {
    char c = Wire.read();
    if (i<=2) {
      chRd[i] = c;
    }   
    i++;
  }

  if (chRd[0] == 'L') {
    bSetMode = true;
    // If pressed to cancel set mode.
    if (chRd[1] == '*') bSetMode = false;
  }
  
  if (!bSetMode) {
    switch(chRd[1]) {
      case '0':
        displayMode = 0;
      case '1':
        displayMode = 1;
        break;
      case '2':
        displayMode = 2;
        break;
    }
  }
  
  SecurityCheck();
  Serial.println((char *)chRd);
  if (!bSetMode || chRd[0] != 'N') updateView(chRd[1]);
  
  delay(1000);
}

/**
 * Reads keyboard for a key input
 */
SKey readKeyboard(){
  // device 2 is keyboard controller
  Wire.requestFrom(2, 3);

  char chRd[3] = "ERR";

  int i = 0;
  while (Wire.available()) {
    char c = Wire.read();
    if (i<=2) {
      chRd[i] = c;
    }   
    i++;
  }

  return {chRd[0], chRd[1]};
}

void updateView(char cKey) {
  if (displayModeBuff != displayMode) 
  {
    if(displayMode == 1) {
      TemperatureView1Init();
    }
    else if(displayMode == 2) {
      rtc.viewInit();
    }
    else {
      SecurityViewInit();
    }
    displayModeBuff = displayMode;
  }

  if (displayMode == 1) {
    TemperatureView1Update();
  }
  else if(displayMode == 2) {
      rtc.viewUpdate(&bSetMode, cKey);
  }
  else {
    SecurityViewUpdate();
  }
}


