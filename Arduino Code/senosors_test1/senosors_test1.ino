/**************************************************************************/
/*!
    @file     Adafruit_MMA8451.h
    @author   K. Townsend (Adafruit Industries)
    @license  BSD (see license.txt)

    This is an example for the Adafruit MMA8451 Accel breakout board
    ----> https://www.adafruit.com/products/2019

    Adafruit invests time and resources providing this open source code,
    please support Adafruit and open-source hardware by purchasing
    products from Adafruit!

    @section  HISTORY

    v1.0  - First release
*/
/**************************************************************************/

#include <Wire.h>
#include <Adafruit_MMA8451.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADS1015.h>
#include <RFduinoBLE.h>

Adafruit_MMA8451 mma = Adafruit_MMA8451();

void setup(void) {
  
  Serial.begin(9600);
    ads.begin();
    
  Serial.println("Adafruit MMA8451 test!");
  

  if (! mma.begin()) {
    Serial.println("Couldnt start");
    while (1);
  }
  Serial.println("MMA8451 found!");
  
  mma.setRange(MMA8451_RANGE_2_G);
  
  Serial.print("Range = "); Serial.print(2 << mma.getRange());  
  Serial.println("G");
  
}

void loop() {
   int16_t adc0, adc1, adc2, adc3;
  // Read the 'raw' data in 14-bit counts
  mma.read();
 RFduinoBLE.sendInt(mma.x); 
 RFduinoBLE.sendInt (mma.y); 
 RFduinoBLE.sendInt (mma.z); 
  adc0 = ads.readADC_SingleEnded(0);
  adc1 = ads.readADC_SingleEnded(1);
  adc2 = ads.readADC_SingleEnded(2);
  adc3 = ads.readADC_SingleEnded(3);
 RFduinoBLE.sendInt(adc0);
RFduinoBLE.sendInt(adc1);
RFduinoBLE.sendInt(adc2);
RFduinoBLE.sendInt(adc3);


  /* Get a new sensor event */ 
  sensors_event_t event; 
  mma.getEvent(&event);



  delay(500);
  
}
