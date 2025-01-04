#ifndef HALL_H
#define HALL_H
// Hall Effects Sensor, mounted to rear axle
// variables 
const int maxCnt = 10;
// Tachometer
    const int TachPin = 4;
unsigned long TachStart;
unsigned long TachEnd;
        float TachSeconds;
        float TachRPM;
 volatile int TachCnt;
          int TachNewValue;
          int TachOldValue;

// Driveshaft
const int DrivePin = 7; 
unsigned long DriveStart;
unsigned long DriveEnd;
        float DriveSeconds;
        float DriveRPM;
 volatile int DriveCnt;
          int DriveNewValue;
          int DriveOldValue;

// Axel
const int AxlesPin = 7; 
unsigned long AxlesStart;
unsigned long AxlesEnd;
        float AxlesSeconds;
        float AxlesRPM;
 volatile int AxlesCnt;
          int AxlesNewValue;
          int AxlesOldValue;

// functions
void HESetup();
void HELoop();
void HEInit();
void HETach();
void HEDrive();
void HEAxle();
#endif // HALL_H