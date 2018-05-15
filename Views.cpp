#include "Views.h"

// create an instance of the library
TFT TFTscreen = TFT(cs, dc, rst);

void ViewsBegin()
{
  TFTscreen.begin();
  TFTscreen.background(0, 0, 0);
  TFTscreen.setTextSize(2);
}

void initBasicView(char * title) {
  TFTscreen.background(0, 0, 0);
  ViewsText(title, LINDENT, TINDENT, CLR_YELLOW);
}

void Color2RGB(char * clr, int *rgb)
{
  long long number = strtol( &clr[1], NULL, 16);

  // Split them up into r, g, b values
  rgb[0] = number >> 16;
  rgb[1] = number >> 8 & 0xFF;
  rgb[2] = number & 0xFF;
}

void ViewsText(char * text, int x, int y, char * clr, char * bgclr = CLR_BLACK, int sz=1, int ln=0)
{
  int rgb[3];

  Color2RGB(bgclr, rgb);
  
  TFTscreen.stroke(rgb[2], rgb[1], rgb[0]);
  TFTscreen.fill(rgb[2], rgb[1], rgb[0]);
  TFTscreen.rect(x-1+LINDENT, y-1+TINDENT, (ln ? ln : strlen(text)) * sz * 6 + 1, sz*8);
  
  Color2RGB(clr, rgb);
  TFTscreen.setTextSize(sz);
  TFTscreen.stroke(rgb[2], rgb[1], rgb[0]);
  TFTscreen.text(text, x+LINDENT, y+TINDENT);
}

void ViewsBar(int x, int y, int l, int h, char *clr = CLR_BLACK) {
  int rgb[3];
  Color2RGB(clr, rgb);
  TFTscreen.stroke(rgb[2], rgb[1], rgb[0]);
  TFTscreen.fill(rgb[2], rgb[1], rgb[0]);
  TFTscreen.rect(x, y, l, h);
}

