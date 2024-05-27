#include <Arduino.h>
#include <WiFi.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "rooster_graphics.h"

///////////////////////
//    Pin Mapping    //
///////////////////////

int Joystick_X  = D2;
int Joystick_Y  = D1;

/////////////////////
//    Variables    //
/////////////////////

int InputX_Raw = 0; // Analog values range from 0-4095
int InputY_Raw = 0;

///////////////////
//    Defines    //
///////////////////

#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16

#define SCREEN_WIDTH    128   // OLED display width, in pixels
#define SCREEN_HEIGHT   64    // OLED display height, in pixels

#define OLED_RESET      -1 
#define SCREEN_ADDRESS  0x3D

uint8_t RXAddress[] = {0x34, 0x85, 0x18, 0x03, 0x9b, 0x84}; // TODO Maybe don't hardcode this

////////////////////////
//    Constructors    //
////////////////////////

Adafruit_SSD1306 Display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

///////////////////
//    SETUP      //
///////////////////

void setup() 
{
  Serial.begin(115200);
  WiFi.mode(WIFI_MODE_STA);
  
  pinMode(Joystick_X, INPUT);
  pinMode(Joystick_Y, INPUT);

  if(!Display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Display Rooster logo.
  Display.clearDisplay();
  Display.drawBitmap(
    0,
    0,
    RoosterLogo, 
    Display.width(), 
    Display.height(), 
    1);
  Display.display();
  delay(1000);

}

///////////////////
//    LOOP       //
///////////////////

void loop() {

  InputX_Raw = analogRead(Joystick_X);
  InputY_Raw = analogRead(Joystick_Y);

  Serial.print("x = ");
  Serial.print(InputX_Raw);
  Serial.print(", y = ");
  Serial.println(InputY_Raw);

  delay(100);
}