#include <Arduino.h>
#include <U8g2lib.h>
#include <SPI.h>

///////////////////////
//    Pin defines    //
///////////////////////

int OLED_CLK  = 34;
int OLED_MOSI = 35;
int OLED_RES  = 32;
int OLED_DC   = 33;
int OLED_CS   = 25;

////////////////////////
//    Constructors    //
////////////////////////

U8G2_SH1106_128X64_NONAME_F_4W_HW_SPI Display(U8G2_R0, OLED_CS, OLED_DC, OLED_RES);

void setup() 
{
  Display.begin();
}

void loop() {
  Display.firstPage();
  do {
    Display.setFont(u8g2_font_ncenB14_tr);
    Display.drawStr(0,20,"Hello World!");
  } while ( Display.nextPage() );
  delay(1000);
}