#ifndef LCD_H
#define LCD_H
// Liquid Crystal Diode 2x16
#include <string>
#include <LiquidCrystal_I2C.h>

// onboard module
LiquidCrystal_I2C lcd(0x27, 20, 4);

// variables
int k;
unsigned long EngineRPM;
unsigned long EngineRPH;
unsigned long TransmissionRPM;
unsigned long TransmissionRPH;
unsigned long AxleRPM;
unsigned long AxleRPH;

float MPM;
float MPH;
float TireDiameter;
float Differential; 
float Gear;
  int Gearn;
float FirstGear;
float SecondGear;
float ThirdGear;
float FourthGear;
float FifthGear;
float SixthGear;
  int TireWidth;
float TireProfile;
  int TireSize;
  int RevPerMile;  
arduino::String topline;
arduino::String botline;
arduino::String str;
arduino::String strpm;
arduino::String strph;
arduino::String strHE;
arduino::String buf;

// functions
void LCDInit();
void LCDsetup();
void LCDloop();
void LCDtop();
void LCDbot();
void LCDcalc();
void LCDlog();
#endif // LCD_H