/*
 PCAhelper
// © 2noodles llc
// minh@2noodles.com
*/


#ifndef PCAhelper_h
#define PCAhelper_h


#include "Adafruit_PWMServoDriver.h"

class PCAhelper
{
  public:

    static bool isSet;
    static void init();
    static Adafruit_PWMServoDriver pwm;
  private:
    PCAhelper() {} //disallow instance creation
};
#endif