#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <DHT.h>

#define DHT1PIN D1

DHT dht1(DHT1PIN, DHT11, 15);

const char* ssid = "";
const char* password = "";

// DEFINIÇÃO DE IP FIXO PARA O NODEMCU
IPAddress ip(192, 168, 111, 2);
IPAddress gateway(192, 168, 111, 1);
IPAddress subnet(255, 255, 255, 0);


ESP8266WebServer server(80);

// Serving Hello world
void getHelloArduino() {
  server.send(200, "text/json", "{\"name\": \"Hello Arduino\"}");
}

// Serving Settings
void getSettings() {
  String sensor1 = "sensor1";
  String response = "{";

  response += "\"ip\": \"" + WiFi.localIP().toString() + "\"";
  response += ",\"gw\": \"" + WiFi.gatewayIP().toString() + "\"";
  response += ",\"nm\": \"" + WiFi.subnetMask().toString() + "\"";

  if (server.arg("signalStrength") == "true") {
    response += ",\"signalStrengh\": \"" + String(WiFi.RSSI()) + "\"";
  }
  response += ",\"endpoint1\": \"" + sensor1 + "\"";

  response += "}";

  server.send(200,"text/json",response);
}

// Sensor1
void getSensor1(){
  float sensor1 = dht1.readTemperature();
  //Convertendo Float em String
  String converteSensor1 = "";
  converteSensor1.concat(sensor1);

  String local = "NoBreak";
  String ambiente = "Ambiente";
  String sensor = "Sensor1";
  String modelo = "DHT11";

  String response = "{";

  response += "\"modelo\": \"" + modelo + "\"";
  response += ",\"sensor\": \"" + sensor + "\"";
  response += ",\"local\": \"" + local  + "\"";
  response += ",\"posicao\": \"" + ambiente + "\"";
  response += ",\"medida\": \"" + converteSensor1 + "\""; 

  response += "}";

  server.send(200, "text/json",response);
}

// Define routing
void restServerRouting() {
  server.on("/", HTTP_GET, []() {
    server.send(200, F("text/html"),
                F("Para mais info sobre a API, acesse: http://{arduinoIP}/info"));
  });
  server.on(F("/hello"), HTTP_GET, getHelloArduino);
  server.on(F("/sensor1"), HTTP_GET, getSensor1);
  server.on(F("/info"), HTTP_GET, getSettings);
}

// Manage not found URL
void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void setup(void) {
  Serial.begin(115200);
  dht1.begin();
  WiFi.mode(WIFI_STA);
  WiFi.config(ip, gateway, subnet);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Activate mDNS this is used to be able to connect to the server
  // with local DNS hostmane esp8266.local
  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  // Set server routing
  restServerRouting();
  // Set not found response
  server.onNotFound(handleNotFound);
  // Start server
  server.begin();
  Serial.println("HTTP server started");
  Serial.println("Para mais info sobre a API, acesse");
  Serial.println("http://"+WiFi.localIP().toString()+"/info");
  Serial.println("Siga os endpoints diponiveis");
  Serial.println("##########");
  Serial.println("Temperatura do sensor1:");
  float t1 = dht1.readTemperature();
  Serial.println(t1);
}

void loop(void) {
  server.handleClient();
}