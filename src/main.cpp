#include <Arduino.h>
#include <ESPAsyncWebServer.h>

const char *ssid = "Ceia2019";
const char *password = "87654321";

void setup() {
  //Puerto Serial
  Serial.begin(115200);Serial.println();
  //Conectar Wifi
  WiFi.mode(WIFI_STA);WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");delay(100);
  }
  //Mostrar IP
  Serial.print("IP : ");Serial.println(WiFi.localIP());
}

void loop() {
  // put your main code here, to run repeatedly:
}