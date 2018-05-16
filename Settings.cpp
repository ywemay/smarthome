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
  if (!this->access) {
    this->passwordView(this->bMode != 0 ? 'I' : bKey);
  }
  else this->showProcessing(bKey);
}

void Settings::passwordView(byte bKey){
  if (bKey == 'I') 
  {
    this->init("Parola:");
    this->done = false;
    this->bMode = 0;
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
    this->access = false;
  }
  if (bKey == '#') {
    char storedPass[5];
    EEPROM.get(0, storedPass);
    if (memcmp(storedPass, this->pass, 4) == 0) {
      this->access = true;
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

void Settings::showProcessing(byte bKey) {
  if (bKey == 'I') {
    //this->init("Settings");  
  }
  if (bKey == '*') {
    this->done = true;
    this->access = false;
  }
  this->TextRow2("1 Data/Ora", CLR_WHITE, 0, 1);
  this->TextRow2("2 Parola", CLR_WHITE, 0, 2);
  this->TextRow2("* Anulare", CLR_WHITE, 0, 3);
}

void Settings::getMenuPage(byte pgNr, byte bKey) {
  int iRow = 0;
  switch (pgNr) {
    case 1:
      this->init("SETARI");
      this->TextRow2("1 Data/Ora", CLR_WHITE, 0, i++);
      this->TextRow2("2 Parola", CLR_WHITE, 0, i++);
      break;
  }
  this->TextRow2("* Anulare", CLR_WHITE, 0, i);
}

