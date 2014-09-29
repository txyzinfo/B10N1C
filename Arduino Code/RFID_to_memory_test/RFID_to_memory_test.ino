/*
 * MFRC522 - Library to use ARDUINO RFID MODULE KIT 13.56 MHZ WITH TAGS SPI W AND R BY COOQROBOT.
 * The library file MFRC522.h has a wealth of useful info. Please read it.
 * The functions are documented in MFRC522.cpp.
 *
 * Based on code Dr.Leong   ( WWW.B2CQSHOP.COM )
 * Created by Miguel Balboa (circuitito.com), Jan, 2012.
 * Rewritten by Søren Thing Andersen (access.thing.dk), fall of 2013 (Translation to English, refactored, comments, anti collision, cascade levels.)
 * Released into the public domain.
 *
 * Sample program showing how to read data from a PICC using a MFRC522 reader on the Arduino SPI interface.
 *----------------------------------------------------------------------------- empty_skull 
 * Aggiunti pin per arduino Mega
 * add pin configuration for arduino mega
 * http://mac86project.altervista.org/
 ----------------------------------------------------------------------------- Nicola Coppola
 * Pin layout should be as follows:
 * Signal     Pin              Pin               Pin
 *            Arduino Uno      Arduino Mega      MFRC522 board
 * ------------------------------------------------------------
 * Reset      9                5                 RST
 * SPI SS     10               53                SDA
 * SPI MOSI   11               51                MOSI
 * SPI MISO   12               50                MISO
 * SPI SCK    13               52                SCK
 *
 * The reader can be found on eBay for around 5 dollars. Search for "mf-rc522" on ebay.com. 
 *

Connect i2c 24LCXXX memory as follows:

Arduino analog pin 4 to EEPROM pin 5
Arduino analog pin 5 to EEPROM pin 6
Arduino 5V to EEPROM pin 8
Arduino GND to EEPROM pin 1,2,3,4
Be sure to leave pin 7 of the EEPROM open or tie it to GND otherwise the EEPROM will be write protected.
 
*/

#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>

#define SS_PIN 10
#define RST_PIN A0
MFRC522 mfrc522(SS_PIN, RST_PIN);	// Create MFRC522 instance.

const int ledCount = 8;    // the number of LEDs in the bar graph

char data[4] = "";


int ledPins[] = { 
  2, 3, 4, 5, 6, 7,8,9};   // an array of pin numbers to which LEDs are attached

void setup() {
  
    for (int thisLed = 0; thisLed < ledCount; thisLed++) {
    pinMode(ledPins[thisLed], OUTPUT); 
  }
  
       Wire.begin(); // initialise the connection
       
	Serial.begin(9600);	// Initialize serial communications with the PC
	SPI.begin();			// Init SPI bus
	mfrc522.PCD_Init();	// Init MFRC522 card
	Serial.println("Scan PICC to see UID and type...");
}

