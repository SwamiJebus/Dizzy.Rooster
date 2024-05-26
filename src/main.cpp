#include <Arduino.h>
#include <U8g2lib.h>
#include <SPI.h>
#include <WiFi.h>

///////////////////////
//    Pin Mapping    //
///////////////////////

int OLED_CLK  = 34;
int OLED_MOSI = 35;
int OLED_RES  = 32;
int OLED_DC   = 33;
int OLED_CS   = 25;

int Joystick_X  = 27;
int Joystick_Y  = 26;

/////////////////////
//    Variables    //
/////////////////////

int InputX_Raw = 0; // Analog values range from 0-4095
int InputY_Raw = 0;

uint8_t RXAddress[] = {0xD0, 0xEF, 0x76, 0x47, 0x60, 0x98}; // TODO Maybe don't hardcode this

////////////////////////
//    Constructors    //
////////////////////////

U8G2_SH1106_128X64_NONAME_F_4W_HW_SPI Display(U8G2_R0, OLED_CS, OLED_DC, OLED_RES);

void setup() 
{
  Serial.begin(115200);
  WiFi.mode(WIFI_MODE_STA);
  
  delay(1000);

  Serial.println(WiFi.macAddress());
  Display.begin();
}

void loop() {
  // Display.firstPage();
  // do {
  //   Display.setFont(u8g2_font_ncenB14_tr);
  //   Display.drawStr(0,20,"Hello World!");
  // } while ( Display.nextPage() );

  InputX_Raw = analogRead(Joystick_X);
  InputY_Raw = analogRead(Joystick_Y);

  Serial.print("x = ");
  Serial.print(InputX_Raw);
  Serial.print(", y = ");
  Serial.println(InputY_Raw);

  delay(100);
}