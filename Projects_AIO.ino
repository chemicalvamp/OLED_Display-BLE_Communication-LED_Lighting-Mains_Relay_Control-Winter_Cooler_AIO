#include <Arduino.h>
//#include <U8g2lib.h>

// Unused pin defines:
#define UnDefined0 = 4
#define UnDefined1 = 5 // PWM
#define UnDefined2 = 6 // PWM
#define UnDefined3 = 13
#define UnDefined5 = A2
#define UnDefined6 = A3

// Tempurature pin defines:
#define BoardThermisterPin A1
#define RemoteThermisterPin A0
#define ThermisterNominal 10000
#define TemperatureNominal 25
#define SamplesCount 5
#define BCoefficient 3950
#define SeriesResistor 10000
#define FanMOSFET 3

// Relay defines:
#define RelayOutput 11
#define PowerButton 12

// Command pin defines:
#define RX 0
#define TX 1
#define SCL A4
#define SDA A5
#define CMDSIZE 10

// Lighting pin defines
#define DrawerLightingMOSFET 5
#define CabinetLightingMOSFET 6
#define AuxLightingMOSFET 9
#define DrawerSwitch 8
#define CabinetSwitch 7
#define DopplerInput 2

// Timing variables:
int LastMillies = 0;

// Tempurature variables:
float DesiredAmbient = -12.95f; // Desired tempurature for this local space °C.
int FanPWM = 0;

// Relay Variables:
bool ShuttingDown = false;

// Lighting Variables:
bool DrawerIsIncrementing = true;
bool CabinetIsIncrementing = true;
int DrawerBreathingValue = 135;
int CabinetBreathingValue = 135;
int BreathingIncrement = 20;
int BreathingDecrement = 40;

// Timing constants:
String DebugZero = "Milliseconds last cycle (Δ Time): ";

// Tempurature constants:
int Samples[SamplesCount];
float Steinhart;
float DifferentialZero = 0.00f;
float AverageTemperature = -12.95f;
String DebugOne = "Temperature: ";
String DebugTwo = "°C ~ Desired: ";
String DebugThree = "°C ~ Differential: ";
String DebugFour = " ~ PWM Zero: ";

// Relay constants:
String DebugFive = "Relay state: ";
String DebugSix = "Online";
String DebugSeven = "Shutting down";
String DebugEight = "Offline";

// Lighting constants:
String DebugNine = "Drawer PWM: ";
String DebugTen = " ~ Cabinet PWM: ";

// Commands constants:
static char    cmd[CMDSIZE];              // command string buffer
static char *  bufp;                      // buffer pointer
static char *  bufe = cmd + CMDSIZE - 1;  // buffer end

void setup() 
{
  //pinMode(RemoteThermisterPin, INPUT);
  pinMode(FanMOSFET, OUTPUT);
  pinMode(RelayOutput, OUTPUT);
  pinMode(PowerButton, INPUT_PULLUP);
  pinMode(DrawerLightingMOSFET, OUTPUT);
  pinMode(CabinetLightingMOSFET, OUTPUT);
  pinMode(DrawerSwitch, INPUT_PULLUP);
  pinMode(CabinetSwitch, INPUT_PULLUP);
  //pinMode(DopplerInput, INPUT);
  digitalWrite(RelayOutput, HIGH);
  digitalWrite(DrawerLightingMOSFET, DrawerBreathingValue);
  digitalWrite(CabinetLightingMOSFET, CabinetBreathingValue);
  ShuttingDown = false;
  Serial.begin(9600);
  analogReference(EXTERNAL);
}

void loop() 
{
  if (digitalRead(PowerButton) == LOW)
  {
    digitalWrite(RelayOutput, 0);
    ShuttingDown = true;
  }
  DoSerial();
  DoLighting();
  DoTemperature();
  Debug();
  LastMillies = millis();
  //delay(80);
}


