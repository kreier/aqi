/*********************************************************************************************** 
 * Sketch:  BMP085_Text.ino
 * Author:  A. Kriwanek: http://www.kriwanek.de/arduino/wetterstation.html
 * Version: 1.00  01.09.2012/17:15
 *
 * This sketch tests a BMP085 barometric sensor. Temperature and pressure are read from the sensor
 * The mathematical computation is  done and the results will be displayed every second on the
 * serial monitor.
 *
 * Used Hardware:
 * - Sparkfun Breakout board BMP085
 * - Arduino UNO Protoboard
 * - Arduino UNO
 * 
 * My sketch is free software: Arduino BMP085_Text.ino von Andreas Kriwanek steht unter 
 * einer Creative Commons Namensnennung-Weitergabe unter gleichen Bedingungen 3.0 Unported Lizenz.
 *
 * My Sketch in wide parts is based on the sketch from Jim Lindblom:
 *
 * BMP085 Extended Example Code
 * by: Jim Lindblom
 * SparkFun Electronics
 * date: 1/18/11
 * license: CC BY-SA v3.0 - http://creativecommons.org/licenses/by-sa/3.0/
 *
 * Update (7/19/11): I've heard folks may be encountering issues
 * with this code, who're running an Arduino at 8MHz. If you're 
 * using an Arduino Pro 3.3V/8MHz, or the like, you may need to 
 * increase some of the delays in the bmp085ReadUP and 
 * bmp085ReadUT functions.
 * 
 * https://www.sparkfun.com/products/9694 Product page
 * http://de.wikipedia.org/wiki/Luftdruck Correction formula to sea level
 * http://www.meteo24.de/wetter/49X8157.html Weather Bad Aibling
 *
 * LCD 20x4 an I2C Port 0x27
 * BMP mit 3,3V versorgt, an I2C Port (zweiter auf MEGA2560) auf 0x77
 *
 * 26.01.2016 Matthias Kreier
 *
 **********************************************************************************************/

#include <Wire.h>
#include <LiquidCrystal_I2C.h> 

#define BMP085_ADDRESS 0x77        // I2C address of BMP085
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 20 chars and 4 line display 

const unsigned char OSS = 0;       // Oversampling Setting

// Calibration values of BMP085:
int ac1;
int ac2; 
int ac3; 
unsigned int ac4;
unsigned int ac5;
unsigned int ac6;
int b1; 
int b2;
int mb;
int mc;
int md;

// b5 is calculated in bmp085GetTemperature(...), this variable is also used in bmp085GetPressure(...)
// so ...Temperature(...) must be called before ...Pressure(...).
long b5; 

float temperature;
float pressure;
float pressureNN;
float pressureCorrection = 0.94028057;  // Höhe 492m ü.NN Bad Aibling. Please calculate your coeffizient
                                        // for your homes elevation!

//----------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------
void setup()
{
   lcd.init();                      // initialize the lcd  
   // Print a message to the LCD. 
   lcd.backlight(); 
   Wire.begin();
   bmp085Calibration();
}

//----------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------
void loop()
{
  temperature = bmp085GetTemperature(bmp085ReadUT())/10;
  pressure = bmp085GetPressure(bmp085ReadUP())/100;
  pressureNN = pressure / pressureCorrection;
  lcd.setCursor(5,0);
  lcd.print("Temperatur:");
  lcd.setCursor(5,1);
  lcd.print(temperature,1);
  lcd.print(" 'C");
  lcd.setCursor(5,2);
  lcd.print("Luftdruck:");
  lcd.setCursor(5,3);
  lcd.print(pressure,1);
  lcd.print(" hPa");
  delay(1000);
}

//----------------------------------------------------------------------------------------------------------
// Stores all of the bmp085's calibration values into global variables
// Calibration values are required to calculate temp and pressure
// This function should be called at the beginning of the program
void bmp085Calibration()
{
  ac1 = bmp085ReadInt(0xAA);
  ac2 = bmp085ReadInt(0xAC);
  ac3 = bmp085ReadInt(0xAE);
  ac4 = bmp085ReadInt(0xB0);
  ac5 = bmp085ReadInt(0xB2);
  ac6 = bmp085ReadInt(0xB4);
  b1 = bmp085ReadInt(0xB6);
  b2 = bmp085ReadInt(0xB8);
  mb = bmp085ReadInt(0xBA);
  mc = bmp085ReadInt(0xBC);
  md = bmp085ReadInt(0xBE);
}

