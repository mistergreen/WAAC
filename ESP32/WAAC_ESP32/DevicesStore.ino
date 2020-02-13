// © 2noodles llc
// minh@2noodles.com
// Andrea Villa
// villa.andrea@gmail.com

// Compute the required size
// 25 expected fields + names with 15 characters + events (350 max chars to be calculated better) + 5 additional strings of 64 characters (servo motor example).
// The size may need to be enlarged for other objects.
static const int sDEVICES_DOC_SIZE = JSON_ARRAY_SIZE(11) + JSON_OBJECT_SIZE(1) + (JSON_OBJECT_SIZE(25) + 200 + 4 * 64);
  
// Loads the configuration from a file
void loadDevices(const char *filename) {
  Serial.println ("Loading Devices");

  // Open file for reading
  File file = FS_HANDLER.open(filename);

  // Allocate a temporary JsonDocument
  // Don't forget to change the capacity to match your requirements.
  // Use arduinojson.org/v6/assistant to compute the capacity.
  StaticJsonDocument<sDEVICES_DOC_SIZE> doc;

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
      
      // Find the right device.
      if (strcmp("ServoMotor", obj["classType"]) == 0)
      {
        Serial.println ("Creating Servo Motor");
        
        ServoMotor *temp = new ServoMotor();
        temp->deserialize(obj);
        deviceDelegate.addDevice( temp );
      }
      else if (strcmp("Relay", obj["classType"]) == 0)
      {
        Serial.println ("Creating Relay");
        
        Relay *temp = new Relay();
        temp->deserialize(obj);
        deviceDelegate.addDevice( temp );
      }
      else if (strcmp("RelayMCP", obj["classType"]) == 0)
      {
        Serial.println ("Creating RelayMCP");
        
        RelayMCP *temp = new RelayMCP();
        temp->deserialize(obj);
        deviceDelegate.addDevice( temp );
      }
      else if (strcmp("RelayPCA", obj["classType"]) == 0)
      {
        Serial.println ("Creating RelayPCA");
        
        RelayPCA *temp = new RelayPCA();
        temp->deserialize(obj);
        deviceDelegate.addDevice( temp );
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

  // Allocate a temporary JsonDocument
  // Don't forget to change the capacity to match your requirements.
  // Use arduinojson.org/assistant to compute the capacity.
  StaticJsonDocument<sDEVICES_DOC_SIZE> doc;

  // First store the number of devices.
  JsonObject mainObj = doc.createNestedObject();

  mainObj["numDevices"] = deviceDelegate.getDeviceCount();

  //get devices name & id for dependent list. Print for everybody. They can use with it what they will.
  // saves adding on to the condition
  for(int i = 0; i < deviceDelegate.getDeviceCount(); i++) {

    // Get the class neame for  the correct casting.
    char* className = deviceDelegate.devices[i]->getClassName();
    
    // Find the right device.
    if (strcmp("ServoMotor", className) == 0)
    {
      Serial.println ("Serializing Servo Motor");
      
      JsonObject obj = doc.createNestedObject();
      
      static_cast<ServoMotor*>(deviceDelegate.devices[i])->serialize(obj);
    }
    else if (strcmp("Relay", className) == 0)
    {
      Serial.println ("Serializing Relay");
      
      JsonObject obj = doc.createNestedObject();
      
      static_cast<Relay*>(deviceDelegate.devices[i])->serialize(obj);
    }
    else if (strcmp("RelayMCP", className) == 0)
    {
      Serial.println ("Serializing RelayMCP");
      
      JsonObject obj = doc.createNestedObject();
      
      static_cast<RelayMCP*>(deviceDelegate.devices[i])->serialize(obj);
    }
    else if (strcmp("RelayPCA", className) == 0)
    {
      Serial.println ("Serializing RelayPCA");
      
      JsonObject obj = doc.createNestedObject();
      
      static_cast<RelayPCA*>(deviceDelegate.devices[i])->serialize(obj);
    }
  }

  // Serialize JSON to file
  if (serializeJson(doc, file) == 0) {
    Serial.println(F("Failed to write to file"));
  }

  // Close the file
  file.close();

  printFile(filename);
}
