#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESPAsyncWebServer.h>
#ifdef ESP32
boolean High = true;
boolean Low = !High;
#include <WiFi.h>
#include <AsyncTCP.h>
#include <SPIFFS.h>
extern "C"{uint8_t temprature_sens_read();}
uint8_t temprature_sens_read();
double getTemp(){
  return (temprature_sens_read() - 32) / 1.8;
}
int getLumenes(){
  return analogRead(36);
}
#elif defined(ESP8266)
boolean High = false;
boolean Low = !High;
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include "FS.h"
double getTemp(){
  return (random(3000, 3200))/100.0;
}
int getLumenes(){  
  return map(analogRead(0), 0, 1023, 0, 4095);
}
#endif

const char *ssid = "Ceia2019";
const char *password = "87654321";

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

int lumenes = 0;
double temperatura = 0.0;

void onWsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len){
  
  if(type == WS_EVT_CONNECT){
    Serial.println("Client connected");    
  } else if(type == WS_EVT_DISCONNECT){
    Serial.println("Client disconnected");
  } else if(type == WS_EVT_DATA){
    Serial.println("Client Recive Data");
  }
}

void setup() {
  //Encender indicardor
  pinMode(2, OUTPUT);
  digitalWrite(2, High);
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
  //Agregando los eventos del WebSocket al Servidor
  ws.onEvent(onWsEvent);
  server.addHandler(&ws);
  //Iniciar Servidor
  server.begin();
  //Apagar indicador OK
  digitalWrite(2, Low);
}

void loop() {
  // Recolectar promedios de punto medio los datos
  lumenes = (lumenes + getLumenes())/2;
  temperatura = (temperatura + getTemp())/2.0;

  // Crear salida en formato JSON
  String text = "";//respuesta json del websocket
  StaticJsonDocument<256> doc;
  JsonObject root = doc.to<JsonObject>();
  root["lumenes"] = lumenes;
  root["temperatura"] = String(temperatura, 2);
  serializeJson(doc, text);

  //Enviar por el WebSocket
  ws.textAll(text.c_str());
  
  //Pausar la ejecucion por 1s
  delay(500);
}