void loop() {

    
	// Look for new cards
	if ( ! mfrc522.PICC_IsNewCardPresent()) {
		return;
	}

	// Select one of the cards
	if ( ! mfrc522.PICC_ReadCardSerial()) {
		return;
	}

	// Dump debug info about the card. PICC_HaltA() is automatically called.
	// mfrc522.PICC_DumpToSerial(&(mfrc522.uid));

        // Dump UID
        Serial.print("Card UID:");
        for (byte i = 0; i < mfrc522.uid.size; i++) {
                Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
                Serial.print(mfrc522.uid.uidByte[i], HEX);
                data[i] = (byte)mfrc522.uid.uidByte[i];
        } 
        Serial.println();

            digitalWrite(ledPins[0], HIGH);  digitalWrite(ledPins[7], HIGH);  delay(40);
            digitalWrite(ledPins[0], LOW);  digitalWrite(ledPins[7], LOW);
            digitalWrite(ledPins[1], HIGH);  digitalWrite(ledPins[6], HIGH);  delay(40);
            digitalWrite(ledPins[1], LOW);  digitalWrite(ledPins[6], LOW); 
            digitalWrite(ledPins[2], HIGH);  digitalWrite(ledPins[5], HIGH);  delay(40);
            digitalWrite(ledPins[2], LOW);  digitalWrite(ledPins[5], LOW);
            digitalWrite(ledPins[3], HIGH);  digitalWrite(ledPins[4], HIGH);  delay(20);
              for (int fadeValue = 200 ; fadeValue >= 0; fadeValue -= 1) {
                analogWrite(ledPins[3], fadeValue); analogWrite(ledPins[4], fadeValue); 
                delay(5);
              }
  

//          for( int i = 0; i < sizeof(data);  ++i )
//           Serial.print (data[i], HEX);
           
//          Serial.println();
         
         //Writeing to memory 
          i2c_eeprom_write_page(0x50, 0, (byte *)data, sizeof(data)); // write to EEPROM
          
          digitalWrite(ledPins[3], HIGH);  digitalWrite(ledPins[4], HIGH);  delay(40);
          digitalWrite(ledPins[3], LOW);  digitalWrite(ledPins[4], LOW);  delay(40);
          digitalWrite(ledPins[3], HIGH);  digitalWrite(ledPins[4], HIGH);  delay(40);
          digitalWrite(ledPins[3], LOW);  digitalWrite(ledPins[4], LOW);  delay(40);
          
//          //Reading from memory
//          int addr=0; //first address
//          byte b = i2c_eeprom_read_byte(0x50, 0); // access the first address from the memory
//
//          while (b!=0) 
//              {
//                Serial.print((byte)b, HEX); //print content to serial port
//                addr++; //increase address
//                b = i2c_eeprom_read_byte(0x50, addr); //access an address from the memory
//              }

          for( int i = 0; i < sizeof(data);  ++i )
          data[i] = (char)0;  

}


  void i2c_eeprom_write_byte( int deviceaddress, unsigned int eeaddress, byte data ) {
    int rdata = data;
    Wire.beginTransmission(deviceaddress);
    Wire.write((int)(eeaddress >> 8)); // MSB
    Wire.write((int)(eeaddress & 0xFF)); // LSB
    Wire.write(rdata);
    Wire.endTransmission();
  }

  // WARNING: address is a page address, 6-bit end will wrap around
  // also, data can be maximum of about 30 bytes, because the Wire library has a buffer of 32 bytes
  void i2c_eeprom_write_page( int deviceaddress, unsigned int eeaddresspage, byte* data, byte length ) {
    Wire.beginTransmission(deviceaddress);
    Wire.write((int)(eeaddresspage >> 8)); // MSB
    Wire.write((int)(eeaddresspage & 0xFF)); // LSB
    byte c;
    for ( c = 0; c < length; c++)
      Wire.write(data[c]);
    Wire.endTransmission();
  }

  byte i2c_eeprom_read_byte( int deviceaddress, unsigned int eeaddress ) {
    byte rdata = 0xFF;
    Wire.beginTransmission(deviceaddress);
    Wire.write((int)(eeaddress >> 8)); // MSB
    Wire.write((int)(eeaddress & 0xFF)); // LSB
    Wire.endTransmission();
    Wire.requestFrom(deviceaddress,1);
    if (Wire.available()) rdata = Wire.read();
    return rdata;
  }

  // maybe let's not read more than 30 or 32 bytes at a time!
  void i2c_eeprom_read_buffer( int deviceaddress, unsigned int eeaddress, byte *buffer, int length ) {
    Wire.beginTransmission(deviceaddress);
    Wire.write((int)(eeaddress >> 8)); // MSB
    Wire.write((int)(eeaddress & 0xFF)); // LSB
    Wire.endTransmission();
    Wire.requestFrom(deviceaddress,length);
    int c = 0;
    for ( c = 0; c < length; c++ )
      if (Wire.available()) buffer[c] = Wire.read();
  }

