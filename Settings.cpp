#include "Settings.h"

Settings::Settings() {
  strcpy(this->pass, "");
  /*
  char toWrite[5];
  strcpy(toWrite, "1111");
  toWrite[4] = '\0';
  EEPROM.put(0, toWrite);
  */
}

void Settings::show(byte bKey){
  this->bKey = bKey;
  if (!this->access) {
    this->passwordForm();
  }
  else this->showProcessing();
}

void Settings::passwordForm(){
  if (!this->isInit) 
  {
    strcpy(this->pass, "****"); // Erase preview user input.
    this->init("Parola:");
    this->done = false;
    this->isInit = true;
    return;
  }
  if (this->keysRead < 4 && bKey - '0' >=0 && bKey - '0' <=9) {
    this->pass[this->keysRead] = bKey;
    this->keysRead++;
    char strAst[5] = "";
    memset(strAst, '*', this->keysRead);
    this->TextRow2(strAst, CLR_WHITE, 0, 1, 7);
    if (this->keysRead>3) {
      this->pass[4] = '\0';
      this->TextRow2("Apasa #",CLR_GREEN, 0, 4);
    }
    else {
      this->TextRow2("", CLR_RED, 0, 4, 10);
      this->TextRow2("", CLR_GREEN, 0, 5, 10);
    }
  }
  if (bKey == '*') {
    this->done = true;
    this->isInit = false;
    this->access = false;
  }
  if (bKey == '#') {
    char storedPass[5];
    EEPROM.get(0, storedPass);
    if (memcmp(storedPass, this->pass, 4) == 0) {
      this->access = true;
      this->isInit = false;
      this->page = 0;
      this->show('I');
    }
    else {
      this->TextRow2("Incorect", CLR_RED, 0, 4);
      this->TextRow2("...repeta", CLR_GREEN, 0, 5);
      this->TextRow2("", CLR_WHITE, 0, 1, 7);
      this->keysRead = 0;
    }
  }
}

void Settings::showProcessing() {
  
  switch(this->page) {
    case 0: this->getMainMenu(); break;
    case 1: this->getDateForm(); break;
  }
}

/**
 * Displays main settings menu.
 */
void Settings::getMainMenu() {
  byte i = 0;
  // Asterisc is used to cancel the menu
  if (bKey == '*') {
    this->done = true;
    this->access = false;
    this->isInit = false;
    this->bKey = 'I';
    this->showProcessing();
    return;
  }
  // Process user choise:
  if (bKey - '0' >=1 && bKey - '0' <=3) {
    this->page = bKey - '0';
    //Serial.println(this->page);
    this->isInit = false;
    this->bKey = 'I';
    this->showProcessing();
    return;
  }
  if (!this->isInit) {
    this->init("SETARI");
    this->TextRow2("1 Data", CLR_WHITE, 0, i++);
    this->TextRow2("2 Ora", CLR_WHITE, 0, i++);
    this->TextRow2("3 Parola", CLR_WHITE, 0, i++);
    this->TextRow2("* Anulare", CLR_WHITE, 0, i); 
    this->isInit = true;
  }
}

/**
 * Date form
 */
