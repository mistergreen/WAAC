/*
 AdaFuitAdaFruitPWM8
*/

// © 2noodles llc
// minh@2noodles.com

#ifndef AdaFruitPWM8_h
#define AdaFruitPWM8_h

#include "PWM4.h"

class AdaFruitPWM8 : public PWM4
{
  public:
    // Empty constructor for deserialization.
    AdaFruitPWM8();

    // Class constructor, it sets the default values.
    AdaFruitPWM8(char *in_name, int in_dependent_device_id, int insda, int inscl);
    ~AdaFruitPWM8() {}; // destructor
    
    void setPins(int red, int green, int blue, int white, int aux1, int aux2, int aux3, int aux4);
    void getPins(int *inArray);
    
    void setPWMs(int in_red, int in_green, int in_blue, int in_white, int in_aux1, int in_aux2, int in_aux3, int in_aux4);
   
    void getI2C(int *inArray);
    void setI2C(int insda, int inscl);
    
    // It serializes the class into a Json document.
    void serialize(
    // Input Json object pointer to be filled with the class information.
        JsonObject& doc);
    
    // It fills the class using the information contained into the document.
    void deserialize(
        // Input Json object pointer containing the class information.
        JsonObject& doc);

  private:
    
    int SDA;
    int SCL;

    // It sets the pin value.
    void setPin(uint8_t channel, uint16_t color);
};

#endif