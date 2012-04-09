/*
  DigitalReadSerial
 Reads a digital input on pin 2, prints the result to the serial monitor 
 
 This example code is in the public domain.
 */

#include <CapacitiveSensing.h>
#include <util/delay.h>
#include "stdlib.h"
#include "stdio.h"




void setup() {
  //initialize the capacitive sensing
  CapacitiveSensing.begin();
  //initialize the serial module at 9600 baud
  Serial.begin(9600);
  //set our LED pin as output, so we can flash the LED.
  pinMode(11, OUTPUT);
}

void loop() {
  int maxval = 16;
  //get a single-pad touch-sensing value from pin 19 
  //with a pullup resistor (~1 Megaohm) connected between pin 19 and VCC
  int sensorValue = CapacitiveSensing.capacitiveSense(PULLUP,5);
  
  //get a two-pad touch-sensing value between pins 19 and 15
  //with a pullup resisor (~1 Megaohm) connected between pin 19 and VCC)
  int touchValue = CapacitiveSensing.hybridSense(PULLUP,5,6);
  
  //print those values to serial
  Serial.print(sensorValue, DEC);
  Serial.print(", ");
  Serial.println(touchValue, DEC);
  
  //scale touchValue from 0-1024 to 0-maxval
  touchValue = (touchValue*maxval)/1024;
  //make sure touchValue is more than 0 and less than maxval
  touchValue = min(max(touchValue,0),maxval);
  
  if(touchValue) {
    //turn on the LED
    digitalWrite(11,LOW);
    //wait some milliseconds according to the touchvalue
    delay(touchValue);
  }
  
  //turn off the LED
  digitalWrite(11,HIGH);
  //wait some more milliseconds so the LED blinks at a constant rate.
  delay(maxval-touchValue);  
}


