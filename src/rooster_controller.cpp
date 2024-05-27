#include <Arduino.h>
#include <WiFi.h>
#include <Adafruit_GFX.h>
#include <esp_now.h>
#include <Adafruit_SSD1306.h>
#include "rooster_graphics.h"
#include "rooster_protocol.h"

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

// Display for health of outgoing packets.
String TransmitHealth;

// Hold telemetry values
int PeerStatus = 0;

///////////////////
//    Defines    //
///////////////////

#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16

#define SCREEN_WIDTH    128   // OLED display width, in pixels
#define SCREEN_HEIGHT   64    // OLED display height, in pixels

#define OLED_RESET      -1 
#define SCREEN_ADDRESS  0x3D

uint8_t SelfAddress[] = {0x34, 0x85, 0x18, 0x03, 0x9b, 0x84}; // TODO Maybe don't hardcode this
uint8_t RemoteAddress[] = {0xA0, 0x76, 0x4E, 0x40, 0x2E, 0x14}; // TODO Maybe don't hardcode this

////////////////////////
//    Constructors    //
////////////////////////

Adafruit_SSD1306    Display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
RoosterPacket       CommandPacket;
RoosterPacket       PeerPacket;
esp_now_peer_info_t PeerInfo;

/////////////////////
//    Functions    //
/////////////////////

// Callback when data is sent.
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  if (status == 0){
    TransmitHealth = "Healthy";
  }
  else{
    TransmitHealth = "!!WARN!!";
  }
}

// Callback when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&PeerPacket, incomingData, sizeof(PeerPacket));
  PeerStatus = PeerPacket.Status;
}

// Display important information during nominal use.
void DisplayMain()
{
  Display.clearDisplay();

  Display.setTextSize(1);             // Normal 1:1 pixel scale
  Display.setTextColor(SSD1306_WHITE);        // Draw white text
  Display.setCursor(0,0);             // Start at top-left corner

  Display.print(F("X Input: "));
  Display.println(InputX_Raw);
  Display.print(F("Y Input: "));
  Display.println(InputY_Raw);
  Display.print(F("Transmit: "));
  Display.println(TransmitHealth);
  Display.print(F("Peer Status: "));
  Display.println(PeerStatus);

  Display.display();
}

void BuildCommandPacket()
{
  CommandPacket.Ch1 = InputX_Raw;
  CommandPacket.Ch2 = InputY_Raw;
  CommandPacket.Status = 1; // TODO: Set up some flags to display transmitter health.
};

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
    for(;;); // Don't proceed, loop forever.
  }

  ////// Initialize ESP-NOW. //////
  if (esp_now_init() != ESP_OK) 
  {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_register_send_cb(OnDataSent);
  esp_now_register_recv_cb(OnDataRecv);

  // Register peer
  memcpy(PeerInfo.peer_addr, RemoteAddress, 6);
  PeerInfo.channel = 0;
  PeerInfo.encrypt = false;

  // Add peer
  if (esp_now_add_peer(&PeerInfo) != ESP_OK)
  {
    Serial.println("Failed to add peer");
    return;
  }

  // Initialize text settings.
  Display.setTextSize(1);               // Normal 1:1 pixel scale.
  Display.setTextColor(SSD1306_WHITE);  // Draw white text.

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

  BuildCommandPacket();
  esp_err_t result = esp_now_send(RemoteAddress, (uint8_t *) &CommandPacket, sizeof(CommandPacket)); 

  // TODO: Take advantage of frame buffer.
  DisplayMain();

  delay(100);
}