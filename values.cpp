#include <Arduino.h>
#include <Adafruit_TFTLCD.h>
#include "defines.h"


extern Adafruit_TFTLCD tft;

void x_value(unsigned int value, uint8_t scale)
{
  static unsigned int last = 0;
  if(abs(value - last) <= DIFF_OFS) return;
  int pct = calcPercent(value);
  last = value;
  char buf[6]="     ";
  unsigned int volt = map(value,0,1023,0,500);
  sprintf_P(buf,PSTR("%d.%02d"),volt / 100,volt % 100);
  
  if(scale > 2) scale = 2;
  tft.fillRect(50 * scale,6,80 * scale,41 * scale,BLACK);
  tft.setTextSize(2 * scale);
  tft.setTextColor(YELLOW);
  tft.setCursor(80 * scale,10 * scale);
  tft.print(pct);
  tft.setCursor(70 * scale,30 * scale);
  tft.print(buf);
  
}


void y_value(unsigned int value, uint8_t scale)
{
  static unsigned int last = 0;
  if(abs(value - last) <= DIFF_OFS) return;
  int pct = calcPercent(value);
  last = value;  
  char buf[6]="     ";
  unsigned int volt = map(value,0,1023,0,500);
  sprintf_P(buf,PSTR("%d.%02d"),volt / 100,volt % 100);
  if(scale > 2) scale = 2;
  tft.fillRect(50 * scale,(50 * scale) + 6,80 * scale,41 * scale,BLACK);
  tft.setTextSize(2 * scale);
  tft.setTextColor(BLUE);
  tft.setCursor(80 * scale,60 * scale);
  tft.print(pct);
  tft.setCursor(70 * scale,80 * scale);
  tft.print(buf);  
  
}


void draw_info_box(uint8_t scale)
{
  if(scale > 2) scale = 2;
  tft.setTextColor(WHITE);
  tft.drawRect(5,5,150 * scale,50 * scale,WHITE);
  tft.setCursor(15 * scale,25 * scale);
  tft.setTextSize(1 * scale);
  tft.print("ASSE X");
  tft.setTextSize(2 * scale);
  tft.setCursor(130 * scale,10 * scale);
  tft.print("%");
  tft.setCursor(130 * scale,30 * scale);
  tft.print("V.");  

  tft.drawRect(5,5 + (50 * scale),150 * scale,50 * scale,WHITE);
  tft.setCursor(15 * scale,75 * scale);
  tft.setTextSize(1 * scale);
  tft.print("ASSE Y");
  tft.setTextSize(2 * scale);
  tft.setCursor(130 * scale,60 * scale);
  tft.print("%");
  tft.setCursor(130 * scale,80 * scale);
  tft.print("V.");  
  
}
