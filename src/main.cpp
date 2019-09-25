#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#include <SPIFFS.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include "FS.h"
#endif

const char *ssid = "Ceia2019";
const char *password = "87654321";

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

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
  //Iniciando la memoria SPIFFS
  SPIFFS.begin();
  //Servidor de Archivos de la memoria SPIFFS
  server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");
  //Iniciar Servidor
  server.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
}