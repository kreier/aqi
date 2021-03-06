/* SFE_BMP180 library example sketch
*
*/

#include <SFE_BMP180.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h> 

SFE_BMP180 pressure;
LiquidCrystal_I2C lcd(0x3F,20,4); // oder 27

char status;
double T,T1,P,P1,p0,a;
int durchlauf = 0;

void setup()
{
  lcd.init();
  lcd.backlight(); 
  Wire.begin();
  lcd.setCursor(0,0);
  if (pressure.begin())
    lcd.print("BMP180 init success");
  else
  {
    // Oops, something went wrong, this is usually a connection problem,
    lcd.print("BMP180 init fail\n\n");
    while(1); // Pause forever.
  }
  for(int i=0;i<10;i++) {
  do {
    status = pressure.startTemperature();
  } while (status = 0);
  delay(100);
  do {
    status = pressure.getTemperature(T1);
  } while (status = 0);
  do {
    status = pressure.startPressure(3);
  } while (status = 0);
  delay(100);
  do {
    status = pressure.getPressure(P1,T1);
  } while (status = 0);
  p0 = P1;
  lcd.clear();
  lcd.setCursor(0,1);
  lcd.print(" Temp: ");
  lcd.print(T1,4);
  lcd.print(" 'C");
  lcd.setCursor(0,2);
  lcd.print("Druck: ");
  lcd.print(P1,3);
  lcd.print(" hPa  ");
  a = pressure.altitude(P1,p0);
  lcd.setCursor(0,3);
  lcd.print("Hoehe: ");
  lcd.print(a,3);
  lcd.print(" m  ");
  delay(100);
  }
}

void loop()
{
  status = pressure.startTemperature();
  if (status != 0)
  {
    // Wait for the measurement to complete:
    delay(status);

    // Retrieve the completed temperature measurement:
    // Note that the measurement is stored in the variable T.
    // Function returns 1 if successful, 0 if failure.

    status = pressure.getTemperature(T);
    if (status != 0)
    {
      lcd.setCursor(0,0);
      lcd.print("Bosch BMP180 Sensor");
      
      // Start a pressure measurement:
      // The parameter is the oversampling setting, from 0 to 3 (highest res, longest wait).
      // If request is successful, the number of ms to wait is returned.
      // If request is unsuccessful, 0 is returned.

      status = pressure.startPressure(3);
      if (status != 0)
      {
        // Wait for the measurement to complete:
        delay(status);

        // Retrieve the completed pressure measurement:
        // Note that the measurement is stored in the variable P.
        // Note also that the function requires the previous temperature measurement (T).
        // (If temperature is stable, you can do one temperature measurement for a number of pressure measurements.)
        // Function returns 1 if successful, 0 if failure.

        status = pressure.getPressure(P,T);
        if (status != 0)
        {
          // Durchschnitt aus 3 Durchläufen
          T1 = 0.9*T1 + 0.1*T;
          P1 = 0.9*P1 + 0.1*P;
          if (durchlauf > 15)
          {
            durchlauf = 0;
            // Print out the measurement:
            lcd.setCursor(0,1);
            lcd.print(" Temp: ");
            lcd.print(T1,4);
            lcd.print(" 'C  ");
            lcd.setCursor(0,2);
            lcd.print("Druck: ");
            lcd.print(P1,3);
            lcd.print(" hPa  ");
            a = pressure.altitude(P1,p0);
            lcd.setCursor(0,3);
            lcd.print("Hoehe: ");
            lcd.print(a,3);
            lcd.print(" m  ");
          }
          durchlauf++;
        }
        else lcd.println("error retrieving pressure measurement\n");
      }
      else lcd.println("error starting pressure measurement\n");
    }
    else lcd.println("error retrieving temperature measurement\n");
  }
  else lcd.println("error starting temperature measurement\n");

  delay(100);  // Pause for 0.1 seconds.
}
