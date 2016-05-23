#ifndef _GFXLCD_H
#define _GFXLCD_H


// LCD e TouchScreen STUFF
#define YP A3  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 23  // can be a digital pin
#define XP 22   // can be a digital pin

#define TS_MINX 200
#define TS_MINY 200
#define TS_MAXX 900
#define TS_MAXY 940

#define MINPRESSURE 100
#define MAXPRESSURE 1000

#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
// optional
#define LCD_RESET A4

// Assign human-readable names to some common 16-bit color values:
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define GREY    0x8410




#define SD_SS   49
#define ENC_A   48
#define ENC_B   47
#define ENC_BTN 46
#define PWR_ON  45


#define L_IN             90
#define H_IN             920
#define ZERO_OFS         15
#define DIFF_OFS         10

#define X_AX_CH         0
#define Y_AX_CH         1

#define DISPL_UPD       50

#define VIEW_ALL        0
#define VIEW_AXIS       1
#define VIEW_VALUE      2
#define VIEW_BUTTON     3

#define AXIS_MIN         150
#define AXIS_MAX         950
#define AXIS_THR         50
#define CHG_THR         2
#define Z_THR           100

int calcPercent(unsigned int adc);
int8_t check_touch_slide();
int16_t btn_color(uint8_t btn);
void x_axis(unsigned int value, bool center);
void y_axis(unsigned int value, bool center);
void draw_axis(bool center);
void check_buttons(bool big);
void draw_buttons(bool big);
void x_value(unsigned int value, uint8_t scale);
void y_value(unsigned int value, uint8_t scale);
void draw_info_box(uint8_t scale);
void draw_view(uint8_t view);
void printVA_data(void);


#endif