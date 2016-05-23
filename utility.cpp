#include <Arduino.h>
#include "defines.h"
#include <TouchScreen.h>
#include <Adafruit_TFTLCD.h>
#include <Adafruit_INA219.h>


extern Adafruit_TFTLCD tft;
extern TouchScreen ts;
extern Adafruit_INA219 currSens;
extern float ofs_24V;
extern uint16_t ofs_5V;

void draw_view(uint8_t view)
{
  if(view > 3) view = 3;
  tft.fillScreen(BLACK);
  switch(view)
  {
    case 0:
      draw_axis(false);
      draw_info_box(1);
      draw_buttons(false);
      break;
    case 1:
      draw_axis(true);
      break;
    case 2:
      draw_info_box(2);
      break;
    case 3:
      draw_buttons(true);
      break;
  }

  //btn1.drawButton();
}

void printVA_data(void)
{
  char buf[20];
  uint16_t in = ofs_24V * 100;
  uint16_t act = (currSens.getShuntVoltage_mV() - ofs_24V) * 100;
  uint16_t volt = currSens.getBusVoltage_V() * 100;
  tft.fillRect(250,15,98,50,BLACK);
  tft.setTextSize(1);
  tft.setTextColor(WHITE);
  sprintf(buf,"%d.%02d mA", in / 100, in % 100);
  tft.setCursor(250,15);
  tft.print(ofs_5V);
  sprintf(buf,"%d.%02d mA", act / 100, act % 100);
  tft.setCursor(250,25);
  tft.print(buf);
  sprintf(buf,"%d.%02d V", volt / 100, volt % 100);
  tft.setCursor(250,35);
  tft.print(buf);
  
}

/*
int16_t btn_color(uint8_t btn)
{
  int16_t c;
    switch(btn)
    {
      case 0:
      case 1:
        c = GREEN;
        break;
      case 2:
      case 3:
        c = BLUE;
        break;
      case 4:
      case 5:
        c = YELLOW;
        break;
      case 6:
        c = RED;
    }
  return c;  
}
*/

int calcPercent(unsigned int adc)
{
  int ret;
  if (abs(512 - adc) <= ZERO_OFS) return 0;
  if (adc < 512) 
  {
    ret = map(adc,512,L_IN,1,99);
    return ret - (ret *2);
  }  
  else return map(adc,512,H_IN,1,99);
}



int8_t check_touch_slide()
{
  static int16_t x_min = AXIS_MIN;
  static int16_t x_max = AXIS_MAX;
  static int16_t y_min = AXIS_MIN;
  static int16_t y_max = AXIS_MAX;
  static uint8_t ct_n = 0;
  static uint8_t ct_p = 0;
  static uint8_t z_ct = 0;
  int8_t ret = 0;

  TSPoint p=ts.getPoint();
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  if(p.z > Z_THR)
  {
    if(p.x - x_min > AXIS_THR)
    {
      x_min = p.x;
      ct_n++;
    }
    if(x_max - p.x > AXIS_THR)
    {
      x_max = p.x;
      ct_p++;
    }
    if(p.y - y_min > AXIS_THR)
    {
      y_min = p.y;
      ct_p++;
    }
    if(y_max - p.y > AXIS_THR)
    {
      y_max = p.y;
      ct_n++;
    }
    
  }
  else
  {
    if(z_ct > 0)
    {
      z_ct = 0;
      ct_n = ct_p = 0;
      x_min = y_min = AXIS_MIN;
      x_max = y_max = AXIS_MAX;
    }
    else z_ct++;
  }

  /*
  tft.fillRect(300,15,38,50,BLACK);
  tft.setTextSize(1);
  tft.setTextColor(WHITE);
  tft.setCursor(300,15);
  tft.print(x_max);
  tft.setCursor(300,25);
  tft.print(ct_p);
  tft.setCursor(300,35);
  tft.print(p.x);
  */
  if(ct_n > CHG_THR)
  {
    //cur_view += 1;
    //if(cur_view == 4) cur_view = 0;
    ret++;
    ct_n = ct_p = 0;
    x_min = y_min = AXIS_MIN;
    x_max = y_max = AXIS_MAX;
    z_ct = 0;
  }
  if(ct_p > CHG_THR)
  {
    //if(cur_view == 0) cur_view = 3;
    //else cur_view--;
    ret--;
    ct_n = ct_p = 0;
    x_min = y_min = AXIS_MIN; 
    x_max = y_max = AXIS_MAX;
    z_ct = 0;
  }
  return ret;
  
}