void Settings::getDateForm(){

  const unsigned int msgRow = 11;
  
  if (!this->isInit) {
    this->init("DATA");
    this->TextRow("Ex:2018-09-14", CLR_SILVER, 0, 3);
    this->TextRow("    09:12:02", CLR_SILVER, 0, 4);
    this->TextRow("# OK, * Exit", CLR_SILVER, 0, 12);
    this->isInit = true;
    this->keysRead = 0;
    strcpy(this->buff, "20_       ");
    strcpy(this->buff2, "");
    this->TextRow2(this->buff, CLR_WHITE, 0, 3);
  }
  
  if (bKey == '*') {
    this->isInit = false;
    this->page = 0;
    this->bKey = 'I';
    this->showProcessing();
    return;
  }
  if (bKey == '#') {
    if (this->keysRead >= 16) {
      this->SetClock();
      this->page = 0;
      this->isInit = false;
      this->bKey = 'I';
      this->showProcessing();
      return;
    }
    else this->TextRow("Not ready...", CLR_RED, 0, msgRow);
  }
  
  if (bKey - '0' >= 0 && bKey - '0' <= 9) {
    if (this->keysRead < 8) { 
      if (this->keysRead == 3 && bKey - '0' > 1) {
        this->TextRow("ERR 0|1", CLR_RED, 0, msgRow);
        return;
      }
      if (this->keysRead == 4) {
        if (this->buff[5] - '0' == 0 && bKey - '0' == 0) {
          this->TextRow("ERR No 0", CLR_RED, 0, msgRow);
          return;
        }
        if (this->buff[5] - '0' == 1 && bKey - '0' > 2) {
          this->TextRow("ERR <=2", CLR_RED, 0, msgRow);
          return;
        }
      }
      int nrDays;
      char err[] = "Numai XX zile.";
      // Validating month:
      if (this->keysRead == 6) {
         
         nrDays = this->getNrOfDays();
         if ((int)(nrDays/10) < bKey - '0') {
           err[6] = (int) nrDays/10 + '0';
           err[7] = nrDays%10 + '0';
           this->TextRow(err, CLR_RED, 0, msgRow);
           return;
         }
      }
      if (this->keysRead == 7) {
         nrDays = this->getNrOfDays();
         if (this->buff[8] - '0' == 0 && bKey - '0' == 0) {
          this->TextRow("ERR No 0", CLR_RED, 0, msgRow);
          return;
         }
         if (this->buff[8] - '0' == (int) nrDays/10 && bKey - '0' > nrDays%10) {
           err[6] = (int) nrDays/10 + '0';
           err[7] = nrDays%10 + '0';
           this->TextRow(err, CLR_RED, 0, msgRow);
           return;
         }
      }
      this->TextRow("OK...", CLR_GREEN, 0, msgRow, 20);
      this->buff[this->keysRead+2] = bKey;
      this->keysRead++;
      if (this->keysRead != 8) {
        this->buff[this->keysRead+2] = '_';
      }
      else {
        this->buff2[0] = '_';
        this->TextRow2(this->buff2, CLR_WHITE, 0, 4);
      }
      if (this->keysRead == 2 || this->keysRead == 5) {
        this->buff[this->keysRead+2] = '-';
        this->keysRead++;
        this->buff[this->keysRead+2] = '_';
      }
      this->TextRow2(this->buff, CLR_WHITE, 0, 3);
    }
    else if (this->keysRead < 16) {
      int iPos = this->keysRead - 8;
      if ((iPos == 0 && bKey - '0' > 2) || (iPos == 1 && this->buff2[0] == '2' && bKey - '0' > 3)) {
        this->TextRow("Pana la 23:59", CLR_RED, 0, msgRow, 15);
        return;
      }
      if ((iPos == 3 || iPos == 6) && bKey - '0' > 5) {
        this->TextRow("Pana la 59", CLR_RED, 0, msgRow, 15);
        return;
      }
      this->TextRow("OK...", CLR_GREEN, 0, msgRow, 20);
      this->buff2[iPos] = bKey;
      this->keysRead++;
      if (iPos < 6) this->buff2[iPos+1] = '_';
      if (this->keysRead == 10 || this->keysRead == 13) {
        this->buff2[iPos+1] = ':';
        this->keysRead++;
        this->buff2[iPos+2] = '_';
      }
      this->TextRow2(this->buff2, CLR_WHITE, 0, 4);
    }
  }
  if (this->keysRead == 16) this->TextRow("Apasa #, pentru memorare.", CLR_GREEN, 0, msgRow, 20);
}

int Settings::getNrOfDays() {
  int y = (this->buff[2] - '0') * 10 + (this->buff[3] - '0') + 2000;
  int m = (this->buff[5] - '0') * 10 + (this->buff[6] - '0');
  if (m == 4 || m == 6 || m == 9 || m == 11)
    return 30;
    
  else if (m == 2)
  {
    bool leapyear = (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);

    if (leapyear == 0)
        return 28;
    else 
        return 29;
  }
  return 31;  
}

void Settings::SetClock(){
  DS1307 ds = DS1307();
  this->buff2[8] = '\0';
  char sDate[] = {this->buff[8], this->buff[9], '-', this->buff[5], this->buff[6], '-', '2', '0', this->buff[2], this->buff[3], '\0'};
  Serial.print(sDate);
  Serial.print(" ");
  Serial.println(this->buff2);
  
  DateTime dt = DateTime (sDate, this->buff2);
  ds.adjust(dt);
}
