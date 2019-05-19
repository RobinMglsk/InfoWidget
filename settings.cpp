#include "settings.h";

settings::settings(String config){
  myConfig = config;
  SPIFFS.begin();
}

String settings::read(String id){
  createConfig();
  File f = SPIFFS.open(myConfig, "r");
  String line;
  while(f.available()) {
     line = f.readStringUntil('\n');
     if (line.indexOf(id+"=") >= 0) {
      f.close();
      return line.substring(line.lastIndexOf(id+"=") + (id.length()+1));
     }
  }
  f.close();
  return "";
}

void settings::write(String id,String value){
  createConfig();
  File f = SPIFFS.open(myConfig, "w");
  f.println(id+"="+value);
  f.close();
  Serial.print("Wrote value "+value+" for "+id);
}

String settings::ids(){}

void settings::createConfig(){
    if (!SPIFFS.exists(myConfig)) {
      Serial.println("Settings File does not yet exists.");
      File f = SPIFFS.open(myConfig, "w");
      if (!f) {
          Serial.println("Failed to create file "+myConfig);
      }
      f.println("Settings=" + myConfig);
      f.close();
    }
}
