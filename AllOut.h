#ifndef ALLOUT_H
#define ALLOUT_H
// AllOut display:  
// HEI Tachometer signal
// VSS signal from tranmission
// Hall Effects Sensors & magnets mounted to rear axle
//
// libraries
// Organic Light Emmiting Diode 128x64  
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
// on board led matrix 12x8
// To use ArduinoGraphics APIs, please include BEFORE Arduino_LED_Matrix
#include "ArduinoGraphics.h"
#include "Arduino_LED_Matrix.h"
// Liquid Crystal Diode 2x16
#include <string>
#include <LiquidCrystal_I2C.h>

// define
#define BAUD 9600
#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64
#define OLED_RESET     -1 
#define SCREEN_ADDRESS 0x3C
#define MainPeriod 100
#define TachPPR 4
#define DrivePPR 8
#define AxlePPR 4 
// green tach
#define LEDPinG 11
#define TachPin 2
// yellow transmission
#define LEDPinY 12
#define DrivePin 4
// blue axle
#define LEDPinB 13                                                                                                                                  
#define AxlePin 6

// devices
// OLED screen
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// onboard led matrix
ArduinoLEDMatrix matrix;

// 12C LCD 1602 module
LiquidCrystal_I2C lcd(0x27, 20, 4);

//constants
const long  TenB = 10000000000;
const float TireDiameter = 24.9;
const float Differential = 3.364; 
const float FirstGear  = 2.97;
const float SecondGear = 2.10;
const float ThirdGear  = 1.46;
const float FourthGear = 1.00;
const float FifthGear  = 0.74;
const float SixthGear  = 0.50;
const int   TireWidth   = 225;
const float TireProfile = 0.60;
const int   TireSize    = 16;
const int   RevPerMile = 840;

//variables
// boolean
bool TESTING = false; 

//char
char Mtext[30];

// integers
int res;
int li;
int ri;
int i;
int j;
int k;
int Gearn;

// float
unsigned long EngineRPM;
unsigned long EngineRPH;
unsigned long TransmissionRPM;
unsigned long TransmissionRPH;
unsigned long AxleRPM;
unsigned long AxleRPH;
float MPM;
float MPH;
float Gear;
float GearRatio;
unsigned long TachPulseCnt;
unsigned long DrivePulseCnt;
unsigned long AxlePulseCnt;

// strings
arduino::String topline;
arduino::String botline;
arduino::String str;
arduino::String strpm;
arduino::String strph;
arduino::String strHE;
arduino::String buf;
 
// Structures
struct OLED
{
  arduino::String bmap;
};
OLED BMAP[8];
OLED NewOled;

struct PULSE
{
         unsigned long DiffMillis;
         unsigned long CurrMillis;
         unsigned long PrevMillis; 
         unsigned long DiffMicros;
         unsigned long CurrMicros;
volatile unsigned long PrevMicros;
volatile unsigned long Duration; 
         unsigned long _Duration;
 volatile unsigned int PulseCnt;
         unsigned long _PulseCnt;
                 float Freq;
                 float RPM;
};
// Root
PULSE Root;
// Tachometer
PULSE Tach;
// Driveshaft
PULSE Drive;
// Axle
PULSE Axle;

// using averages 
struct APULSE
{
                  byte numReadings; 
                   int readIndex;
         unsigned long DiffMillis;
         unsigned long ZeroDiff;
         unsigned long DiffMicros;
         unsigned long ZeroTimeout;  
volatile unsigned long LastTimeWeMeasured; 
volatile unsigned long PeriodBetweenPulses; 
volatile unsigned long PeriodAverage; 
         unsigned long FrequencyRaw; 
         unsigned long FrequencyReal;
         unsigned long RPM;
          unsigned int PulseCnt;
         unsigned long PeriodSum;
         unsigned long LastTimeCycleMeasure; 
         unsigned long CurrentMicros; 
          unsigned int AmountOfReadings;
          unsigned int ZeroDebouncingExtra;
         unsigned long Readings[2];
         unsigned long ReadIndex;
         unsigned long Total;
         unsigned long Average;
         unsigned long RemapedAmountOfReadings;
};
// Root
APULSE ARoot;
// Tachometer
APULSE ATach;
// Driveshaft
APULSE ADrive;
// Axle
APULSE AAxle;

// Test functions
void SetPulse();

// HE functions
void HESetup();
void HELoop();
void HELoopErpm();
void HEInit();
void HETach();
void HETachISR();
void HEATach();
void HEATachISR();
void HEDrive();
void HEDriveISR();
void HEADrive();
void HEADriveISR();
void HEAxle();
void HEAxleISR();
void HEAAxle();
void HEAAxleISR();

// LCD functions
void LCDInit();
void LCDsetup();
void LCDloop();
void LCDtop();
void LCDbot();
void LCDcalc();
void LCDlog();

// matrix functions
void Matrixsetup();
void Matrixloop();

// OLED functions
void OLEDsetup();
void OLEDloop();
void OLEDpopup();
#endif // ALLOUT_H

