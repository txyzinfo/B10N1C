const int analogPin = A6;   // the pin that the potentiometer is attached to
const int ledCount = 8;    // the number of LEDs in the bar graph

const int numReadings = 4;

int readings[numReadings];      // the readings from the analog input
int index = 0;                  // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average

int i = 0;

int ledPins[] = { 
  2, 3, 4, 5, 6, 7,8,9};   // an array of pin numbers to which LEDs are attached


void setup() {
  Serial.begin(9600);  
  // loop over the pin array and set them all to output:
  for (int thisLed = 0; thisLed < ledCount; thisLed++) {
    pinMode(ledPins[thisLed], OUTPUT); 
  }
  
    for (int thisReading = 0; thisReading < numReadings; thisReading++)
    readings[thisReading] = 0; 
}

void loop() {
  
    // subtract the last reading:
  total= total - readings[index];         
  // read from the sensor:  
  readings[index] = analogRead(analogPin); 
  // add the reading to the total:
  total= total + readings[index];       
  // advance to the next position in the array:  
  index = index + 1;                    

  // if we're at the end of the array...
  if (index >= numReadings)              
    // ...wrap around to the beginning: 
    index = 0;                           

  // calculate the average:
  average = total / numReadings;         

  // average = analogRead(analogPin); 
  // map the result to a range from 0 to the number of LEDs:
  int ledLevel = map(average, 600, 1000, 0, 10);
 

  // loop over the LED array:
  for (int thisLed = 0; thisLed < ledCount; thisLed++) {
    // if the array element's index is less than ledLevel,
    // turn the pin for this element on:
    if (thisLed < ledLevel) {
      digitalWrite(ledPins[thisLed], HIGH);
    } 
    // turn off all pins higher than the ledLevel:
    else {}
    }
    
    if (ledLevel > 9) {
           for (int i = 0; i < 4; i++) { 
              for (int thisLed = 0; thisLed < ledCount; thisLed++) {
                 digitalWrite(ledPins[thisLed], HIGH);
              }
              delay(100);
              for (int thisLed = 0; thisLed < ledCount; thisLed++) {
                 digitalWrite(ledPins[thisLed], LOW);
              }
              delay(100);
           }
              
              total = 0;
              index = 0;
              for (int i; i < numReadings; i++) { readings[i] = 0; }
              delay(10);
          }
    

   
  
 // delay(0);
}


