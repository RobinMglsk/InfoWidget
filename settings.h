#pragma once
#include <ESP8266WiFi.h>
#include "FS.h"

class settings {
  
  private:
    String myConfig = "/config.cfg";
    void createConfig();
    
  public:
    settings(String config);
    String read(String id);
    void write(String id, String value);
    String ids();
    
};
