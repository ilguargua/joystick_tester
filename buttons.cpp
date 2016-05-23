#include <Arduino.h>
#include <Adafruit_TFTLCD.h>
#include <Adafruit_MCP23017.h>
#include "defines.h"


extern Adafruit_TFTLCD tft;
extern uint8_t view_chgd;
extern Adafruit_MCP23017 IOexp;

int16_t small_btn[7][2] = 
{
  {35,170},{80,170},{20,195},{95,195},{35,220},{80,220},{57,195}
};

int16_t big_btn[7][2] = 
{
  {130,60},{190,60},{80,120},{240,120},{130,180},{190,180},{160,120}
};

void check_buttons(bool big)
{
  static byte last = 0;
  byte btns = IOexp.readGPIO(0);
  if(btns != last)
  {
    last = btns;
    if(big)
    {
      for(uint8_t i=0;i<7;i++)
      {
        if(bitRead(btns,i) == 1) tft.fillCircle(big_btn[i][0],big_btn[i][1],15,btn_color(i));
        else tft.fillCircle(big_btn[i][0],big_btn[i][1],15,BLACK);
      }
    }
    else
    {
      for(uint8_t i=0;i<7;i++)
      {
        if(bitRead(btns,i) == 1) tft.fillCircle(small_btn[i][0],small_btn[i][1],7,btn_color(i));
        else tft.fillCircle(small_btn[i][0],small_btn[i][1],7,BLACK);
      }
    }
  }
}

void draw_buttons(bool big)
{
  if(big) for(uint8_t i=0;i<7;i++) tft.drawCircle(big_btn[i][0],big_btn[i][1],16,WHITE);
  else for(uint8_t i=0;i<7;i++) tft.drawCircle(small_btn[i][0],small_btn[i][1],8,WHITE);
  
}

int16_t btn_color(uint8_t btn)
{
  int16_t c;
    switch(btn)
    {
      case 0:
      case 1:
        c = BLUE;
        break;
      case 2:
      case 3:
        c = YELLOW;
        break;
      case 4:
      case 5:
        c = GREEN;
        break;
      case 6:
        c = RED;
    }
  return c;  
}
