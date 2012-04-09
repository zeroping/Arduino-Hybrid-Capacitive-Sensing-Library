/*
  CapacitiveSensing.cpp - Library for performing capacitive sensing
  Created by Scott M. Gilliland, October 1, 2011.
  Released into the public domain.
*/

#include "WProgram.h"
#include "CapacitiveSensing.h"

#include <avr/io.h>
#include <util/delay.h>
#include <pins_arduino.h>
#include <inttypes.h>





void CapacitiveSensingClass::begin()
{

  //We have to disable the AVR's internal pull-up resistors to get our sensing to work
  //(technically, we can still do this when we're using an external pull-up resisor, 
  // but we disable them globally for flexibility
  bitSet(MCUCR,PUD);
	
}


  


/** this takes in a 0 or a 1 (for a pull up or pull down) and a pin number, 
  * and does capacitive sensing. It returns a numbe rbetween 0 and 1023 
  * that tells you vaugely how much capacitance the pin has to ground. 
  *
  * note that, because we have to do some defererences, using a pin number is about 50% slower 
  * than using the I/O registers directly. **/

int CapacitiveSensingClass::capacitiveSense(uint8_t pullup, uint8_t pin) {
  uint16_t time2 = 0;
  //uint8_t sensemask = (1<<6);
  uint8_t senseMask = digitalPinToBitMask(pin);
  volatile uint8_t* senseOutR = portOutputRegister(digitalPinToPort(pin)); 
  volatile uint8_t* senseInR = portInputRegister(digitalPinToPort(pin));
  volatile uint8_t* senseModeR = portModeRegister(digitalPinToPort(pin));
  
  //make sure we know our input value
  if(pullup){
    pullup = 1; 
  }
      
  //for now, let's do this hardcoded
  // sense line is F6
  // exc line is F7
  
  //charge capacitor
  //DDRF |= sensemask;
  *senseModeR |= senseMask;
  if (pullup){
    //PORTF &= ~sensemask;
    *senseOutR &= ~senseMask;
  } else {
    //PORTF |= sensemask;
    *senseOutR |= senseMask;
  }
  _delay_us(155);
  _delay_ms(1);
  
  //now sense
  
  // set the sense line as input
  //DDRF &= ~sensemask;
  *senseModeR &= ~senseMask;

  //while the capacitor stays discharged, keep counting up
  if (pullup) {
    //while( (PINF & sensemask) == 0 && time2<CTMAX)
    while( (*senseInR & senseMask) == 0 && time2<CTMAX)
      time2 ++;
  } else {
    //while( (PINF & sensemask) != 0 && time2<CTMAX)
    while( (*senseInR & senseMask) != 0 && time2<CTMAX)
      time2 ++;    
  }
  
  //set sense as output
  //DDRF |= sensemask;
  *senseModeR |= senseMask;
  
  // set the capacitor completely charged (to ensure that it's ready next time)
  if (pullup){
    //PORTF = sensemask;
    *senseOutR |= senseMask;
  } else {
    //PORTF &= ~sensemask;
    *senseOutR &= ~senseMask;
  }
  _delay_us(15);
  
  //set as input before we leave it
  //DDRF &= ~sensemask;
  *senseModeR &= ~senseMask;
 
  return (time2);
}



/** This is designed to take two lines, one with a pull-up resisitor, 
  * and determine if there's a person touching between them.
  * The 'sense' line needs the pull-up.
  **/
int CapacitiveSensingClass::hybridSense(uint8_t pullup, uint8_t sense, uint8_t excitation) {
  
  uint16_t time1=0, time2=0;
  pinMode(excitation, OUTPUT);
  
  if (pullup) {
    digitalWrite(excitation, HIGH);
  } else {
    digitalWrite(excitation, LOW);
  }
  //let things settle
  _delay_us(150);
  
  time1 = capacitiveSense(pullup, sense);

  if (pullup) {
    digitalWrite(excitation, LOW);
  } else {
    digitalWrite(excitation, HIGH);
  }
  //let things settle
  _delay_us(150);

  time2 = capacitiveSense(pullup, sense);
  
  pinMode(excitation, INPUT);
  
  return time2-time1;
  
}


CapacitiveSensingClass CapacitiveSensing;



