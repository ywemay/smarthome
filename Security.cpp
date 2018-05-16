#include <Wire.h> // I2C communication

#include "Security.h"
#include "Views.h"
#include "Buzzer.h"

bool bSecurityViewRefresh = true; //
String sRead, sReadBuff;  // I2C reading strings

// char array to print to the screen
// char areasValue[5];

/**
 * Starts communication with security board.
 */
void Security::begin(){
    #ifndef WIRE_BEGIN_CALLED //if not defined
    Wire.begin(); // will only call if it hasn't been called yet
    #endif
    sReadBuff = "******";
}

/**
 * Checks the data on security board
 */
void Security::check()
{
  sRead = "";
  // put your main code here, to run repeatedly:
  Wire.requestFrom(8, 7);
  
  while (Wire.available()) {
    char c = Wire.read();
    sRead += c;
  }

  #ifdef DEBUG
  Serial.println(sRead);
  #endif

  // If failed to communicate with the security system controller - 
  // display error
  if (sRead == "") {
    sRead = "EEEEEE";
    if (sRead != sReadBuff) {
      toneShort(); toneShort(); toneShort();
    }
  } else {
    if (sRead.charAt(0) != sReadBuff.charAt(0)) {
       sRead.charAt(1) == 'A' ? toneLong() : toneShort();
    }
    
    if (sRead.charAt(1) != sReadBuff.charAt(1)) {
       if (sRead.charAt(1) == 'A' && sReadBuff.charAt(1) != "*") {
        toneLong(); toneShort(); toneLong();
       }
    }
  }
  
  if (sRead != sReadBuff) {
    bSecurityViewRefresh = true;
  }

  //ds1302_struct rtc;
  sReadBuff = sRead;
}

/**
 * Switches the view to security display
 */
void Security::viewInit() 
{
  this->init("SECURITY");
}

/**
 * Updates Security View.
 */
void Security::viewUpdate()
{
  if (!bSecurityViewRefresh) return;

  char msg[15];
  byte clr;
 
  switch (sRead.charAt(0)) {
    case 'I':
      strcpy(msg,"UNARMED"); clr = CLR_SILVER;
      break;
    case 'A':
      strcpy(msg,"ARMED  "); clr = CLR_GREEN;
      break;
    default:
      strcpy(msg,"ERROR  "); clr = CLR_RED;
      break;
  }
  this->TextRow2(msg, clr, 0, 1);

  switch (sRead.charAt(1)) {
    case 'N':
      strcpy(msg,"NO ALARM"); clr = CLR_GREEN;
      break;
    case 'A':
      strcpy(msg,"ALARM!!!"); clr = CLR_RED;
      break;
    default:
      strcpy(msg,"ERROR  "); clr = CLR_RED;
      break;
  }
  this->TextRow2(msg, clr, 0, 2);
}

