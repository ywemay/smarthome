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

#include <Wire.h>
#include "Temperature.h"
#include "Views.h"
#include "Security.h"
#include "Settings.h"
#include "RTClock.h"


#define DEBUG 1 // Comment to disable serial logging
#define KBD_ID 2 // keyboard controller id for I2C connetion identification

bool bReadKey = false;
int iMode;  // Current view mode
uint8_t displayMode, displayModeBuff;

// Keyboard sensing pin
const byte kbdInterruptPin = 2;

RTClock rtc = RTClock();
Views views = Views();
Temperature temp = Temperature();
Security sec= Security();
Settings set = Settings();

bool bSetMode = false;
bool bAccess = false;
// default password
char pass[5] = {'1', '2', '3', '4'};

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
   Serial.println("Welcome!");
  #endif

  Wire.begin();
  views.begin();
  rtc.begin();
  iMode = 0;
  views.init("Hello");

  // Initiate interrupt pin to let keyboard to inform about keypresses.
  pinMode(kbdInterruptPin,  INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(kbdInterruptPin), readKbdInterrupt, FALLING);
  
  //temp.sensorsSetup();
  displayMode = 2;
  displayModeBuff = -1;
  bSetMode = false;
}

/**
 * Main loop
 */
void loop() {
  sec.check();
  if (bReadKey) {
    readKey();
    bReadKey = false;
  }
   
  updateViewModes();
  delay(1000);
}

void updateViewModes() {
  if (bSetMode) return;
  
  if (displayModeBuff != displayMode) 
  {
    if(displayMode == 1) {
      temp.view1Init();
    }
    else if(displayMode == 2) {
      rtc.viewInit();
    }
    else {
      sec.viewInit();
    }
    displayModeBuff = displayMode;
  }

  if (displayMode == 1) {
    temp.view1Update();
  }
  else if(displayMode == 2) {
      rtc.viewUpdate();
  }
  else {
    sec.viewUpdate();
  }
}
/**
 * Reads data from keyboard when intterrup on pin 2
 */
void readKbdInterrupt() {
  bReadKey = true;
}

void readKey() {
  
  char chRd[3] = "ERR";
  
  Wire.requestFrom(KBD_ID, 3); // request 3 bytes from keyboard

  int i = 0;
  while (Wire.available()) {
    char c = Wire.read();
    if (i<=2) {
      chRd[i] = c;
    }   
    i++;
  }
  
  int iKey = chRd[1] - '0';
  
  // If long keyboard press;
  if (chRd[0] == 'L' && chRd[1] == '#' ) {
    if (!bSetMode) {
      bSetMode = true;
      set.done = false;
    }
  }
  if (bSetMode) {
    set.show(chRd[1]);
    if (set.done) {
      displayMode = 0;
      bSetMode = false;
      displayModeBuff = -1; // to force reinitiation of view
    }
  }
  else if (iKey < 3 && iKey >= 0){
    if (!bSetMode) displayMode = iKey;
  }
}

