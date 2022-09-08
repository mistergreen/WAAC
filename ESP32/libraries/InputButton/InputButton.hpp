// Andrea Villa
// villa.andrea@gmail.com

#ifndef InputButton_h
#define InputButton_h


#include <Device.h>
#include <Storable.h>
#include "Button2.h"


class InputButton : public Device, public Storable
{
  public:
    // Empty constructor for deserialization.
    InputButton();

    // Constructor
    InputButton(char *in_name, uint8_t in_pin);
    
    ~InputButton() {}; // destructor
    
    // It serializes the class into a Json document.
    void serialize(
    // Input Json object pointer to be filled with the class information.
        JsonObject& doc);
    
    // It fills the class using the information contained into the document.
    void deserialize(
        // Input Json object pointer containing the class information.
        JsonObject& doc);

    void loop(); //required
    const char * read(); // override

  private:

    // The button handler class.
    Button2 button;

    char output[5];
};

#endif