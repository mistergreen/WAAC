/*
 PCAhelper
// © 2noodles llc
// minh@2noodles.com
*/

#include <Arduino.h>
#include "PCAhelper.h"

bool PCAhelper::isSet = false;
Adafruit_PWMServoDriver PCAhelper::pwm = Adafruit_PWMServoDriver();

void PCAhelper::init() {
    
    pwm.begin();
    pwm.setPWMFreq(1600);
    isSet = true;
    
}

