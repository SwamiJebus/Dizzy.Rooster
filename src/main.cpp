#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>

///////////////////////
//    Pin defines    //
///////////////////////

int PinSDA = 26;
int PinSCL = 27;

////////////////////////
//    Constructors    //
////////////////////////

TwoWire I2C = TwoWire(0);
U8G2_SH1106_128X64_NONAME_F_HW_I2C Display = U8G2_SH1106_128X64_NONAME_F_HW_I2C(U8G2_R0, 0, PinSCL, PinSDA);

// put function declarations here:
int myFunction(int, int);

void setup() 
{
  I2C.begin(PinSDA, PinSCL, 400000); // ESP32 supports "fast-mode" of between 100-400 kHz
  Display.begin();
}

void loop() 
{
  Display.firstPage();
  do 
  {
    Display.setFont(u8g2_font_ncenB14_tr);
    Display.drawStr(0,20,"Hello World!");
  }
  while (Display.nextPage()); 
}