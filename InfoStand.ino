
#include "config.h"
SSD1306Wire     display(I2C_DISPLAY_ADDRESS, SDA_PIN, SCL_PIN); // this is the default
ESP8266WebServer server(WEBSERVER_PORT);
httpController httpc(1);
settings sett(String("/conf.cfg"));

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  SPIFFS.begin();
  delay(10);
  
  Serial.println();
  pinMode(LED_BUILTIN, OUTPUT);

   // initialize display
  display.init();
  if (INVERT_DISPLAY) {
    display.flipScreenVertically(); // connections at top of OLED display
  }
  display.clear();
  display.display();

  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;
  
  // Uncomment for testing wifi manager
  //wifiManager.resetSettings();
  wifiManager.setAPCallback(configModeCallback);
  
  String hostname(HOSTNAME);
  hostname += String(ESP.getChipId(), HEX);
  if (!wifiManager.autoConnect((const char *)hostname.c_str())) {// new addition
    delay(3000);
    WiFi.disconnect(true);
    ESP.reset();
    delay(5000);
  }

   // print the received signal strength:
  Serial.print("Signal Strength (RSSI): ");
  Serial.print(WiFi.RSSI());
  Serial.println("db");

  void configModeCallback (WiFiManager *myWiFiManager);

  if (WEBSERVER_ENABLED) {

    //ROUTES
    server.on("/", HTTP_GET, displayIndex);
    server.on("/", HTTP_POST, displayMsg);
    server.on("/test", HTTP_GET, displayIndex);
    
    server.onNotFound(error404);
    server.begin();
    Serial.println("Server started");
  }

  display.clear();
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(ArialMT_Plain_10);
  display.drawString(64, 10, "My IP!");
  display.setFont(ArialMT_Plain_16);
  display.drawString(64, 23, WiFi.localIP().toString());
  display.display();

  Serial.print("Settings test: ");
  Serial.println(sett.read("test"));
}

void loop() {
  if (WEBSERVER_ENABLED) {
    server.handleClient();
  }
}

void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());

  display.clear();
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(ArialMT_Plain_10);
  display.drawString(64, 0, "Wifi Manager");
  display.drawString(64, 10, "Please connect to AP");
  display.setFont(ArialMT_Plain_16);
  display.drawString(64, 23, myWiFiManager->getConfigPortalSSID());
  display.setFont(ArialMT_Plain_10);
  display.drawString(64, 42, "To setup Wifi connection");
  display.display();
  
  Serial.println("Wifi Manager");
  Serial.println("Please connect to AP");
  Serial.println(myWiFiManager->getConfigPortalSSID());
  Serial.println("To setup Wifi Configuration");
}

void displayIndex(){

    digitalWrite(LED_BUILTIN, LOW);


    display.clear();
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.setFont(ArialMT_Plain_10);
    display.drawString(64, 10, "Got request from...");
    display.setFont(ArialMT_Plain_16);
    display.drawString(64, 23, server.client().remoteIP().toString());
    display.setFont(ArialMT_Plain_10);
    display.drawString(64, 43, "Hey Jolien!");
    display.display();
  
    server.sendHeader("Cache-Control", "no-cache, no-store");
    server.sendHeader("Pragma", "no-cache");
    server.sendHeader("Expires", "-1");
    server.setContentLength(CONTENT_LENGTH_UNKNOWN);
    server.send(200, "text/html", httpc.getIndex());

    

    digitalWrite(LED_BUILTIN, HIGH);
}

void displayMsg(){
  String title = server.arg("title");
  String msg1 = server.arg("msg1");
  String msg2 = server.arg("msg2");
  title.toUpperCase();

  sett.write("test", title);

  display.clear();
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(ArialMT_Plain_10);
  display.drawString(64, 0, title);
  display.drawLine(0, 12, 128, 12);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawStringMaxWidth(0, 15, 128, msg1);
  display.display();

  server.sendHeader("Cache-Control", "no-cache, no-store");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send(200, "text/html", httpc.getIndex());
}

void error404() {
  // Send them back to the Root Directory
  server.sendHeader("Cache-Control", "no-cache, no-store");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");
  server.send(404, "text/plain", "404");
  server.client().stop();
}
