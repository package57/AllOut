// Techometer & Speedometer Combined All Out
#include "AllOut.h"
void setup() 
{

  Serial.begin(BAUD);

PinSetup();

ButtonSetup();

RunSetup();
  
// need to add SSD file system 

// external OLED   
  OLEDsetup();

  NewOled.bmap = "setup"; OLEDloop();

// external Hall Effect sensors
  HESetup();

// external 16x2 LCD display - the AllOut display
  LCDsetup();

// on board led matrix 
  Matrixsetup();

  str = "";
  str += "Test Mode ";
  str += TESTING;
  str2 = "";
  str2 += "Debug Mode ";
  str2 += DEBUG;
  str3 = "";
  str3 += "FeedBack Mode ";
  str3 += FEEDBACK;

  if  (DEBUG)
  {
    Serial.println(str);
    Serial.println(str2);
    Serial.println(str3);
  }

  NewOled.bmap = str; OLEDloop();
  NewOled.bmap = str2; OLEDloop();
  NewOled.bmap = str3; OLEDloop();

}
void PinSetup()
{

//pinMode (DontusePin0, OUTPUT);    // 0 don't use
//pinMode (DontusePin1, OUTPUT);    // 1 don't use
  pinMode(TachPin, INPUT_PULLUP);   // 2 HEI Tachometer signal
  pinMode(DrivePin, INPUT_PULLUP);  // 3 Transmission VSS signal 
  pinMode(AxlePin, INPUT_PULLUP);   // 4 Axle hall effect sensor
  pinMode (UnusedPin5, OUTPUT);     // 5 unused
  pinMode (UnusedPin6, OUTPUT);     // 6 unused 
  pinMode (UnusedPin7, OUTPUT);     // 7 unused

}
void ButtonSetup()
{

// run mode buttons
  TestButb.attach(TestBut, INPUT);  // 8 toggle Testing runmode ( a simulation )
  TestButb.interval(5);              
  FeedBackButb.attach(FeedBackBut, INPUT);  // 9 toggle Feedback runmode
  FeedBackButb.interval(5);
  DebugButb.attach(DebugBut, INPUT ); // 10 toggle Debug runmode
  DebugButb.interval(5);

}
void RunSetup()
{

// LED ouputs
  pinMode(LEDPinB, OUTPUT);         // 11 blue LED  ( flash at startup, blink during Tach ISR )
  pinMode(LEDPinY, OUTPUT);         // 12 yellow LED ( flash at startup, blink during Drive ISR )
  pinMode(LEDPinG, OUTPUT);         // 13 green LED ( flash at startup, blink during Axle ISR )
  //  am I running? flash the LED's
  digitalWrite(LEDPinG, HIGH);
  digitalWrite(LEDPinY, HIGH);
  digitalWrite(LEDPinB, HIGH);
  delay(2000);
  digitalWrite(LEDPinG, LOW);
  digitalWrite(LEDPinY, LOW);
  digitalWrite(LEDPinB, LOW);

  TESTING = true;   // defaul is simulation mode
  FEEDBACK = false; // output bread crumbs to OLED
  DEBUG = false;    // output bread crums to serial monitor

}
void HESetup()
{

  WhatWhat("HESetup");

  TachPulseCnt = 0;
  DrivePulseCnt = 0;
  AxlePulseCnt = 0;
  GearRatio = 0.0;
 
  Root.DiffMillis = 0;
  Root.CurrMillis = 0;
  Root.PrevMillis = 0;
  Root.Duration = 0; 
  Root.PulseCnt = 0;
  Root._PulseCnt = 0;
  Root.RPM = 0.0;

// Tachometer
  Tach = Root;
// Driveshaft
  Drive = Root;
// Axel
  Axle = Root;

  AttachTachISR();
  
  AttachDriveISR();
  
  AttachAxleISR();

}
void loop()
{
    
  WhatWhat("loop");

  if (TESTING)
  {
    Simulation();
  }
  else
  {
    RunMode();
    HELoop();
    Matrixloop();
    LCDloop();
  }
  
  if (DEBUG)
  {  
    str = "";
    str = "Tach Pulse ";
    str += String(Tach._PulseCnt);
    Serial.println(str);  
    str = "";
    str = "Drive Pulse ";
    str += String(Drive._PulseCnt);
    Serial.println(str);
    str = "";
    str = "Axle Pulse ";
    str += String(Axle._PulseCnt);
    Serial.println(str);
  }

}
void HELoop() 
{

  WhatWhat("HEloop");

// Tachometer function
  HETach();
// Transmission function
  HEDrive();
// Axle function
  HEAxle();

  if (TESTING)
  {
    HELoopTestRPM();
    HELoopTestGear();
  }
  else
  {
    HELoopErpm();
  }

}
void HETach()
{

  WhatWhat("HETach");

// The frequency of the pulses corresponds to the engine's RPM. For an 8-cylinder engine, 
// there are 4 pulses per crankshaft revolution (ppr) from the HEI distributor
//  500 RPM x 4 ppr =   2000 pulses per minute / 60 = 33.33 pulses per second
// 6000 RPM x 4 ppr = 24,000 pulses per minute / 60 = 400 pulses per second
// (x pulses per second * 60) / 4 = RPM
   
  Tach.CurrMillis = millis();
  Tach.DiffMillis = Tach.CurrMillis - Tach.PrevMillis;
  Tach.Duration  += Tach.DiffMillis;  
  
  if (Tach.Duration > OneK) 
  {
    DetachTachISR();     
    Tach._PulseCnt = Tach.PulseCnt;
    Tach.RPM = Tach._PulseCnt / TachPPR;
    Tach.Duration = 0;
    Tach.PulseCnt = 0;
    AttachTachISR();
    Tach.PrevMillis = Tach.CurrMillis;
  }

  if (DEBUG)
  {
    Serial.print("Tachometer pulse ");
    Serial.print(Tach._PulseCnt);
    Serial.print(" RPM ");
    Serial.println(Tach.RPM);
  }

}
void HEDrive()
{

  WhatWhat("HEDrive");

// The number of pulses per driveshaft revolution can vary depending on the specific T56 model and application.
// Common setups include 8, 12, or 16 PPR.
// 4th gear 1:1
//  500 RPM x 8 ppr =   4000 pulses per minute / 60 = 66.66 pulses per second
// 6000 RPM x 8 ppr = 48,000 pulses per minute / 60 = 800 pulses per second
// (y pulses per second * 60) / 8 = RPM

  Drive.CurrMillis = millis();
  Drive.DiffMillis = Drive.CurrMillis - Drive.PrevMillis;
  Drive.Duration  += Drive.DiffMillis;  
  
  if (Drive.Duration > OneK)
  {
    DetachDriveISR();     
    Drive._PulseCnt = Drive.PulseCnt;
    Drive.RPM = Drive._PulseCnt / DrivePPR;
    Drive.Duration = 0;
    Drive.PulseCnt = 0;
    AttachDriveISR();
    Drive.PrevMillis = Drive.CurrMillis;
  }  

  if (DEBUG)
  {
    Serial.print("Drive pulse ");
    Serial.print(Drive._PulseCnt);
    Serial.print(" RPM ");
    Serial.println(Drive.RPM);
  }

}
void HEAxle()
{

  WhatWhat("HEAxle");

// The number of pulses per axle revolution is 1
//  500 RPM x 1 ppr =   500 pulses per minute / 60 = 8.33 pulses per second
// 6000 RPM x 1 ppr = 6,000 pulses per minute / 60 = 100 pulses per second
// (z pulses per second * 60) / 8 = RPM
  
  Axle.CurrMillis = millis();
  Axle.DiffMillis = Axle.CurrMillis - Axle.PrevMillis;
  Axle.Duration  += Axle.DiffMillis;  
  
  if (Axle.Duration > OneK)
  {
    DetachAxleISR();     
    Axle._PulseCnt = Axle.PulseCnt;
    Axle.RPM = Axle._PulseCnt / AxlePPR;
    Axle.Duration = 0;
    Axle.PulseCnt = 0;
    AttachAxleISR();
    Axle.PrevMillis = Axle.CurrMillis;
  }   
  
  if (DEBUG)
  {
    Serial.print("Axle pulse ");
    Serial.print(Axle._PulseCnt);
    Serial.print(" RPM ");
    Serial.println(Axle.RPM);
  }

}
void HETachISR() 
{

  Tach.PulseCnt++;

}
void HEDriveISR()
{

  Drive.PulseCnt++;

}
void HEAxleISR()
{

  Axle.PulseCnt++;

}
void AttachTachISR()
{

  WhatWhat("AttachTachISR");

  TachISRVector = digitalPinToInterrupt(TachPin);  

  if (TachISRVector != TachPin)
  {
    Serial.println("no Tach Vector ");
    exit(0);
  }   

  attachInterrupt(TachISRVector, HETachISR, RISING); 

}
void DetachTachISR()
{
  
  WhatWhat("DetachTachISR");
  
  detachInterrupt(TachISRVector);

}
void AttachDriveISR()
{
  
  WhatWhat("AttachDriveISR");

  DriveISRVector = digitalPinToInterrupt(DrivePin);  
  
  if (DriveISRVector != DrivePin)
  {
    Serial.println("no Drive Vector ");
    exit(0);
  }   
  
  attachInterrupt(DriveISRVector, HEDriveISR, RISING); 

}
void DetachDriveISR()
{

  WhatWhat("DetachDriveISR");

  detachInterrupt(DriveISRVector);

}
void AttachAxleISR()
{

  WhatWhat("AttachAxleISR");

  AxleISRVector = digitalPinToInterrupt(AxlePin);  
  
  if (AxleISRVector != AxlePin)
  {
    Serial.println("no Axle Vector ");
    exit(0);
  }

  attachInterrupt(AxleISRVector, HEAxleISR, RISING); 

}
void DetachAxleISR()
{
  
  WhatWhat("DetachDriveISR");

  detachInterrupt(AxleISRVector);

}
void Simulation()
{

  WhatWhat("Simulation");

//go through the RPM range by 1000
// RPM ranges is limited to 6000 within HEI 
  for (i = 0; i < 6; i++)
  {
//  go though all the gears from 1 to 6 
    for (j = 0; j < 6; j++)
    {
      HELoop();
      Matrixloop();
      LCDloop();
      RunMode();
    }
  }

}
void RunMode()
{

  WhatWhat("RunMode");

  BOOLEAN = TestButb.update();
  BOOLEAN = FeedBackButb.update();
  BOOLEAN = DebugButb.update();

  if (TestButb.changed())
  {
    BounceRead = TestButb.read();
    if (BounceRead == LOW)
    {
      TESTING = !TESTING;
      Serial.println("Test Mode Changed");
      str = "";
      str += "Test Mode ";
      str += TESTING;
      Serial.println(str);
      NewOled.bmap = str; OLEDloop();
      exit(0);
    }
  }

  if (DebugButb.changed())
  {
    BounceRead = DebugButb.read();
    if (BounceRead == LOW)
    {
      DEBUG = !DEBUG;
      Serial.println("Debug Mode Changed");
      str = "";
      str += "Debug Mode ";
      str += DEBUG;
      Serial.println(str);
      NewOled.bmap = str; OLEDloop();
      exit(0);
    }
  }

  if (FeedBackButb.changed())
  {
    BounceRead = FeedBackButb.read();
    if (BounceRead == LOW)
    {
      FEEDBACK = !FEEDBACK;
      Serial.println("FeedbBack Mode Changed");
      str = "";
      str += "FeedBack Mode ";
      str += FEEDBACK;
      Serial.println(str);
      NewOled.bmap = str; OLEDloop();
      exit(0);
    }
  } 
       
}
void LCDsetup() 
{

  WhatWhat("LCDsetup");
  
  LCDInit();
  
  lcd.init();
  lcd.backlight();
  topline = "RPM 6000 1 12000"; // Engine RPM 6000 / Gear / Driveshaft RPM
  botline = " 3567 MPH 254.79"; // Axle RPM / MPH 
  lcd.setCursor(0, 0);  // l-r 0-15  t-b 0-1
  lcd.print(topline);
  lcd.setCursor(0, 1);
  lcd.print(botline);

}
void LCDInit()
{

  WhatWhat("LCDInit");

  EngineRPM = 6000;
  EngineRPH = 360000;
  TransmissionRPH = 2020.202;  
  AxleRPH = 600.535;  
  Gear = 1.000;
  TransmissionRPH = EngineRPH / FirstGear;
  AxleRPH = TransmissionRPH / Differential;
  MPH = AxleRPH / RevPerMile;
  
}
void Matrixsetup()
{
  
  WhatWhat("Matrixsetup");
  
  matrix.begin();
  matrix.beginDraw();
  matrix.stroke(0xFFFFFFFF);
  Mtext[0] = 'A';
  Mtext[1] = 'l';
  Mtext[2] = 'l';
  matrix.textFont(Font_4x6);
  matrix.beginText(0, 1, 0xFFFFFF);
  matrix.println(Mtext);
  matrix.endText();
  matrix.endDraw();

  delay(500);

  matrix.beginDraw();
  matrix.stroke(0xFFFFFFFF);
  matrix.textScrollSpeed(50);
  Mtext[0] = 'O';
  Mtext[1] = 'u';
  Mtext[2] = 'T';
  matrix.textFont(Font_5x7);
  matrix.beginText(0, 1, 0xFFFFFF);
  matrix.println(Mtext);
  matrix.endText(SCROLL_LEFT);
  matrix.endDraw();

}

