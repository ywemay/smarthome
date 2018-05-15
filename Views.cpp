#include "Views.h"

// create an instance of the library
TFT TFTscreen = TFT(cs, dc, rst);

Views::Views(){
}

void Views::begin(){
  TFTscreen.begin();
  this->setTextSize(2);
  TFTscreen.background(0,0,0);
}

void Views::setTextSize(int sz) {
  TFTscreen.setTextSize(sz);
  this->textSize = sz;
}

void Views::init(char * title) {
  TFTscreen.background(0, 0, 0);
  TFTscreen.fill(0, 0, 0);
  this->Text(title, LINDENT, TINDENT, CLR_YELLOW);
}

void Views::SetEraseColor(){
  TFTscreen.stroke(0, 0, 0);
}

void Views::SetTextColor(byte clr) {
  TColor c = this->getRGB(clr);
  TFTscreen.stroke(c.r, c.g, c.b);
  /*
  switch (clr) {
    case CLR_BLACK: TFTscreen.stroke(0, 0, 0); break;
    case CLR_RED: TFTscreen.stroke(255, 0, 0); break;
    case CLR_GREEN: TFTscreen.stroke(0, 255, 0); break;
    case CLR_BLUE: TFTscreen.stroke(0, 0, 255); break;
    case CLR_YELLOW: TFTscreen.stroke(0, 155, 155); break;
    case CLR_SILVER: TFTscreen.stroke(155, 155, 155); break;
    default: TFTscreen.stroke(255, 255, 255); break;
  }*/
}

TColor Views::getRGB(byte clr) {
  switch (clr) {
    case CLR_BLACK: return {0, 0, 0}; break;
    case CLR_RED: return {255, 0, 0}; break;
    case CLR_GREEN: return {0, 255, 0}; break;
    case CLR_BLUE: return {0, 0, 255}; break;
    case CLR_YELLOW: return {0, 155, 155}; break;
    case CLR_SILVER: return {155, 155, 155}; break;
    default: return {255, 255, 255}; break;
  }
}

void Views::Text(char * text, int x, int y, byte clr, int ln=0)
{
  this->SetEraseColor();
  TFTscreen.rect(x-1+LINDENT, y-1+TINDENT, (ln ? ln : strlen(text)) * this->textSize * 6 + 1, this->textSize*8);
  
  this->SetTextColor(clr);
  TFTscreen.text(text, x+LINDENT, y+TINDENT);
}

void Views::Bar(int x, int y, int l, int h, byte clr = CLR_BLACK) {
  TColor c = this->getRGB(clr);
  TFTscreen.stroke(c.r, c.g, c.b);
  TFTscreen.fill(c.r, c.g, c.b);
  TFTscreen.rect(x, y, l, h);
}

