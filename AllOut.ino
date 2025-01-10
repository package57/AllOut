// Techometer & Speedometer Combined All Out
// using three inputs
// tachometer signal for engine rpm from the HEI distributor
// VSS for driveshaft rpm from transmission
// hall effect on axle for axel rpm
// MPH is calculated using axle rpm and tire dimensions
// since even with the engine off and the transmission in neutral, you can still be coasting
// only with live testing will I know if the VSS is senging a pulse, and what the actual pulse is
// the book says it's either 8, 12, or 16 PPR.     
//  
// will display 
//         Engine RPM - Axle RPM
//     Driveshaft RPM - MPH
// 
// to test this I added a intermittent button and three LED's
// in the Tach ISR's I turn the Green LED on at the start and off at the end   
// in the Drive ISR's I turn the Yellow LED on at the start and off at the end  
// in the Axle ISR's I turn the Blue LED on at the start and off at the end
// with a quick press of the button, I send 3v signal to all three inturrupt pins  
// then I set the pulse count to what it "should be" since there is only one inturrupt signal where there would be many
// using the Distributor as the driver, everything follows after that 
// in the end, the real MPH is taken off the VSS and or the Axle     
// I have two different RPM calculations, one is basic, the other is an Average
// this gives me the need for 3x2 ISR, so I trigger the Average 'ISR' from the Basic