void OLEDsetup()
{

// initialize the OLED object
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) 
  {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); 
  }
  
  for (i = 0; i < 8; i++)
  {
    BMAP[i].bmap = "";
  }

  NewOled.bmap = "All Out";

  display.setTextSize(1);
  display.setTextColor(WHITE);

}
void LCDloop()
{

  WhatWhat("LCDloop");

  LCDcalc();
 
  LCDtop();

  LCDbot();

  LCDlog();

}
void LCDcalc()
{

  WhatWhat("LCDcalc");

  EngineRPH = EngineRPM * 60;
 
  TransmissionRPM = EngineRPM / Gear;

  TransmissionRPH = EngineRPH / Gear;
 
  AxleRPM = TransmissionRPM / Differential;
  
  AxleRPH = TransmissionRPH / Differential;

  MPM = AxleRPM * 60;  
  MPM /= RevPerMile;
  MPH = AxleRPH / RevPerMile;

}
void LCDtop()
{

  WhatWhat("LCDtop");

//            0123456789012345
// topline = "RPM 6000 g 12000" 
  str = String(EngineRPM);
  lcd.setCursor(0, 0);  // l-r 0-15  t-b 0-1
  lcd.print("RPM ");
  
  if (EngineRPM > 999)
  {
    lcd.setCursor(4, 0);  // l-r 0-15  t-b 0-1
    lcd.print(str);       // EngineRPM  
  }
  else
  {
    if (EngineRPM > 99)
    {
      lcd.setCursor(4, 0);
      lcd.print(buf);
      lcd.setCursor(5, 0);  // l-r 0-15  t-b 0-1 
      lcd.print(str);       // EngineRPM  
    }
    else
    {
      lcd.setCursor(4, 0);
      lcd.print(buf);
      lcd.setCursor(5, 0);
      lcd.print(buf);
      lcd.setCursor(6, 0);  // l-r 0-15  t-b 0-1 
      lcd.print(str);       // EngineRPM  
    }
  }
  
  str = String(Gearn); 
  lcd.setCursor(9, 0);
  lcd.print(str);           // Gear  

//            0123456789012345
// topline = "RPM 6000 g 12000" 
  str = String(TransmissionRPM);
  lcd.setCursor(10, 0);
  lcd.print(buf);
  
  if (TransmissionRPM > 9999)
  {
    lcd.setCursor(11, 0);
    lcd.print(str);   // TransmissionRPM
    return;
  }
 
  if (TransmissionRPM > 999)
  {
    lcd.setCursor(11, 0);
    lcd.print(buf);
    lcd.setCursor(12, 0);
    lcd.print(str);   // TransmissionRPM 
    return;
  }

  if (TransmissionRPM > 99)
  {
    lcd.setCursor(11, 0);
    lcd.print(buf);
    lcd.setCursor(12, 0);
    lcd.print(buf);
    lcd.setCursor(13, 0);
    lcd.print(str);   // TransmissionRPM 
    return;
  }

  lcd.setCursor(11, 0);
  lcd.print(buf);
  lcd.setCursor(12, 0);
  lcd.print(buf);
  lcd.setCursor(13, 0);
  lcd.print(buf);
  lcd.setCursor(14, 0);
  lcd.print(str);   // TransmissionRPM 

// add noting here unless your remove the return and add all the else/if
}
void LCDbot()
{

  WhatWhat("LCDbot");

//               0123456789012345
// bottomline = " 3567 MPH 254.79" 

  str = String(AxleRPM);
  lcd.setCursor(0, 1);   // l-r 0-15  t-b 0-1
  lcd.print(" ");   
  
  if (AxleRPM > 999)
  {
    lcd.setCursor(1, 1);    // l-r 0-15  t-b 0-1 
    lcd.print(str);          // AxleRPM  
  }
  else
  {
    if (AxleRPM > 99)
    {
      lcd.setCursor(1, 1);
      lcd.print(buf); 
      lcd.setCursor(2, 1);    // l-r 0-15  t-b 0-1 
      lcd.print(str);         // AxleRPM  
    }
    else
    {
      lcd.setCursor(1, 1);
      lcd.print(buf); 
      lcd.setCursor(2, 1);
      lcd.print(buf); 
      lcd.setCursor(3, 1);    // l-r 0-15  t-b 0-1 
      lcd.print(str);         // AxleRPM  
    }
  }

//               0123456789012345
// bottomline = " 3567 MPH 254.79"
  str = String(MPH);
  lcd.setCursor(6, 1);
  lcd.print("MPH ");
  
  if (MPH > 99)
  {
    lcd.setCursor(10, 1);
    lcd.print(str);     // MPH
    return;
  }
  
  if (MPH > 99)
  {
    lcd.setCursor(10, 1);
    lcd.print(buf); 
    lcd.setCursor(11, 1);
    lcd.print(str);     // MPH
    return;
  }

  lcd.setCursor(10, 1);
  lcd.print(buf);
  lcd.setCursor(11, 1);
  lcd.print(buf);  
  lcd.setCursor(12, 1);
  lcd.print(str);     // MPH

// add noting here unless your remove the return and add all the else/if
}
void LCDlog()
{

  WhatWhat("LCDlog");

  if (TESTING)
  {
    LCDLogTest();
  }
  
  if (FEEDBACK)
  {
    LCDLogFeed();
  }
  
  if (DEBUG)
  {
    LCDLogDebug();
  }

}
void LCDLogTest()
{
    
  WhatWhat("LCDLogTest");

  switch (j)
  {
  case 0:
    strph = "1st gear 2.97:1 RPH "; 
    strpm = "1st gear 2.97:1 RPM "; 
    break;
  case 1:
    strph = "2nd gear 2.10:1 RPH "; 
    strpm = "2nd gear 2.10.1 RPM "; 
    break;
  case 2:
    strph = "3rd gear 1.46:1 RPH "; 
    strpm = "3rd gear 1.46.1 RPM "; 
    break;
  case 3:
    strph = "4th gear 1.00:1 RPH "; 
    strpm = "4th gear 1.00:1 RPM "; 
    break;
  case 4:
    strph = "5th gear 0.74:1 RPH "; 
    strpm = "5th gear 0.74:1 RPM "; 
    break;
  case 5:
    strph = "6th gear 0.50:1 RPH "; 
    strpm = "6th gear 0.50:1 RPM "; 
    break;
  }

}
void LCDLogFeed()
{

  WhatWhat("LCDLogFeed");

  str = String(EngineRPH);       NewOled.bmap = str; OLEDloop();
  str = String(EngineRPM);       NewOled.bmap = str; OLEDloop();
  str = String(TransmissionRPH); NewOled.bmap = str; OLEDloop();
  str = String(TransmissionRPM); NewOled.bmap = str; OLEDloop();
  str = String(AxleRPH);         NewOled.bmap = str; OLEDloop();
  str = String(AxleRPM);         NewOled.bmap = str; OLEDloop(); 
  str = String(MPH);             NewOled.bmap = str; OLEDloop();

}
void LCDLogDebug()
{

  WhatWhat("LCDLogDebug");

// per hour
  strph = "";
  str = String(EngineRPH);
  strph += str;
  str = String(TransmissionRPH);
  strph += " Tx ";
  strph += str;
  str = String(AxleRPH);
  strph += " Ax ";
  strph += str;
  str = String(MPH);
  strph += " MPH ";
  strph += str;  
  Serial.println(strph);

// per minute
  strpm = "";
  str = String(MPM);
  strpm += " MPM ";
  strpm += str;
  str = String(EngineRPM);
  strpm += str;
  str = String(TransmissionRPM);
  strpm += " Tx ";
  strpm += str;
    str = String(AxleRPM);  
  strpm += " Ax ";
  strpm += str;
  Serial.println(strpm);

}
void OLEDloop()
{

  display.clearDisplay();
  display.setCursor(0,0);
  
  OLEDpopup();
  
  for (li = 0; li < 8; li++)
  {
    display.println(BMAP[li].bmap); 
  }

  display.display();

}
void OLEDpopup()
{

  ri = 1;

  for (li = 0; li < 7; li++)
  {
    BMAP[li].bmap = BMAP[ri].bmap;
    ri++;
  }

  BMAP[7].bmap = NewOled.bmap;

}
void Matrixloop() 
{

  WhatWhat("Matrixloop");

  matrix.beginDraw();
  matrix.stroke(0xFFFFFFFF);

  if (TESTING){MatrixloopTestI();}
  else
  {
    MatrixloopRPM();
  }        

  matrix.textFont(Font_4x6);
  matrix.beginText(0, 1, 0xFFFFFF);
  matrix.println(Mtext);
  matrix.endText();
  matrix.endDraw();

  matrix.beginDraw();
  matrix.stroke(0xFFFFFFFF);
  matrix.textScrollSpeed(100);

  if (TESTING)
  {
    MatrixloopTestJ();
  }
  else
  {
    MatrixloopGear();           
  }

  matrix.textFont(Font_5x7);  
  matrix.beginText(0, 1, 0xFFFFFF);
  matrix.println(Mtext);
  matrix.endText(SCROLL_LEFT);
  matrix.endDraw();

}
void MatrixloopRPM()
{

  WhatWhat("MatrixloopRPM");

  Mtext[1] = '0';
  Mtext[2] = '0';

  if (EngineRPM >= 6000)
  {
    Mtext[0] = '6';
    return;
  }

  if (EngineRPM >= 5000)
  {
    Mtext[0] = '5';
    return;
  }

  if (EngineRPM >= 4000)
  {
    Mtext[0] = '4';
    return;
  }

  if (EngineRPM >= 3000)
  {
    Mtext[0] = '3';
    return;
  }

  if (EngineRPM >= 2000)
  {
    Mtext[0] = '2';
    return;
  }

  Mtext[0] = '1';

}
void MatrixloopGear()
{

  WhatWhat("MatrixloopGear");

  switch (Gearn)
  {
  case 1:
    Mtext[0] = '1';
    Mtext[1] = 's';
    Mtext[2] = 't';
    break;
  case 2:
    Mtext[0] = '2';
    Mtext[1] = 'n';
    Mtext[2] = 'd';
    break;
  case 3:
    Mtext[0] = '3';
    Mtext[1] = 'r';
    Mtext[2] = 'd';
    break;
  case 4:
    Mtext[0] = '4';
    Mtext[1] = 't';
    Mtext[2] = 'h';
    break;
  case 5:
    Mtext[0] = '5';
    Mtext[1] = 't';
    Mtext[2] = 'h';
    break;
  case 6:
    Mtext[0] = '6';
    Mtext[1] = 't';
    Mtext[2] = 'h';
    break;
  } 

}
void MatrixloopTestI()
{

  switch (i)
  {
  case 0:
    Mtext[0] = '1';
    Mtext[1] = '0';
    Mtext[2] = '0';
    break;
  case 1:
    Mtext[0] = '2';
    Mtext[1] = '0';
    Mtext[2] = '0';
    break;
  case 2:
    Mtext[0] = '3';
    Mtext[1] = '0';
    Mtext[2] = '0';
    break;
  case 3:
    Mtext[0] = '4';
    Mtext[1] = '0';
    Mtext[2] = '0';
    break;
  case 4:
    Mtext[0] = '5';
    Mtext[1] = '0';
    Mtext[2] = '0';
    break;
  case 5:
    Mtext[0] = '6';
    Mtext[1] = '0';
    Mtext[2] = '0';
    break;
  }

}
void MatrixloopTestJ()
{

  switch (j)
  {
  case 0:
    Mtext[0] = '1';
    Mtext[1] = 's';
    Mtext[2] = 't';
    break;
  case 1:
    Mtext[0] = '2';
    Mtext[1] = 'n';
    Mtext[2] = 'd';
    break;
  case 2:
    Mtext[0] = '3';
    Mtext[1] = 'r';
    Mtext[2] = 'd';
    break;
  case 3:
    Mtext[0] = '4';
    Mtext[1] = 't';
    Mtext[2] = 'h';
    break;
  case 4:
    Mtext[0] = '5';
    Mtext[1] = 't';
    Mtext[2] = 'h';
    break;
  case 5:
    Mtext[0] = '6';
    Mtext[1] = 't';
    Mtext[2] = 'h';
    break;
  }

}
void HELoopTestRPM()
{

  WhatWhat("HELoopTestRPM");

  switch (i)
  {
  case 0:
    EngineRPM = 1000;  
    TachPulseCnt = 4000;
    SetPulse();
    break;
  case 1:
    EngineRPM = 2000;  
    TachPulseCnt = 8000;
    SetPulse();
    break;
  case 2:
    EngineRPM = 3000;  
    TachPulseCnt = 12000;
    SetPulse();
    break;
  case 3:
    EngineRPM = 4000;  
    TachPulseCnt = 16000;
    SetPulse();
    break;
  case 4:
    EngineRPM = 5000;  
    TachPulseCnt = 20000;
    SetPulse();
    break;
  case 5:
    EngineRPM = 6000;  
    TachPulseCnt = 24000;
    SetPulse();
    break;
  }

}
void HELoopTestGear()
{

  WhatWhat("HELoopTestGear");

  switch (j)
  {
  case 0:
    Gear = FirstGear;
    Gearn = 1;
    break;
  case 1:
    Gear = SecondGear;
    Gearn = 2;
    break;
  case 2:
    Gear = ThirdGear;
    Gearn = 3;
    break;
  case 3:
    Gear = FourthGear;
    Gearn = 4;
    break;
  case 4:
    Gear = FifthGear;
    Gearn = 5;
    break;
  case 5:
    Gear = SixthGear;
    Gearn = 6;
    break;
  }

}
void HELoopErpm()
{

  WhatWhat("HELoopErpm");

  if (EngineRPM == (TransmissionRPM / FirstGear))
  {
    Gear = FirstGear;
    Gearn = 1;
    return;
  }

  if (EngineRPM == (TransmissionRPM / SecondGear))
  {
    Gear = SecondGear;
    Gearn = 2;
    return;
  }

  if (EngineRPM == (TransmissionRPM / ThirdGear))
  {
    Gear = ThirdGear;
    Gearn = 3;
    return;
  }

  if (EngineRPM == (TransmissionRPM / FourthGear))
  {
    Gear = FourthGear;
    Gearn = 4;
    return;
  }

  if (EngineRPM == (TransmissionRPM / FifthGear))
  {
    Gear = FifthGear;
    Gearn = 5;
    return;
  }

  Gear = SixthGear;
  Gearn = 6;   

// add noting here unless your remove the return and add all the else/if
}
void SetPulse()
{
  
  WhatWhat("SetPulse");

  if (TESTING)
  {
    switch (j)
    {
    case 0:
      DrivePulseCnt = EngineRPM / FirstGear; // actually drive rpm
      break;
    case 1:
      DrivePulseCnt = EngineRPM / SecondGear; // actually drive rpm
      break;
    case 2:
      DrivePulseCnt = EngineRPM / ThirdGear; // actually drive rpm
      break;
    case 3:
      DrivePulseCnt = EngineRPM / FourthGear; // actually drive rpm
      break;
    case 4:
      DrivePulseCnt = EngineRPM / FifthGear; // actually drive rpm
      break;
    case 5:
      DrivePulseCnt = EngineRPM / SixthGear; // actually drive rpm
      break;
    }
  }

  AxlePulseCnt = DrivePulseCnt / Differential;    // actually axle rpm 
  
  DrivePulseCnt *= DrivePPR;  
  
  AxlePulseCnt *= AxlePPR;

}
void WhatWhat(arduino::String buf)
{

  if (FEEDBACK)
  {
    NewOled.bmap = buf;
    OLEDloop();
  }
  
  if (DEBUG)
  {
    Serial.println(buf);
  }

}