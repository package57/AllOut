// Techometer & Speedometer Combined All Out
// using three inputs
// hall effect on axle for axel rpm
// tachometer signal for engine rpm
// VSS from transmission for driveshaft rpm
// mph is calculated using axle rpm and tire dimensions
//  
//         Engine RPM - Axle RPM
//     Driveshaft RPM - MPH
// 
#include "AllOut.h"
void setup() 
{

  Serial.println("setup");

  Serial.begin(BAUD);

  OLEDsetup();

  NewOled.bmap = "OLED setup";
  OLEDloop();

  LCDInit();

  LCDsetup();

  Matrixsetup();

  HESetup();

}
void loop()
{
  
  Serial.println("loop");

  NewOled.bmap = "loop";
  OLEDloop(); 
  
  for (i = 0; i < 6; i++)
  {
    for (j = 0; j < 6; j++)
    {
      HELoop();
      Matrixloop();
      LCDloop();
      delay(1000);
    }
  }

}
void LCDInit()
{

  Serial.println("LCDInit");

  NewOled.bmap = "LCD Init";
  OLEDloop();

  buf = " ";

  EngineRPM = 6000;
  EngineRPH = 360000;
  TransmissionRPH = 2020.202;  // 1st
//TransmissionRPH = 2857.142;  // 2nd 
//TransmissionRPH = 4109.589;  // 3rd
//TransmissionRPH = 6000.000;  // 4th
//TransmissionRPH = 8108.108;  // 5th
//TransmissionRPH = 12000.000;  // 6th

  AxleRPH = 600.535;  // 1st
//AxleRPH = 849.328;  // 2nd 
//AxleRPH = 1221.640;  // 3rd
//AxleRPH = 1783.590;  // 4th
//AxleRPH = 2410.258;  // 5th
//AxleRPH = 3567.11;  // 6th

  TireDiameter = 24.9;
  Differential = 3.364; 
  Gear = 1.000;
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

  TransmissionRPH = EngineRPH / FirstGear;
  AxleRPH = TransmissionRPH / Differential;
  MPH = AxleRPH / RevPerMile;
//MPH = ((EngineRPM / FirstGear)  / Differential) / RevPerMile)) * 60;   //  42.895405
//MPH = ((EngineRPM / SecondGear) / Differential) / RevPerMile)) * 60;   //  60.666359
//MPH = ((EngineRPM / ThirdGear)  / Differential) / RevPerMile)) * 60;   //  87.259831
//MPH = ((EngineRPM / FourthGear) / Differential) / RevPerMile)) * 60;   // 127.399354
//MPH = ((EngineRPM / FifthGear)  / Differential) / RevPerMile)) * 60;   // 172.161289
//MPH = ((EngineRPM / SixthGear)  / Differential) / RevPerMile)) * 60;   // 254.798709

}
void LCDsetup() 
{

  Serial.println("LCDsetup"); 

  NewOled.bmap = "LCD setup";
  OLEDloop();

  lcd.init();
  lcd.backlight();
  topline = "RPM 6000 1 12000";    // Engine RPM 6000 / Gear / Driveshaft RPM
  botline = "A3567 MPH 254.79"; // Axle RPM / MPH 
  lcd.setCursor(0, 0);  // l-r 0-15  t-b 0-1
  lcd.print(topline);
  lcd.setCursor(0, 1);
  lcd.print(botline);

}
void Matrixsetup()
{
 
  Serial.println("Matrixsetup");

  NewOled.bmap = "Matrix setup";
  OLEDloop();
  
  matrix.begin();
  matrix.beginDraw();
  matrix.stroke(0xFFFFFFFF);
  Mtext[0] = 'A';
  Mtext[1] = 'L';
  Mtext[2] = 'L';
  matrix.textFont(Font_4x6);
  matrix.beginText(0, 1, 0xFFFFFF);
  matrix.println(Mtext);
  matrix.endText();
  matrix.endDraw();

  delay(2000);

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
void HESetup()
{

  Serial.println("HEsetup");

  NewOled.bmap = "Hall Effect setup";
  OLEDloop();

  HEInit();

  // modules
  pinMode(TachPin, INPUT);    // tachometer
  pinMode(DrivePin, INPUT);    // driveshaft     
  pinMode(AxlesPin, INPUT);    // axle

}
void OLEDsetup()
{
  
  Serial.println("OLEDsetup");

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

  Serial.println("LCDloop");

  NewOled.bmap = "LCD loop";
  OLEDloop();  
  
  LCDcalc();
 
  LCDtop();

  LCDbot();

  LCDlog();

}
void LCDcalc()
{

  Serial.println("LCDcalc");

  NewOled.bmap = "LCDcalc";
  OLEDloop();

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

  Serial.println("LCDtop");

  NewOled.bmap = "LCDtop";
  OLEDloop();

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
  }
  else
  {
    if (TransmissionRPM > 999)
    {
    lcd.setCursor(11, 0);
    lcd.print(buf);
    lcd.setCursor(12, 0);
    lcd.print(str);   // TransmissionRPM 
    }
    else
    {
      if (TransmissionRPM > 99)
      {
        lcd.setCursor(11, 0);
        lcd.print(buf);
        lcd.setCursor(12, 0);
        lcd.print(buf);
        lcd.setCursor(13, 0);
        lcd.print(str);   // TransmissionRPM 
      }
      else
      {
        lcd.setCursor(11, 0);
        lcd.print(buf);
        lcd.setCursor(12, 0);
        lcd.print(buf);
        lcd.setCursor(13, 0);
        lcd.print(buf);
        lcd.setCursor(14, 0);
        lcd.print(str);   // TransmissionRPM 
      }
    }
  }

}
void LCDbot()
{

  Serial.println("LCDbot");

  NewOled.bmap = "LCDbot ";
  OLEDloop();

//               0123456789012345
// bottomline = "A3567 MPH 254.79" 

  str = String(AxleRPM);
  lcd.setCursor(0, 1);   // l-r 0-15  t-b 0-1
  lcd.print("A");   
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
// bottomline = "A3567 MPH 254.79" 

  str = String(MPH);
  lcd.setCursor(6, 1);
  lcd.print("MPH ");
  if (MPH > 99)
  {
    lcd.setCursor(10, 1);
    lcd.print(str);     // MPH
  }
  else
  {
    if (MPH > 99)
    {
    lcd.setCursor(10, 1);
    lcd.print(buf); 
    lcd.setCursor(11, 1);
    lcd.print(str);     // MPH
    }
    else
    {
      lcd.setCursor(10, 1);
      lcd.print(buf);
      lcd.setCursor(11, 1);
      lcd.print(buf);  
      lcd.setCursor(12, 1);
      lcd.print(str);     // MPH
    }
  }

}
void LCDlog()
{

  Serial.println("LCDlog");

  NewOled.bmap = "LCDlog ";
  OLEDloop();

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

  str = String(EngineRPH);
  strph += str;
  NewOled.bmap = str;
  OLEDloop();

  str = String(EngineRPM);
  strpm += str;
  

  str = String(TransmissionRPH);
  strph += " Tx ";
  strph += str;

  NewOled.bmap = str;
  OLEDloop();

  str = String(TransmissionRPM);
  strpm += " Tx ";
  strpm += str;


  str = String(AxleRPH);
  strph += " Ax ";
  strph += str;

  NewOled.bmap = str;
  OLEDloop();

  str = String(AxleRPM);
  strpm += " Ax ";
  strpm += str;
  

  str = String(MPH);
  strph += " MPH ";
  strph += str;

  NewOled.bmap = str;
  OLEDloop();

  str = String(MPM);
  strpm += " MPM ";
  strpm += str;

  Serial.println(strph);
  Serial.println(strpm);

}
void OLEDloop()
{

  Serial.println("OLEDloop");

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

  Serial.println("OLEDpopup");

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

  Serial.println("Matrixloop");

  NewOled.bmap = "Matrixloop ";
  OLEDloop();

  matrix.beginDraw();
  matrix.stroke(0xFFFFFFFF);

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

  matrix.textFont(Font_4x6);
  matrix.beginText(0, 1, 0xFFFFFF);
  matrix.println(Mtext);
  matrix.endText();
  matrix.endDraw();

  delay(2000);

  matrix.beginDraw();
  matrix.stroke(0xFFFFFFFF);
  matrix.textScrollSpeed(100);

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

  matrix.textFont(Font_5x7);
  matrix.beginText(0, 1, 0xFFFFFF);
  matrix.println(Mtext);
  matrix.endText(SCROLL_LEFT);
  matrix.endDraw();

}
void HELoop() 
{

  Serial.println("HEloop");

  NewOled.bmap = "Hall Effect loop ";
  OLEDloop();

  HETach();

  HEDrive();

  HEAxle();

  strHE = "Hall Effect ";
  
  str = String(TachRPM);  
  strHE += " Tachometer ";
  strHE += str;
  
  str = String(DriveRPM);
  strHE += " Driveshaft ";
  strHE += str;
  
  str = String(AxlesRPM);
  strHE += " Axel ";
  strHE += str;

  Serial.println(strHE);

  switch (i)
  {
    case 0:
      EngineRPM = 1000;
      break;
    case 1:
      EngineRPM = 2000;
      break;
    case 2:
      EngineRPM = 3000;
      break;
    case 3:
      EngineRPM = 4000;
      break;
    case 4:
      EngineRPM = 5000;
      break;
    case 5:
      EngineRPM = 6000;
      break;
  }


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
void HETach()
{
// The frequency of the pulses corresponds to the engine's RPM. For an 8-cylinder engine, 
// there are four pulses per crankshaft revolution (since the distributor fires once every two revolutions of the crankshaft).

  Serial.println("HETach");

  NewOled.bmap = "HETach ";
  OLEDloop();

  TachStart = micros();
  TachOldValue = 1;
  TachCnt = 0;

  while (TachCnt < maxCnt)
  {
    TachNewValue = 0; // temp digitalRead(TachPin);
    if (!TachNewValue 
    &&  TachNewValue != TachOldValue)
    { 
      TachCnt++;
    }
    // temp TachOldValue = TachNewValue;
  }

  TachEnd = micros();
  TachSeconds = TachEnd - TachStart;
  TachSeconds /= 1000000.0;

  TachRPM = TachCnt / TachSeconds;
  TachRPM *= 60.0;

}
void HEDrive()
{
// The number of pulses per driveshaft revolution can vary depending on the specific T56 model and application.
// Common setups include 8, 12, or 16 PPR.
  Serial.println("HEDrive");

  NewOled.bmap = "HEDrive ";
  OLEDloop();

  DriveStart = micros();
  DriveOldValue = 1;
  DriveCnt = 0;

  while (DriveCnt < maxCnt)
  {
    DriveNewValue = 0; // temp digitalRead(DrivePin);
    if (!DriveNewValue 
    &&   DriveNewValue != DriveOldValue)
    { 
      DriveCnt++;
    }
    // temp DriveOldValue = DriveNewValue;
  }

  DriveEnd = micros();
  DriveSeconds = DriveEnd - DriveStart;
  DriveSeconds /= 1000000.0;

  DriveRPM = DriveCnt / DriveSeconds;
  DriveRPM *= 60.0;

}
void HEAxle()
{
 
  Serial.println("HEAxle");

  NewOled.bmap = "HEAxle ";
  OLEDloop();

  AxlesStart = micros();
  AxlesOldValue = 1;
  AxlesCnt = 0;

  while (AxlesCnt < maxCnt)
  {
    AxlesNewValue = 0; // temp digitalRead(AxlePin);
    if (!AxlesNewValue 
    &&  AxlesNewValue != AxlesOldValue)
    { 
      AxlesCnt++;
    }
    // temp AxleOldValue = AxleNewValue;
  }

  AxlesEnd = micros();
  AxlesSeconds = AxlesEnd - AxlesStart;
  AxlesSeconds /= 1000000.0;

  AxlesRPM = AxlesCnt / AxlesSeconds;
  AxlesRPM *= 60.0;

}
void HEInit()
{

  Serial.println("HEInit");

  NewOled.bmap = "HEInit ";
  OLEDloop();

// Tachometer
  TachStart = 0;
  TachEnd = 0;
  TachSeconds = 0.0;
  TachRPM = 0.0;
  TachCnt = 0;
  TachNewValue = 0;
  TachOldValue = 0;

// Driveshaft
  DriveStart = 0;
  DriveEnd = 0;
  DriveSeconds = 0.0;
  DriveRPM = 0.0;
  DriveCnt = 0;
  DriveNewValue = 0;
  DriveOldValue = 0;

// Axel
  AxlesStart = 0;
  AxlesEnd = 0;
  AxlesSeconds = 0.0;
  AxlesRPM = 0.0;
  AxlesCnt = 0;
  AxlesNewValue = 0;
  AxlesOldValue = 0;

}
