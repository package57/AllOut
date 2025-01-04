#ifndef OLED_H
#define OLED_H
// Organic Light Emmiting Diode 128x64  
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64
#define OLED_RESET     -1 
#define SCREEN_ADDRESS 0x3C
// module
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// variables
int i;
int j;
struct OLED
{
  arduino::String bmap;
};
OLED BMAP[8];
OLED NewOled;

// functions
void OLEDsetup();
void OLEDloop();
void OLEDpopup();
#endif // OLED_H