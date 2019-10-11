#include <Wire.h>
#include <AM2320.h>

// 1 VDD         Pins des AM2320 von links
// 2 SDA - A4
// 3 GND
// 4 SCL - A5

unsigned char txt[6];
signed int T = 0x0000;
unsigned int RH = 0x0000;
unsigned int CRC_data = 0x0000;
unsigned int CRC_temp = 0x0000;

void setup() {
  AM2320_init();

  Serial.begin(9600);
  Serial.println("Messwerte des AM2320");
}

void loop() {
  get_sensor_data(AM2320_RH_hb, 0x04, 0x08);
  get_RH_and_temperature(&RH, &T);
  get_CRC(&CRC_temp);
  CRC_data = CRC16(data_buffer, 6);
  
  if(CRC_temp == CRC_data) {
    Serial.print("Relative Feuchte: ");
    Serial.print(RH);
    Serial.print("  Temperatur: ");
    Serial.print(T);
    Serial.print("  CRC16: 0x");
    //WordToHex(CRC_data, txt);
    Serial.print(CRC_data);
    Serial.print("\n");
  }
  else {
    Serial.print("Error\n");
  }
  delay(4000);
}
