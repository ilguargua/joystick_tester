#include <SPI.h>
#include <SD.h>
#include <Wire.h>

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library
#include <TouchScreen.h>
#include <Adafruit_MCP23017.h>
#include <Adafruit_INA219.h>

#include <mcp3008.h>
#include <encoder.h>
#include <buttons.h>
#include "defines.h"


TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

Adafruit_MCP23017 IOexp;
Adafruit_INA219 currSens;
mcp3008 extADC;
Button enc_btn;
encoder enc(ENC_A,ENC_B);

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);



uint8_t cur_view = 0;
uint8_t last_view = 3;
uint8_t view_chgd = 0;
float ofs_24V = 0.0;
float load_current = 0.0;
float bus_volt = 0.0;
uint16_t ofs_5V = 0;


void setup(void) {

  pinMode(PWR_ON,OUTPUT);
  digitalWrite(PWR_ON, HIGH);  
  
  //Serial.begin(9600);
  
  tft.reset();
  
  tft.begin(0x9341);
  
  tft.setRotation(1);

  IOexp.begin();
  currSens.begin();
  
  IOexp.pinMode(8,OUTPUT);
  IOexp.pinMode(9,OUTPUT);
  
  ofs_24V = currSens.getShuntVoltage_mV();
  
  IOexp.digitalWrite(8,HIGH);
  
  
  //draw_view(0);
  
  SPI.begin();
  //pinMode(SS,OUTPUT);
  //digitalWrite(SS,LOW);
  //digitalWrite(SS,HIGH);
  
  pinMode(SD_SS,OUTPUT);
  digitalWrite(SD_SS,HIGH);
  SD.begin(SD_SS);
  
  ofs_5V = extADC.readADC(4) - extADC.readADC(5);
  
  enc_btn.assign(ENC_BTN);
  enc_btn.setMode(OneShotTimer);
  enc_btn.setTimer(2000);
  enc_btn.setPolarity(1);    
}


void loop()
{
  static unsigned long display_upd=0;
  //static unsigned long d = 0;
  unsigned int x_adc = 512;
  unsigned int y_adc = 512; 
  int8_t page_chg = 0;
  
  if(cur_view != last_view)
  {
    draw_view(cur_view);
    last_view = cur_view;
    view_chgd = 1;
  }
  /*
  if (millis() - d > 500)
  {
    d = millis();
    a = random(L_IN,H_IN);
    b = random(L_IN,H_IN);
  }  
  */    
  if(millis() - display_upd > DISPL_UPD)
  {
    display_upd = millis();
    x_adc = extADC.readADC(X_AX_CH);
    y_adc = extADC.readADC(Y_AX_CH);
    switch(cur_view)
    {
      case 0:
        x_axis(x_adc,false);
        x_value(x_adc,1);
        y_axis(y_adc,false);
        y_value(y_adc,1);
        check_buttons(false);
        //print_touch();
        break;
      case 1:
        x_axis(x_adc,true);
        y_axis(y_adc,true);
        break;
      case 2:
        x_value(x_adc,2);
        y_value(y_adc,2);
        break;
      case 3:
        check_buttons(true);
        break;
        
    }
    //axis_redraw();
    //check_touch();
    view_chgd = 0;
  }
  //view_button();
  page_chg = enc.check();
  if(page_chg == 0) page_chg = check_touch_slide();
  if(page_chg != 0)
  {
    if(page_chg < 0)
    {
      if(cur_view == 0) cur_view = 3;
      else cur_view--;
    }
    else
    {
      if(cur_view == 3) cur_view = 0;
      else cur_view++;
    }
    
  }
  if(enc_btn.check() == Hold) digitalWrite(PWR_ON, LOW);
  
}


#define BUFFPIXEL 20

