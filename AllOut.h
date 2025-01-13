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
// Buttons
#include <Bounce2.h>
// define
#define BAUD 9600
#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64
#define OLED_RESET     -1 
#define SCREEN_ADDRESS 0x3C
#define MainPeriod 100
#define TachPPR 16   // HEI is actually 4
#define DrivePPR 8   // 
#define AxlePPR 4 
// green tach
#define LEDPinG 13
#define TachPin 2
// yellow transmission
#define LEDPinY 12
#define DrivePin 3
// blue axle
#define LEDPinB 11                                                                                                                                  
#define AxlePin 4
// run mode buttons
#define TestPin 8
#define FeedBackPin 9
#define DebugPin 10
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
//QT
float QTvalue;
float QTrev;
int QTrpm;
int QToldtime;
int QTtime;

// bytes
byte TestPinState;
byte FeedBackPinState;
byte DebugPinState;
// boolean values changed by buttons!
bool TESTING; 
bool FEEDBACK;
bool DEBUG;

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

// Test/FeedBack/Debug functions
void loopTest();
void loopDebug();
void SetPulse();
void RunMode();

// HE functions
void HESetup();
void HELoop();
void HELoopDebugB();
void HELoopDebugA();
void HELoopTestRPM();
void HELoopTestGear();
void HELoopErpm();
void HEInit();
void HEInitBase();
void HEInitAvg();
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
void LCDLogTest();
void LCDLogFeed();
void LCDLogDebug();

// matrix functions
void Matrixsetup();
void Matrixloop();
void MatrixloopRPM();
void MatrixloopGear();
void MatrixloopTestI();
void MatrixloopTestJ(); 

// OLED functions
void OLEDsetup();
void OLEDloop();
void OLEDpopup();

#endif // ALLOUT_H
/*
using three inputs
tachometer signal for engine rpm from the HEI distributor
VSS for driveshaft rpm from transmission
hall effect on axle for axel rpm
MPH is calculated using axle rpm and tire dimensions
since even with the engine off and the transmission in neutral, you can still be coasting
only with live testing will I know if the VSS is senging a pulse, and what the actual pulse is
the book says it's either 8, 12, or 16 PPR.    
we'll display 
 top line = "RPM 6000 1 12000"; Engine RPM 6000 / Gear / Driveshaft RPM
 bot line = "3567 MPH 254.79";  Axle RPM / MPH 
to test this I added a intermittent button and three LED's
in the Tach ISR's I turn the Green LED on at the start and off at the end   
in the Drive ISR's I turn the Yellow LED on at the start and off at the end  
in the Axle ISR's I turn the Blue LED on at the start and off at the end
with a quick press of the button, I send 3v signal to all three inturrupt pins  
then I set the pulse count to what it "should be" since there is only one inturrupt signal where there would be many
using the Distributor as the driver, everything follows after that 
in the end, the real MPH is taken off the VSS and or the Axle     
I have two different RPM calculations, one is basic, the other is an Average
this gives me the need for 3x2 ISR, so I trigger the Average 'ISR' from the Basic
*/
/*
TransmissionRPH = 2020.202;  1st
TransmissionRPH = 2857.142;  2nd 
TransmissionRPH = 4109.589;  3rd
TransmissionRPH = 6000.000;  4th
TransmissionRPH = 8108.108;  5th
TransmissionRPH = 12000.000; 6th
AxleRPH = 600.535;  1st
AxleRPH = 849.328;  2nd 
AxleRPH = 1221.640; 3rd
AxleRPH = 1783.590; 4th
AxleRPH = 2410.258; 5th
AxleRPH = 3567.11;  6th
*/
/*
TireDiameter = 24.9;
Differential = 3.364; 
FirstGear  = 2.97;
SecondGear = 2.10;
ThirdGear  = 1.46;
FourthGear = 1.00;
FifthGear  = 0.74;
SixthGear  = 0.50;
TireWidth   = 225;
TireProfile = 0.60;
TireSize    = 16;
RevPerMile = 840;
*/
/*
MPH = ((EngineRPM / FirstGear)  / Differential) / RevPerMile)) * 60;  42.895405
MPH = ((EngineRPM / SecondGear) / Differential) / RevPerMile)) * 60;  60.666359
MPH = ((EngineRPM / ThirdGear)  / Differential) / RevPerMile)) * 60;  87.259831
MPH = ((EngineRPM / FourthGear) / Differential) / RevPerMile)) * 60; 127.399354
MPH = ((EngineRPM / FifthGear)  / Differential) / RevPerMile)) * 60; 172.161289
MPH = ((EngineRPM / SixthGear)  / Differential) / RevPerMile)) * 60; 254.798709
*/
