/*
  CapacitiveSensing.h - Library for performing capacitive sensing
  Created by Scott M. Gilliland, October 1, 2011.
  Released into the public domain.
*/

#ifndef CapacitiveSensing_h
#define CapacitiveSensing_h

#include "WProgram.h"
#include <inttypes.h>



#define TMAX 1<<12
#define CTMAX 1<<10

#define PULLUP 1
#define PULLDOWN 0

class CapacitiveSensingClass
{
  public:
    void begin();

    int hybridSense(uint8_t pullup, uint8_t sense, uint8_t excitation);
    int capacitiveSense(uint8_t pullup, uint8_t pin);

};
extern CapacitiveSensingClass CapacitiveSensing;



#endif