void bmpDraw(char *filename, int x, int y) {

  File     bmpFile;
  int      bmpWidth, bmpHeight;   // W+H in pixels
  uint8_t  bmpDepth;              // Bit depth (currently must be 24)
  uint32_t bmpImageoffset;        // Start of image data in file
  uint32_t rowSize;               // Not always = bmpWidth; may have padding
  uint8_t  sdbuffer[3*BUFFPIXEL]; // pixel in buffer (R+G+B per pixel)
  uint16_t lcdbuffer[BUFFPIXEL];  // pixel out buffer (16-bit per pixel)
  uint8_t  buffidx = sizeof(sdbuffer); // Current position in sdbuffer
  boolean  goodBmp = false;       // Set to true on valid header parse
  boolean  flip    = true;        // BMP is stored bottom-to-top
  int      w, h, row, col;
  uint8_t  r, g, b;
  uint32_t pos = 0, startTime = millis();
  uint8_t  lcdidx = 0;
  boolean  first = true;

  if((x >= tft.width()) || (y >= tft.height())) return;

  Serial.println();
  Serial.print(F("Loading image '"));
  Serial.print(filename);
  Serial.println('\'');
  // Open requested file on SD card
  if ((bmpFile = SD.open(filename)) == NULL) {
    Serial.println(F("File not found"));
    return;
  }

  // Parse BMP header
  if(read16(bmpFile) == 0x4D42) { // BMP signature
    Serial.println(F("File size: ")); Serial.println(read32(bmpFile));
    (void)read32(bmpFile); // Read & ignore creator bytes
    bmpImageoffset = read32(bmpFile); // Start of image data
    Serial.print(F("Image Offset: ")); Serial.println(bmpImageoffset, DEC);
    // Read DIB header
    Serial.print(F("Header size: ")); Serial.println(read32(bmpFile));
    bmpWidth  = read32(bmpFile);
    bmpHeight = read32(bmpFile);
    if(read16(bmpFile) == 1) { // # planes -- must be '1'
      bmpDepth = read16(bmpFile); // bits per pixel
      Serial.print(F("Bit Depth: ")); Serial.println(bmpDepth);
      if((bmpDepth == 24) && (read32(bmpFile) == 0)) { // 0 = uncompressed

        goodBmp = true; // Supported BMP format -- proceed!
        Serial.print(F("Image size: "));
        Serial.print(bmpWidth);
        Serial.print('x');
        Serial.println(bmpHeight);

        // BMP rows are padded (if needed) to 4-byte boundary
        rowSize = (bmpWidth * 3 + 3) & ~3;

        // If bmpHeight is negative, image is in top-down order.
        // This is not canon but has been observed in the wild.
        if(bmpHeight < 0) {
          bmpHeight = -bmpHeight;
          flip      = false;
        }

        // Crop area to be loaded
        w = bmpWidth;
        h = bmpHeight;
        if((x+w-1) >= tft.width())  w = tft.width()  - x;
        if((y+h-1) >= tft.height()) h = tft.height() - y;

        // Set TFT address window to clipped image bounds
        tft.setAddrWindow(x, y, x+w-1, y+h-1);

        for (row=0; row<h; row++) { // For each scanline...
          // Seek to start of scan line.  It might seem labor-
          // intensive to be doing this on every line, but this
          // method covers a lot of gritty details like cropping
          // and scanline padding.  Also, the seek only takes
          // place if the file position actually needs to change
          // (avoids a lot of cluster math in SD library).
          if(flip) // Bitmap is stored bottom-to-top order (normal BMP)
            pos = bmpImageoffset + (bmpHeight - 1 - row) * rowSize;
          else     // Bitmap is stored top-to-bottom
            pos = bmpImageoffset + row * rowSize;
          if(bmpFile.position() != pos) { // Need seek?
            bmpFile.seek(pos);
            buffidx = sizeof(sdbuffer); // Force buffer reload
          }

          for (col=0; col<w; col++) { // For each column...
            // Time to read more pixel data?
            if (buffidx >= sizeof(sdbuffer)) { // Indeed
              // Push LCD buffer to the display first
              if(lcdidx > 0) {
                tft.pushColors(lcdbuffer, lcdidx, first);
                lcdidx = 0;
                first  = false;
              }
              bmpFile.read(sdbuffer, sizeof(sdbuffer));
              buffidx = 0; // Set index to beginning
            }

            // Convert pixel from BMP to TFT format
            b = sdbuffer[buffidx++];
            g = sdbuffer[buffidx++];
            r = sdbuffer[buffidx++];
            lcdbuffer[lcdidx++] = tft.color565(r,g,b);
          } // end pixel
        } // end scanline
        // Write any remaining data to LCD
        if(lcdidx > 0) {
          tft.pushColors(lcdbuffer, lcdidx, first);
        } 
        Serial.print(F("Loaded in "));
        Serial.print(millis() - startTime);
        Serial.println(" ms");
      } // end goodBmp
    }
  }

  bmpFile.close();
  if(!goodBmp) Serial.println(F("BMP format not recognized."));
}

// These read 16- and 32-bit types from the SD card file.
// BMP data is stored little-endian, Arduino is little-endian too.
// May need to reverse subscript order if porting elsewhere.

uint16_t read16(File f) {
  uint16_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read(); // MSB
  return result;
}

uint32_t read32(File f) {
  uint32_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read();
  ((uint8_t *)&result)[2] = f.read();
  ((uint8_t *)&result)[3] = f.read(); // MSB
  return result;
}










