#include <Arduino.h>
#include <Adafruit_TFTLCD.h>
#include "defines.h"


extern Adafruit_TFTLCD tft;
 
void x_axis(unsigned int value, bool center)
{
  uint8_t ofs = 0;
  if(center) ofs = 40;
  static unsigned int last = 0;
  
  if(abs(value - last) <= DIFF_OFS) return;
  int pct = calcPercent(value);
  last = value;
  
  tft.fillRect(100 - ofs,118,201,5,BLACK);
  if(pct == 0) return;
  if(pct > 0)
  {
    tft.fillRect(200 - ofs,118,pct,5,YELLOW);
  }
  else
  {
    tft.fillRect(200 - ofs - abs(pct),118,abs(pct),5,YELLOW);
  }
  
}


void y_axis(unsigned int value, bool center)
{
  uint8_t ofs = 0;
  if(center) ofs = 40;
  static unsigned int last = 0;
  if(abs(value - last) <= DIFF_OFS) return;
  int pct = calcPercent(value);
  last = value;  
  tft.fillRect(198 - ofs,20,5,201,BLACK);
  if(pct == 0) return;
  if(pct > 0)
  {
    tft.fillRect(198 - ofs,120 - pct,5,pct,BLUE);
  }
  else
  {
    tft.fillRect(198 - ofs, 120,5,abs(pct),BLUE);
  }
}

void draw_axis(bool center)
{
  uint8_t ofs = 0;
  if(center) ofs = 40;

  for(byte i=0; i < 9; i++)
  {
    tft.drawLine(190 - ofs,20 + (i*25),210 - ofs,20 + (i*25),WHITE);
  }
  
  //tft.drawLine(100,120,300,120,WHITE);
  for(byte i=0; i < 9; i++)
  {
    tft.drawLine(100 - ofs + (i*25),110,100 - ofs + (i*25),130,WHITE);
  }
  tft.fillRect(100 - ofs,118,200,5,BLACK);
  tft.fillRect(198 - ofs,20,5,200,BLACK);
  
  tft.setTextSize(1);
  tft.setTextColor(WHITE);    
  tft.setCursor(215 - ofs,16);
  tft.print("100%");  
  tft.setCursor(215 - ofs,41);
  tft.print("75%");    
  tft.setCursor(215 - ofs,66);
  tft.print("50%");
  tft.setCursor(215 - ofs,91);
  tft.print("25%");
  tft.setCursor(165 - ofs,141);
  tft.print("25%");    
  tft.setCursor(165 - ofs,166);
  tft.print("50%");
  tft.setCursor(165 - ofs,191);
  tft.print("75%");
  tft.setCursor(165 - ofs,216);
  tft.print("100%");   
  tft.setCursor(80 - ofs,141);
  tft.print("100%");
  tft.setCursor(115 - ofs,141);
  tft.print("75%");
  tft.setCursor(140 - ofs,141);
  tft.print("50%");  
  tft.setCursor(240 - ofs,91);
  tft.print("50%");
  tft.setCursor(265 - ofs,91);
  tft.print("75%"); 
  tft.setCursor(290 - ofs,91);
  tft.print("100%");    
  
}