// https://learn.adafruit.com/thermistor/using-a-thermistor
// https://learn.adafruit.com/assets/571
void DoTemperature()
{
  uint8_t i;
  // take N samples in a row, with a slight delay
  for (i=0; i< SamplesCount; i++) 
  {
   Samples[i] = analogRead(RemoteThermisterPin);
   delay(5);
  }
 
  // average all the samples out
  AverageTemperature = 0;
  for (i=0; i< SamplesCount; i++) 
  {
     AverageTemperature += Samples[i];
  }
  AverageTemperature /= SamplesCount;
 
  // convert the value to resistance
  AverageTemperature = 1023 / AverageTemperature - 1;
  AverageTemperature = SeriesResistor / AverageTemperature;
 
  Steinhart = AverageTemperature / ThermisterNominal;     // (R/Ro)
  Steinhart = log(Steinhart);                  // ln(R/Ro)
  Steinhart /= BCoefficient;                   // 1/B * ln(R/Ro)
  Steinhart += 1.0 / (TemperatureNominal + 273.15); // + (1/To)
  Steinhart = 1.0 / Steinhart;                 // Invert
  Steinhart -= 273.15;                         // convert to C
   
  DifferentialZero = EliminateNegative(Steinhart - DesiredAmbient); // store the float difference in tempurature
  FanPWM = PWMClamp(map(DifferentialZero, 0, 5, 0, 255)); // 0°C differnace, no power. +5°C differance, full power.
  analogWrite(FanMOSFET, FanPWM);
}

void DoLighting()
{
  if(digitalRead(DrawerSwitch) == LOW)
  {
    digitalWrite(DrawerLightingMOSFET, HIGH);
  }
  else
  {
    digitalWrite(DrawerLightingMOSFET, DrawerLightingClamp());
  }

  if(digitalRead(CabinetSwitch) == LOW)
  {
    digitalWrite(CabinetLightingMOSFET, HIGH);
  }
  else
  {
    digitalWrite(CabinetLightingMOSFET, CabinetLightingClamp());
  }
}

int CabinetLightingClamp()
{
  if(CabinetIsIncrementing == true)
  {
    CabinetBreathingValue += BreathingIncrement;
  }
  else
  {
    CabinetBreathingValue -= BreathingDecrement;
  }
  
  if(CabinetBreathingValue >= 255)
  {
    CabinetIsIncrementing = false;
    CabinetBreathingValue = 255;
  }
  else if(CabinetBreathingValue <= 0)
  {
    CabinetIsIncrementing = true;
    CabinetBreathingValue = 0;
  }
  return CabinetBreathingValue;
}

int DrawerLightingClamp()
{
  if(DrawerIsIncrementing == true)
  {
    DrawerBreathingValue += BreathingIncrement;
  }
  else
  {
    DrawerBreathingValue -= BreathingDecrement;
  }
  if(DrawerBreathingValue >= 255)
  {
    DrawerIsIncrementing = false;
    DrawerBreathingValue = 255;
  }
  else if(DrawerBreathingValue <= 0)
  {
    DrawerIsIncrementing = true;
    DrawerBreathingValue = 0;
  }
  return DrawerBreathingValue;
}


void Debug()
{
  Serial.println(DebugZero + (millis() - LastMillies));
  Serial.print(DebugOne + Steinhart);
  Serial.print(DebugTwo + DesiredAmbient);
  Serial.print(DebugThree + DifferentialZero);
  Serial.println(DebugFour + FanPWM);

  if (ShuttingDown == true)
  {
    Serial.println(DebugFive + DebugSeven); // shutting down
  }
  else
  {
    Serial.println(DebugFive + DebugSix); // online
  }
  Serial.print(DebugNine + DrawerBreathingValue);
  Serial.println(DebugTen + CabinetBreathingValue);
}

void DoSerial()
{
  int ch;
  while (Serial.available() > 0) 
  {
    ch = Serial.read();

    if (ch == '\n') 
    {
      *bufp = '\0';
      HandleCommand();
      bufp = cmd;
    } 
    else {
      if (bufp < bufe) 
      {
        *bufp++ = ch;
      }
    }
  }
}

void HandleCommand()
{
  if (strcmp(cmd, "FanTempUp") == 0) 
  {
    DesiredAmbient += 0.5f;
  }
  if (strcmp(cmd, "FanTempDown") == 0) 
  {
    DesiredAmbient -= 0.5f;
  }
}
  
float EliminateNegative(float temp)
{
  if (temp <= 0)
    return 0;
  else 
    return temp;
}

int PWMClamp(float PWM){
  if(PWM >= 255)
    return 255;
   else if (PWM <= 32) // 48,64 worked alright
    return 0;
   else
    return PWM;
}

// command temporary space:
/*
    int value = 0;

    for (char * cmdp = cmd; *cmdp != '\0'; ++cmdp) 
    {
      if ((*cmdp >= '0') && (*cmdp <= '9')) 
      {
        value *= 10;
        value += *cmdp - '0';
      }
      else 
      {
        // optionally return error
        return;
      }
    }
    DesiredAmbient = value;
  }
*/
