// © 2noodles llc
// minh@2noodles.com
// Andrea Villa
// villa.andrea@gmail.com

// Compute the required size
static const int sCONFIG_DOC_SIZE = 3*JSON_ARRAY_SIZE(4) + JSON_OBJECT_SIZE(7) + 256;
  
// Loads the configuration from a file
void loadConfiguration(const char *filename, Config &config) {
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
    // Copy values from the JsonDocument to the Config
    config.timeZone = doc["timeZone"];
  
    strlcpy(config.ssid,              // <- destination
            doc["ssid"],              // <- source
            sizeof(config.ssid));     // <- destination's capacity

    strlcpy(config.password,              // <- destination
            doc["password"],              // <- source
            sizeof(config.password));     // <- destination's capacity

    strlcpy(config.ntpServer,              // <- destination
            doc["ntpServer"],              // <- source
            sizeof(config.ntpServer));     // <- destination's capacity

    // Get a reference to the root array
    JsonArray ip = doc["ip"].as<JsonArray>();
    
    config.ip[0] = (int)ip[0];
    config.ip[1] = (int)ip[1];
    config.ip[2] = (int)ip[2];
    config.ip[3] = (int)ip[3];

    // Get a reference to the root array
    JsonArray gateway = doc["gateway"].as<JsonArray>();

    config.gateway[0] = (int)gateway[0];
    config.gateway[1] = (int)gateway[1];
    config.gateway[2] = (int)gateway[2];
    config.gateway[3] = (int)gateway[3];

    // Get a reference to the root array
    JsonArray subnet = doc["subnet"].as<JsonArray>();
    
    config.subnet[0] = (int)subnet[0];
    config.subnet[1] = (int)subnet[1];
    config.subnet[2] = (int)subnet[2];
    config.subnet[3] = (int)subnet[3];
  }
  
  // Close the file (Curiously, File's destructor doesn't close the file)
  file.close();
  
  printFile(filename);
}

// Saves the configuration to a file
void saveConfiguration(const char *filename, const Config &config) {
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

  // Set the values in the document
  doc["ssid"] = config.ssid;
  doc["password"] = config.password;
  doc["ntpServer"] = config.ntpServer;
  doc["timeZone"] = config.timeZone;

  JsonArray ip = doc["ip"];
  ip.add(config.ip[0]);
  ip.add(config.ip[1]);
  ip.add(config.ip[2]);
  ip.add(config.ip[3]);
  
  JsonArray gateway = doc["gateway"];
  gateway.add(config.gateway[0]);
  gateway.add(config.gateway[1]);
  gateway.add(config.gateway[2]);
  gateway.add(config.gateway[3]);
  
  JsonArray subnet = doc["subnet"];
  subnet.add(config.subnet[0]);
  subnet.add(config.subnet[1]);
  subnet.add(config.subnet[2]);
  subnet.add(config.subnet[3]);

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
