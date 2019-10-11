#include <Wire.h>
#define I2CADDR 0xB8 
#define READCMD 0x03   // Lesekommando war 0x03
#define REGSTADDR 0x00 
#define REGCOUNT 4

#define REGADDR 0x0B

// 1 VDD         Pins des AM2320 von links
// 2 SDA - A4
// 3 GND
// 4 SCL - A5

void setup() {
  Wire.begin(); // wake up I2C bus
  //Wire.setClock(100);
  Serial.begin(9600);
  Serial.println("Messwerte des AM2320");
}

void loop() {
  byte buf[10];
  //
  // Read Command
  //
  Wire.beginTransmission(I2CADDR);
  Wire.write(0x00);  // READCMD
  Wire.write(0x00);  // REGSTADDR
  //Wire.write(REGCOUNT);
  Wire.endTransmission();
  //
  // Waiting
  //
  delay(1600); //>1.5ms
  //
  // Read
  //
  //Wire.read();
  Wire.requestFrom(I2CADDR, 2 + REGCOUNT + 2); // COMMAND + DATA + REGCOUNT + CRCLSB + CRCMSB
  int i = 0;
  for (; i < 2 + REGCOUNT + 2; i++)
      buf[i] = Wire.read();
  for (i=0; i < 2 + REGCOUNT + 2; i++) {
      Serial.print(buf[i]);
      Serial.print("  ");
  }
  Serial.print("\n");
  delay(3000);
}