//----------------------------------------------------------------------------------------------------------
// Calculate temperature given ut.
// Value returned will be in units of 0.1 deg C
short bmp085GetTemperature(unsigned int ut)
{
  long x1, x2;
  
  x1 = (((long)ut - (long)ac6)*(long)ac5) >> 15;
  x2 = ((long)mc << 11)/(x1 + md);
  b5 = x1 + x2;

  return ((b5 + 8)>>4);  
}

//----------------------------------------------------------------------------------------------------------
// Calculate pressure given up
// calibration values must be known
// b5 is also required so bmp085GetTemperature(...) must be called first.
// Value returned will be pressure in units of Pa.
long bmp085GetPressure(unsigned long up)
{
  long x1, x2, x3, b3, b6, p;
  unsigned long b4, b7;
  
  b6 = b5 - 4000;
  // Calculate B3
  x1 = (b2 * (b6 * b6)>>12)>>11;
  x2 = (ac2 * b6)>>11;
  x3 = x1 + x2;
  b3 = (((((long)ac1)*4 + x3)<<OSS) + 2)>>2;
  
  // Calculate B4
  x1 = (ac3 * b6)>>13;
  x2 = (b1 * ((b6 * b6)>>12))>>16;
  x3 = ((x1 + x2) + 2)>>2;
  b4 = (ac4 * (unsigned long)(x3 + 32768))>>15;
  
  b7 = ((unsigned long)(up - b3) * (50000>>OSS));
  if (b7 < 0x80000000)
    p = (b7<<1)/b4;
  else
    p = (b7/b4)<<1;
    
  x1 = (p>>8) * (p>>8);
  x1 = (x1 * 3038)>>16;
  x2 = (-7357 * p)>>16;
  p += (x1 + x2 + 3791)>>4;
  
  return p;
}

//----------------------------------------------------------------------------------------------------------
// Read 1 byte from the BMP085 at 'address'
char bmp085Read(unsigned char address)
{
  unsigned char data;
  
  Wire.beginTransmission(BMP085_ADDRESS);
  Wire.write(address);
  Wire.endTransmission();
  
  Wire.requestFrom(BMP085_ADDRESS, 1);
  while(!Wire.available())
    ;
    
  return Wire.read();
}

//----------------------------------------------------------------------------------------------------------
// Read 2 bytes from the BMP085
// First byte will be from 'address'
// Second byte will be from 'address'+1
int bmp085ReadInt(unsigned char address)
{
  unsigned char msb, lsb;
  
  Wire.beginTransmission(BMP085_ADDRESS);
  Wire.write(address);
  Wire.endTransmission();
  
  Wire.requestFrom(BMP085_ADDRESS, 2);
  while(Wire.available()<2)
    ;
  msb = Wire.read();
  lsb = Wire.read();
  
  return (int) msb<<8 | lsb;
}

//----------------------------------------------------------------------------------------------------------
// Read the uncompensated temperature value
unsigned int bmp085ReadUT()
{
  unsigned int ut;
  
  // Write 0x2E into Register 0xF4
  // This requests a temperature reading
  Wire.beginTransmission(BMP085_ADDRESS);
  Wire.write(0xF4);
  Wire.write(0x2E);
  Wire.endTransmission();
  
  // Wait at least 4.5ms
  delay(5);
  
  // Read two bytes from registers 0xF6 and 0xF7
  ut = bmp085ReadInt(0xF6);
  return ut;
}

//----------------------------------------------------------------------------------------------------------
// Read the uncompensated pressure value
unsigned long bmp085ReadUP()
{
  unsigned char msb, lsb, xlsb;
  unsigned long up = 0;
  
  // Write 0x34+(OSS<<6) into register 0xF4
  // Request a pressure reading w/ oversampling setting
  Wire.beginTransmission(BMP085_ADDRESS);
  Wire.write(0xF4);
  Wire.write(0x34 + (OSS<<6));
  Wire.endTransmission();
  
  // Wait for conversion, delay time dependent on OSS
  delay(2 + (3<<OSS));
  
  // Read register 0xF6 (MSB), 0xF7 (LSB), and 0xF8 (XLSB)
  Wire.beginTransmission(BMP085_ADDRESS);
  Wire.write(0xF6);
  Wire.endTransmission();
  Wire.requestFrom(BMP085_ADDRESS, 3);
  
  // Wait for data to become available
  while(Wire.available() < 3)
    ;
  msb = Wire.read();
  lsb = Wire.read();
  xlsb = Wire.read();
  
  up = (((unsigned long) msb << 16) | ((unsigned long) lsb << 8) | (unsigned long) xlsb) >> (8-OSS);
  
  return up;
}
//----------------------------------------------------------------------------------------------------------

