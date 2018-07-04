/*
 *  IRBEACON.h
 *  How to use:
 *  IRBEACON::read(PA5) returns a value
 *  if returns 0, there is no beacon in sight
 *  if returns 1, there is a 1khz beacon in sight
 *  if returns 2, there is a 10khz beacon in sight
 */

#ifndef IRBEACON_H
#define IRBEACON_H

#include <Arduino.h>
#include <HardwareTimer.h>

//DO NOT CHANGE! WORKS PERFECT!
#define SAMPLING_TIME 5 //sampling period in us (microseconds)
#define NUMBER_OF_SAMPLES 300 //number of samples in array

#define TIMER 4

class IRBEACON
{
  public:
    static int read(int _pin);
  private:
    static int times_crossed();
    static int pin;
    static int count;
    static int readings[NUMBER_OF_SAMPLES];
    IRBEACON() {}
    static void sample();
};

#endif