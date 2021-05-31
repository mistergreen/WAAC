// © 2noodles llc
// minh@2noodles.com
// Andrea Villa
// villa.andrea@gmail.com

// Compute the required size
static const int sCONFIG_DOC_SIZE = JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(32) + 496;

// Loads the configuration from a file
bool loadConfiguration(const char *filename) {

  // The return value.
  bool retVal = false;
  
  // Open file for reading
  File file = FS_HANDLER.open(filename);

  // Allocate a temporary JsonDocument
  // Don't forget to change the capacity to match your requirements.
  // Use arduinojson.org/v6/assistant to compute the capacity.
  StaticJsonDocument<sCONFIG_DOC_SIZE> doc;

  // Deserialize the JSON document
  DeserializationError error = deserializeJson(doc, file);
  
  if (error){
    Serial.println(F("Failed to read file, using default configuration"));
    Serial.println(error.c_str());
  }
  else {
    Serial.println(F("Deserializing configuration"));
    // Get the JSON object.
    JsonObject obj = doc[0];
    wwws->deserialize(obj);
    retVal = true;
  }
  
  // Close the file (Curiously, File's destructor doesn't close the file)
  file.close();
  
  printFile(filename);

  return retVal;
}

// Saves the configuration to a file
void saveConfiguration(const char *filename) {
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
  StaticJsonDocument<sCONFIG_DOC_SIZE> doc;
  
  JsonObject obj = doc.createNestedObject();
  wwws->serialize(obj);

  // Serialize JSON to file
  if (serializeJson(doc, file) == 0) {
    Serial.println(F("Failed to write to file"));
  }
  
  // Close the file
  file.close();

  printFile(filename);
}

// Prints the content of a file to the Serial
void printFile(const char *filename) {
  // Open file for reading
  File file = FS_HANDLER.open(filename);
  if (!file) {
    Serial.println(F("Failed to read file"));
    return;
  }

  // Extract each characters by one by one
  while (file.available()) {
    Serial.print((char)file.read());
  }
  Serial.println();

  // Close the file
  file.close();
}
