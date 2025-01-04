// AllOut display
#ifndef ALLOUT_H
#define ALLOUT_H
// Liquid Crystal Diode 2x16
#include "LCD.h"
// Organic Light Emmiting Diode 128x64  
#include "OLED.h"
// on board led matrix 12x8
#include "Matrix.h"
// Hall Effects Sensor, mounted to rear axle
#include "Hall.h"
// VSS signal from tachometer
// Tachometer signal from HEI
#define BAUD 9600 
char string[99];
int res;
int li;
int ri;
#endif // ALLOUT_H