// © 2noodles llc
// minh@2noodles.com
// Andrea Villa
// villa.andrea@gmail.com

 
// Loads the configuration from a file
void loadDevices(const char *filename) {
  Serial.println ("Loading Devices");

  // Open file for reading
  File file = FS_HANDLER.open(filename);

  // Deserialize the JSON document
  DeserializationError error = deserializeJson(doc, file);

  // Check the parsing status first.
  if (error)
    Serial.println(F("Failed to read file, using default configuration"));
  else {
    // First get the number of stored devices.
    int numDevices = doc[0]["numDevices"];
    
    Serial.print ("Found devices: ");
    Serial.println (numDevices);
  
    //get devices name & id for dependent list. Print for everybody. They can use with it what they will.
    // saves adding on to the condition
    for(int i = 0; i < numDevices; i++) {

      // Get the JSON object.
      JsonObject obj = doc[i + 1];

      const char* deviceType = obj["classType"];
      
      Serial.print ("Creating ");
      Serial.println (deviceType);

      Device* temp = NULL;
      
      // Find the right device.
      if (strcmp("ServoMotor", deviceType) == 0)
      {
        temp = new ServoMotor();
      }
      else if (strcmp("StepperMotor", deviceType) == 0)
      {
        temp = new StepperMotor();
      }
      else if (strcmp("Relay", deviceType) == 0)
      {
        temp = new Relay();
      }
      else if (strcmp("RelayMCP", deviceType) == 0)
      {
        temp = new RelayMCP();
      }
      else if (strcmp("RelayPCA", deviceType) == 0)
      {
        temp = new RelayPCA();
      }
      else if (strcmp("OneWireSensor", deviceType) == 0)
      {
        temp = new OneWireSensor();
      }
      else if (strcmp("PWM4", deviceType) == 0)
      {
        temp = new PWM4();
      }
      else if (strcmp("AdaFruitPWM8", deviceType) == 0)
      {
        temp = new AdaFruitPWM8();
      }
      else if (strcmp("Alert", deviceType) == 0)
      {
        temp = new Alert();
      }
      else if (strcmp("Analog", deviceType) == 0)
      {
        temp = new Analog();
      }
      else if (strcmp("HallSensor", deviceType) == 0)
      {
        temp = new HallSensor();
      }
      else if (strcmp("Input", deviceType) == 0)
      {
        temp = new Input();
      }
      else if (strcmp("InputMCP", deviceType) == 0)
      {
        temp = new InputMCP();
      }
      else if (strcmp("InputButton", deviceType) == 0)
      {
        temp = new InputButton();
      }
      else if (strcmp("Video", deviceType) == 0)
      {
        temp = new Video();
      }
      
      if (temp != NULL)
      {
        temp->deserialize(obj);
        deviceDelegate.addDevice(temp, temp->getDeviceId());

        if (strcmp("HallSensor", deviceType) == 0)
        {
           static_cast<HallSensor*>(deviceDelegate.currentDevice())->setup([](){ static_cast<HallSensor*>(deviceDelegate.currentDevice())->rpm(); }, RISING);
        }
      }
    }
  }
    
  // Close the file (Curiously, File's destructor doesn't close the file)
  file.close();

  printFile(filename);
}

// Saves the configuration to a file
void saveDevices(const char *filename) {
  Serial.println ("Saving Devices");
  
  // Delete existing file, otherwise the configuration is appended to the file
  FS_HANDLER.remove(filename);

  // Open file for writing
  File file = FS_HANDLER.open(filename, FILE_WRITE);
  if (!file) {
    Serial.println(F("Failed to create file"));
    return;
  }

  doc.clear();

  // First store the number of devices.
  JsonObject mainObj = doc.createNestedObject();

  mainObj["numDevices"] = deviceDelegate.getDeviceCount();

  //get devices name & id for dependent list. Print for everybody. They can use with it what they will.
  // saves adding on to the condition
  for(int i = 0; i < deviceDelegate.getDeviceCount(); i++) {

    // Get the class neame for  the correct casting.
    char* className = deviceDelegate.devices[i]->getClassName();

    Serial.print ("Serializing ");
    Serial.println (className);

    JsonObject obj = doc.createNestedObject();
      
    static_cast<Storable*>(deviceDelegate.devices[i])->serialize(obj);
  }

  // Serialize JSON to file
  if (serializeJson(doc, file) == 0) {
    Serial.println(F("Failed to write to file"));
  }

  // Close the file
  file.close();

  printFile(filename);
}
