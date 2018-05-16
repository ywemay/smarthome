#ifndef H_VIEWS
#define H_VIEWS

#include <TFT.h>  // Display library
#include <SPI.h>  // TODO: Clarify...
#include "utils.h"

// TFT
#define cs   10
#define dc   9
#define rst  8

// Views:
#define ROW_HIGH  6
#define ROW2_HIGH  18
#define COL_WIDTH 30
#define LINDENT  5
#define TINDENT  5

#define CLR_BLACK   0x00
#define CLR_RED     0x01
#define CLR_GREEN   0x02
#define CLR_BLUE    0x03
#define CLR_YELLOW  0x04
#define CLR_SILVER  0x05
#define CLR_WHITE   0x06



class Views {
  private:
    unsigned int textSize = 1;
    
  public:
    bool done = false;
    Views();
    void begin();
    void init(char * title);
    void setTextSize(int sz);
    void SetEraseColor();
    void SetTextColor(byte cl0r);
    TColor getRGB(byte clr);
    void Text(char * text, int x, int y, byte clr = CLR_WHITE, int ln = 0);
    void TextRow2(char * text, byte clr, int row, int col = 0, int ln=0);
    void Bar(int x, int y, int l, int h, byte clr = CLR_BLACK);
};

#endif