#include "AllOut.h"
void setup() 
{

    Serial.begin(BAUD);

    pinMode(LEDPinG, OUTPUT);
    pinMode(LEDPinY, OUTPUT);   
    pinMode(LEDPinB, OUTPUT);

    digitalWrite(LEDPinG, HIGH);
    digitalWrite(LEDPinY, HIGH);
    digitalWrite(LEDPinB, HIGH);
    delay(4000);
    digitalWrite(LEDPinG, LOW);
    digitalWrite(LEDPinY, LOW);
    digitalWrite(LEDPinB, LOW);

    Serial.println("setup");

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
 
    if (TESTING)
    {
        for (i = 0; i < 6; i++)
        {
            for (j = 0; j < 6; j++)
            {
                HELoop();
                Matrixloop();
                LCDloop();
            }
        }
        delay(4000);
    }
    else
    {
        HELoop();
        Matrixloop();
        LCDloop();
    }

  str = "";
  str = "Tach.PulseCnt ";
  str += String(Tach.PulseCnt);
  Serial.println(str);

  str = "";
  str = "ATach.PulseCnt ";
  str += String(ATach.PulseCnt);
  Serial.println(str);

  str = "";
  str = "Drive.PulseCnt ";
  str += String(Drive.PulseCnt);
  Serial.println(str);

  str = "";
  str = "ADrive.PulseCnt ";
  str += String(ADrive.PulseCnt);
  Serial.println(str);

  str = "";
  str = "Axle.PulseCnt ";
  str += String(Axle.PulseCnt);
  Serial.println(str);

  str = "";
  str = "AAxle.PulseCnt ";
  str += String(AAxle.PulseCnt);
  Serial.println(str);
  
}
void LCDInit()
{

    NewOled.bmap = "LCD Init";
    OLEDloop();

    buf = " ";

    if (TESTING)
    {
        EngineRPM = 6000;
        EngineRPH = 360000;
        TransmissionRPH = 2020.202;  // 1st
//      TransmissionRPH = 2857.142;  // 2nd 
//      TransmissionRPH = 4109.589;  // 3rd
//      TransmissionRPH = 6000.000;  // 4th
//      TransmissionRPH = 8108.108;  // 5th
//      TransmissionRPH = 12000.000;  // 6th
        AxleRPH = 600.535;  // 1st
//      AxleRPH = 849.328;  // 2nd 
//      AxleRPH = 1221.640;  // 3rd
//      AxleRPH = 1783.590;  // 4th
//      AxleRPH = 2410.258;  // 5th
//      AxleRPH = 3567.11;  // 6th
        Gear = 1.000;
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
        TransmissionRPH = EngineRPH / FirstGear;
        AxleRPH = TransmissionRPH / Differential;
        MPH = AxleRPH / RevPerMile;
//      MPH = ((EngineRPM / FirstGear)  / Differential) / RevPerMile)) * 60;   //  42.895405
//      MPH = ((EngineRPM / SecondGear) / Differential) / RevPerMile)) * 60;   //  60.666359
//      MPH = ((EngineRPM / ThirdGear)  / Differential) / RevPerMile)) * 60;   //  87.259831
//      MPH = ((EngineRPM / FourthGear) / Differential) / RevPerMile)) * 60;   // 127.399354
//      MPH = ((EngineRPM / FifthGear)  / Differential) / RevPerMile)) * 60;   // 172.161289
//      MPH = ((EngineRPM / SixthGear)  / Differential) / RevPerMile)) * 60;   // 254.798709
    }

}
void LCDsetup() 
{

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
 
  NewOled.bmap = "Matrix setup";
  OLEDloop();
  
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

    NewOled.bmap = "Hall Effect setup";
    OLEDloop();

    HEInit();

// interrupt service routines
    pinMode(TachPin, OUTPUT);    // HEI Tachometer signal
    attachInterrupt(TachPin, HETachISR, CHANGE);
    
    pinMode(DrivePin, INPUT);   // Transmission VSS signal 
    attachInterrupt(DrivePin, HEDriveISR, RISING);    

    pinMode(AxlePin, INPUT);  // Axle hall effect sensor
    attachInterrupt(AxlePin, HEAxleISR, HIGH);  

}
void HEInit()
{

    NewOled.bmap = "HEInit ";
    OLEDloop();

    GearRatio = 0.0;

// base 
    Root.DiffMillis = 0;
    Root.CurrMillis = 0;
    Root.PrevMillis = 0; 
    Root.DiffMicros = 0;
    Root.CurrMicros = 0;
    Root.PrevMicros = 0;
    Root.Duration = 0; 
    Root._Duration = 0;
    Root.PulseCnt = 0;
    Root._PulseCnt = 0;
    Root.Freq = 0.0;
    Root.RPM = 0.0;

// Tachometer
    Tach = Root;
// Driveshaft
    Drive = Root;
// Axel
    Axle = Root;

// average
    ARoot.numReadings = 2;
    ARoot.readIndex = 0;
    ARoot.DiffMillis = 0;
    ARoot.ZeroDiff = 0;
    ARoot.DiffMicros = 0;   
    ARoot.ZeroTimeout = 300000;  
    ARoot.LastTimeWeMeasured = 0;   
    ARoot.PeriodBetweenPulses = ARoot.ZeroTimeout + 1000; 
    ARoot.PeriodAverage = ARoot.ZeroTimeout + 1000; 
    ARoot.FrequencyRaw = 0; 
    ARoot.FrequencyReal = 0;
    ARoot.RPM = 0;
    ARoot.PulseCnt = 1;
    ARoot.PeriodSum = 0;
    ARoot.LastTimeCycleMeasure = 0; 
    ARoot.CurrentMicros = micros(); 
    ARoot.AmountOfReadings = 1;
    ARoot.ZeroDebouncingExtra = 0;
    ARoot.Readings[0] = 0;  
    ARoot.Readings[1] = 0;
    ARoot.Total = 0;
    ARoot.Average = 0;
    ARoot.RemapedAmountOfReadings;

// Tachometer
    ATach = ARoot;
// Driveshaft
    ADrive = ARoot;
// Axel
    AAxle = ARoot;

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

    NewOled.bmap = "LCD loop";
    OLEDloop();  
  
    LCDcalc();
 
    LCDtop();

    LCDbot();

    LCDlog();

}
void LCDcalc()
{

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

    NewOled.bmap = "LCDlog ";
    OLEDloop();

    if (TESTING)
    {
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

    if (TESTING)
    {
        str = String(MPM);
        strpm += " MPM ";
        strpm += str;
        Serial.println(strph);
        Serial.println(strpm);
    }

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

    NewOled.bmap = "Matrixloop ";
    OLEDloop();

    matrix.beginDraw();
    matrix.stroke(0xFFFFFFFF);

    if (TESTING)
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
    else
    {
        if (EngineRPM >= 6000)
        {
            Mtext[0] = '6';
            Mtext[1] = '0';
            Mtext[2] = '0';
        }
        else
        {
            if (EngineRPM >= 5000)
            {
                Mtext[0] = '5';
                Mtext[1] = '0';
                Mtext[2] = '0';
            }
            else
            {
                if (EngineRPM >= 4000)
                {
                    Mtext[0] = '4';
                    Mtext[1] = '0';
                    Mtext[2] = '0';
                }
                else
                {
                    if (EngineRPM >= 3000)
                    {
                        Mtext[0] = '3';
                        Mtext[1] = '0';
                        Mtext[2] = '0';
                    }
                    else
                    {
                        if (EngineRPM >= 2000)
                        {
                            Mtext[0] = '2';
                            Mtext[1] = '0';
                            Mtext[2] = '0';
                        }
                        else
                        {
                            Mtext[0] = '1';
                            Mtext[1] = '0';
                            Mtext[2] = '0';
                        }
                    }
                }
            }
        }
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

    if (TESTING)
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
    else
    {
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

    matrix.textFont(Font_5x7);  
    matrix.beginText(0, 1, 0xFFFFFF);
    matrix.println(Mtext);
    matrix.endText(SCROLL_LEFT);
    matrix.endDraw();

}
void HELoop() 
{

  NewOled.bmap = "Hall Effect loop ";
  OLEDloop();

// Basic calculation
// Tachometer
  HETach();
//HETachISR();
// Average calculation
  HEATach();
//HEATachISR();

// Basic calculation
// Transmission
  //  HEDrive();
//HEDriveISR();
// Average calculation
  HEADrive();
//HEADriveISR();

// Basic calculation
// Axle
  HEAxle();
//HEAxleISR();
// Average calculation
  // HEAAxle();
//HEAAxleISR();

  strHE = "  Basic Hall Effect ";  
  str = String(Tach.RPM);  
  strHE += " Tach ";
  strHE += str;  
  str = String(Drive.RPM);
  strHE += " Drive ";
  strHE += str;
  str = String(Gear);
  strHE += " Gear ";
  strHE += str;  
  str = String(Axle.RPM);
  strHE += " Axle ";
  strHE += str;
  Serial.println(strHE);

  strHE = "Average Hall Effect ";  
  str = String(ATach.RPM);  
  strHE += " Tach ";
  strHE += str;  
  str = String(ADrive.RPM);
  strHE += " Drive ";
  strHE += str;
  str = String(Gear);
  strHE += " Gear ";
  strHE += str;  
  str = String(AAxle.RPM);
  strHE += " Axle ";
  strHE += str;
  Serial.println(strHE);

  if (TESTING)
  {
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

// create fist second third forth fifth sixth ()
// calculate DrivePulseCnt by RPM
// calculate AxlePulseCnt by DriveRPM 

  if (TESTING)
  {
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
  else
  {
 
    }

}
void HELoopErpm()
{

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

}
void SetPulse()
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

  AxlePulseCnt = DrivePulseCnt / Differential;    // actually axle rpm 
  
  DrivePulseCnt *= DrivePPR;  
  
  AxlePulseCnt *= AxlePPR;

}
void HETach()
{
// The frequency of the pulses corresponds to the engine's RPM. For an 8-cylinder engine, 
// there are 4 pulses per crankshaft revolution 
//  500 RPM x 4 ppr =   2000 pulses per minute / 60 = 33.33 pulses per second
// 6000 RPM x 4 ppr = 24,000 pulses per minute / 60 = 400 pulses per second
// (x pulses per second * 60) / 4 = RPM
  Serial.println("HETach");
  NewOled.bmap = "HETach ";
  OLEDloop();

  Tach.CurrMillis = millis();
 
  Tach.DiffMillis = Tach.CurrMillis - Tach.PrevMillis;
  if (Tach.DiffMillis < MainPeriod)
  {
    return;
  }

  Tach.PrevMillis = Tach.CurrMillis;   
  Tach._Duration = Tach.Duration;
  Tach._PulseCnt = Tach.PulseCnt;
  
  Tach.RPM = Tach.PulseCnt * 60;
  Tach.RPM /= TachPPR;   
  
  Tach.Duration = 0; 
  Tach.PulseCnt = 0;
  
  Tach.Freq = 1e6 / float(Tach._Duration);   
  Tach.Freq *= Tach._PulseCnt;

  Serial.print("Tachometer RPM: ");
  Serial.print(Tach.RPM);
  Serial.print(" Frequency: ");
  Serial.print(Tach.Freq);
  Serial.println(" Hz");

}
void HETachISR()
{

    digitalWrite(LEDPinG, HIGH);

    Tach.CurrMicros = micros();
  
    Tach.DiffMicros = Tach.CurrMicros - Tach.PrevMicros;

    Tach.Duration += Tach.DiffMicros;   
  
    Tach.PrevMicros = Tach.CurrMicros;
  
    if (TESTING)
    {
        Tach.PulseCnt += TachPulseCnt;
    }
    else
    {
        Tach.PulseCnt++;
    }
  
    ATach.PeriodBetweenPulses = micros() - ATach.LastTimeWeMeasured; 
    ATach.LastTimeWeMeasured = micros();
  
    if (ATach.PulseCnt >= ATach.AmountOfReadings)
    {
        ATach.PeriodAverage = ATach.PeriodSum / ATach.AmountOfReadings;
        ATach.PulseCnt = 1; 
        ATach.PeriodSum = ATach.PeriodBetweenPulses;
        ATach.RemapedAmountOfReadings = map(ATach.PeriodBetweenPulses, 40000, 5000, 1, 10);
        ATach.RemapedAmountOfReadings = constrain(ATach.RemapedAmountOfReadings, 1, 10);
        ATach.AmountOfReadings = ATach.RemapedAmountOfReadings;    
    }
    else
    {
        ATach.PeriodSum = ATach.PeriodSum + ATach.PeriodBetweenPulses;
        if (TESTING)
        {
            ATach.PulseCnt  += TachPulseCnt;
        }
        else
        {
            ATach.PulseCnt++;
        }
    }  

    digitalWrite(LEDPinG, LOW);

}
void HEATach()
{
  
Serial.println("HEATach");
  NewOled.bmap = "HEATach ";
  OLEDloop();

  ATach.LastTimeCycleMeasure = ATach.LastTimeWeMeasured; 
  ATach.CurrentMicros = micros(); 
  
  if (ATach.CurrentMicros < ATach.LastTimeCycleMeasure)
  {
    ATach.LastTimeCycleMeasure = ATach.CurrentMicros;
  }
  
  ATach.FrequencyRaw = TenB / ATach.PeriodAverage; 
  ATach.ZeroDiff = ATach.ZeroTimeout - ATach.ZeroDebouncingExtra;  
  
  ATach.DiffMicros = ATach.CurrentMicros - ATach.LastTimeCycleMeasure;       

  if(ATach.PeriodBetweenPulses > ATach.ZeroDiff 
  || ATach.DiffMicros > ATach.ZeroDiff)
  {  
    ATach.FrequencyRaw = 0;
    ATach.ZeroDebouncingExtra = 2000;
  }
  else
  {
    ATach.ZeroDebouncingExtra = 0;
  }
  
  ATach.FrequencyReal = ATach.FrequencyRaw / 10000;
  ATach.RPM = ATach.FrequencyRaw / TachPPR * 60;
  ATach.RPM = ATach.RPM / 10000; 
  ATach.Total = ATach.Total - ATach.Readings[ATach.readIndex];
  ATach.Readings[ATach.readIndex] = ATach.RPM;
  ATach.Total = ATach.Total + ATach.Readings[ATach.readIndex];
  ATach.readIndex = ATach.readIndex + 1;
  
  if (ATach.readIndex >= ATach.numReadings)
  {
    ATach.readIndex = 0;
  }
 
  ATach.Average = ATach.Total / ATach.numReadings;

}
void HEDrive()
{
// The number of pulses per driveshaft revolution can vary depending on the specific T56 model and application.
// Common setups include 8, 12, or 16 PPR.
// 4th gear 1:1
//  500 RPM x 8 ppr =   4000 pulses per minute / 60 = 66.66 pulses per second
// 6000 RPM x 8 ppr = 48,000 pulses per minute / 60 = 800 pulses per second
// (y pulses per second * 60) / 8 = RPM
  Serial.println("HEDrive");
  NewOled.bmap = "HEDrive ";
  OLEDloop();

  Drive.CurrMillis = millis();
 
  Drive.DiffMillis = Drive.CurrMillis - Drive.PrevMillis;
  if (Drive.DiffMillis < MainPeriod)
  {
    return;
  }

  Drive.PrevMillis = Drive.CurrMillis;   
  Drive._Duration = Drive.Duration;
  Drive._PulseCnt = Drive.PulseCnt;
  
  Drive.RPM = Drive.PulseCnt * 60;
  Drive.RPM /= DrivePPR; 
  
  
  Drive.Duration = 0; 
  Drive.PulseCnt = 0;
  
  Drive.Freq = 1e6 / float(Drive._Duration);   
  Drive.Freq *= Drive._PulseCnt;
  
  Serial.print("Transmission RPM: ");
  Serial.print(Drive.RPM);
  Serial.print(" Frequency: ");
  Serial.print(Drive.Freq);
  Serial.println(" Hz"); 

}
void HEDriveISR()
{
  
    digitalWrite(LEDPinY, HIGH);

    Drive.CurrMicros = micros();
    Drive.DiffMicros = Drive.CurrMicros - Drive.PrevMicros;
    Drive.Duration += Drive.DiffMicros;
    Drive.PrevMicros = Drive.CurrMicros;
  
    if (TESTING)
    {
        Drive.PulseCnt += DrivePulseCnt;
    }
    else
    {
        Drive.PulseCnt++;
    }

    ADrive.PeriodBetweenPulses = micros() - ADrive.LastTimeWeMeasured; 
    ADrive.LastTimeWeMeasured = micros();
  
    if(ADrive.PulseCnt >= ADrive.AmountOfReadings)
    {
        ADrive.PeriodAverage = ADrive.PeriodSum / ADrive.AmountOfReadings;
        ADrive.PulseCnt = 1; 
        ADrive.PeriodSum = ADrive.PeriodBetweenPulses;
        ADrive.RemapedAmountOfReadings = map(ADrive.PeriodBetweenPulses, 40000, 5000, 1, 10);
        ADrive.RemapedAmountOfReadings = constrain(ADrive.RemapedAmountOfReadings, 1, 10);
        ADrive.AmountOfReadings = ADrive.RemapedAmountOfReadings;
    }
    else
    {
        ADrive.PeriodSum = ADrive.PeriodSum + ADrive.PeriodBetweenPulses;
        if (TESTING)
        {
            ADrive.PulseCnt += DrivePulseCnt;
        }
        else
        {
            ADrive.PulseCnt++;
        }
    }

    digitalWrite(LEDPinY, LOW);

}
void HEADrive()
{

  Serial.println("HEADrive");
  NewOled.bmap = "HEADrive ";
  OLEDloop();

  ADrive.LastTimeCycleMeasure = ADrive.LastTimeWeMeasured; 
  ADrive.CurrentMicros = micros(); 
  
  if (ADrive.CurrentMicros < ADrive.LastTimeCycleMeasure)
  {
    ADrive.LastTimeCycleMeasure = ADrive.CurrentMicros;
  }
  
  ADrive.FrequencyRaw = TenB / ADrive.PeriodAverage; 
  ADrive.ZeroDiff = ADrive.ZeroTimeout - ADrive.ZeroDebouncingExtra;  
  
  ADrive.DiffMicros = ADrive.CurrentMicros - ADrive.LastTimeCycleMeasure;       
  if(ADrive.PeriodBetweenPulses > ADrive.ZeroDiff 
  || ADrive.DiffMicros > ADrive.ZeroDiff)
  {  
    ADrive.FrequencyRaw = 0;
    ADrive.ZeroDebouncingExtra = 2000;
  }
  else
  {
    ADrive.ZeroDebouncingExtra = 0;
  }
  
  ADrive.FrequencyReal = ADrive.FrequencyRaw / 10000;
  ADrive.RPM = ADrive.FrequencyRaw / DrivePPR * 60;
  ADrive.RPM = ADrive.RPM / 10000; 
  ADrive.Total = ADrive.Total - ADrive.Readings[ADrive.readIndex];
  ADrive.Readings[ADrive.readIndex] = ADrive.RPM;
  ADrive.Total = ADrive.Total + ADrive.Readings[ADrive.readIndex];
  ADrive.readIndex = ADrive.readIndex + 1;
  
  if (ADrive.readIndex >= ADrive.numReadings)
  {
    ADrive.readIndex = 0;
  }
 
  ADrive.Average = ADrive.Total / ADrive.numReadings;

}
void HEAxle()
{
// The number of pulses per axle revolution is 1
//  500 RPM x 1 ppr =   500 pulses per minute / 60 = 8.33 pulses per second
// 6000 RPM x 1 ppr = 6,000 pulses per minute / 60 = 100 pulses per second
// (z pulses per second * 60) / 8 = RPM
  Serial.println("HEAxle");
  NewOled.bmap = "HEAxle ";
  OLEDloop();

  Axle.CurrMillis = millis();
 
  Axle.DiffMillis = Axle.CurrMillis - Axle.PrevMillis;
  if (Axle.DiffMillis < MainPeriod)
  {
    return;
  }

  Axle.PrevMillis = Axle.CurrMillis;   
  Axle._Duration = Axle.Duration;
  Axle._PulseCnt = Axle.PulseCnt;
  
  Axle.RPM = Axle.PulseCnt * 60;
  Axle.RPM /= AxlePPR;  
  
  Axle.Duration = 0; 
  Axle.PulseCnt = 0;
  
  Axle.Freq = 1e6 / float(Axle._Duration);   
  Axle.Freq *= Axle._PulseCnt;

  Serial.print("Axle RPM: ");
  Serial.print(Axle.RPM);
  Serial.print(" Frequency: ");
  Serial.print(Axle.Freq);
  Serial.println(" Hz");

}
void HEAxleISR()
{
    digitalWrite(LEDPinB, HIGH);

    Axle.CurrMicros = micros();
    Axle.DiffMicros = Axle.CurrMicros - Axle.PrevMicros;
    Axle.Duration += Axle.DiffMicros;
    Axle.PrevMicros = Axle.CurrMicros;
  
    if (TESTING)
    {
        Axle.PulseCnt  += AxlePulseCnt;
    }
    else
    {
        Axle.PulseCnt++;
    } 

    AAxle.PeriodBetweenPulses = micros() - AAxle.LastTimeWeMeasured; 
    AAxle.LastTimeWeMeasured = micros();
  
    if(AAxle.PulseCnt >= AAxle.AmountOfReadings)
    {
        AAxle.PeriodAverage = AAxle.PeriodSum / AAxle.AmountOfReadings;
        AAxle.PulseCnt = 1; 
        AAxle.PeriodSum = AAxle.PeriodBetweenPulses;
        AAxle.RemapedAmountOfReadings = map(AAxle.PeriodBetweenPulses, 40000, 5000, 1, 10);
        AAxle.RemapedAmountOfReadings = constrain(AAxle.RemapedAmountOfReadings, 1, 10);
        AAxle.AmountOfReadings = AAxle.RemapedAmountOfReadings;
    }
    else
    {
        AAxle.PeriodSum = AAxle.PeriodSum + AAxle.PeriodBetweenPulses;
        if (TESTING)
        {
            AAxle.PulseCnt += AxlePulseCnt;
        }
        else
        {
            AAxle.PulseCnt++;
       }
    }

    digitalWrite(LEDPinB, LOW);

}
void HEAAxle()
{
  Serial.println("HEAAxle");
  NewOled.bmap = "HEAAxle ";
  OLEDloop();

  AAxle.LastTimeCycleMeasure = AAxle.LastTimeWeMeasured; 
  AAxle.CurrentMicros = micros(); 
  
  if(AAxle.CurrentMicros < AAxle.LastTimeCycleMeasure)
  {
    AAxle.LastTimeCycleMeasure = AAxle.CurrentMicros;
  }
  
  AAxle.FrequencyRaw = TenB / AAxle.PeriodAverage; 
  AAxle.ZeroDiff = AAxle.ZeroTimeout - AAxle.ZeroDebouncingExtra;  
  
  AAxle.DiffMicros = AAxle.CurrentMicros - AAxle.LastTimeCycleMeasure;       
  if(AAxle.PeriodBetweenPulses > AAxle.ZeroDiff 
  || AAxle.DiffMicros > AAxle.ZeroDiff)
  {  
    AAxle.FrequencyRaw = 0;
    AAxle.ZeroDebouncingExtra = 2000;
  }
  else
  {
    AAxle.ZeroDebouncingExtra = 0;
  }
  
  AAxle.FrequencyReal = AAxle.FrequencyRaw / 10000;
  AAxle.RPM = AAxle.FrequencyRaw / AxlePPR * 60;
  AAxle.RPM = AAxle.RPM / 10000; 
  AAxle.Total = AAxle.Total - AAxle.Readings[AAxle.readIndex];
  AAxle.Readings[AAxle.readIndex] = AAxle.RPM;
  AAxle.Total = AAxle.Total + AAxle.Readings[AAxle.readIndex];
  AAxle.readIndex = AAxle.readIndex + 1;
  
  if (AAxle.readIndex >= AAxle.numReadings)
  {
    AAxle.readIndex = 0;
  }
 
  AAxle.Average = AAxle.Total / AAxle.numReadings;

}
