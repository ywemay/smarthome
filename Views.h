#ifndef H_VIEWS
#define H_VIEWS

#include <TFT.h>  // Display library
#include <SPI.h>  // TODO: Clarify...

// TFT
#define cs   10
#define dc   9
#define rst  8

// Views:
#define ROW_HIGH  6
#define ROW2_HIGH  ROW_HIGH * 2
#define COL_WIDTH 30
#define LINDENT  5
#define TINDENT  3

#define CLR_WHITE "#FFFFFF"
#define CLR_BLACK "#000000"
#define CLR_RED "#FF0000"
#define CLR_GREEN "#00FF00"
#define CLR_BLUE "#0000FF"
#define CLR_YELLOW "#FFCC00"


void ViewsBegin();
void initBasicView(char * title);
void Color2RGB(char * clr, int *rgb);
void ViewsText(char * text, int x, int y, char * clr, char * bgclr = CLR_BLACK, int sz=1, int ln=0);
void ViewsBar(int x, int y, int l, int h, char *clr = CLR_BLACK);

#endif
