/* 
Programação para 4 sensores DHT11 para a placa de NODEMCU ESP8266
com visualização no WebServer
*/


#include<ESP8266WiFi.h> //Biblioteca para funcionamento do WiFi do ESP8266
#include<ESP8266WebServer.h> //Biblioteca para funcionamento do WebServer
#include <DHT.h> //Biblioteca para sensores DHT

const char* ssid = "";  // Nome da rede WiFi
const char* password = "";  //Senha da Rede WiFi
const char* Channel = "1";
const char* bssid;
const char* Connect = "false";

//DEFINIÇÃO DE IP FIXO PARA O NODEMCU
IPAddress ip(192, 168, 111, 2); 
IPAddress gateway(192, 168, 111, 1); 
IPAddress subnet(255, 255, 255, 0); 

ESP8266WebServer server(80); //Porta do WebServer

// Definição dos pinos
#define DHT1PIN D0
#define DHT2PIN D2 
#define DHT3PIN D3
#define DHT4PIN D4 

// Outra forma de definir os pinos
//#define DHT1PIN 5
//#define DHT2PIN 4
//#define DHT3PIN 0
//#define DHT4PIN 2

// Definição do sensor utilizado.
DHT dht1(DHT1PIN, DHT11, 15); 
DHT dht2(DHT2PIN, DHT11, 15); 
DHT dht3(DHT3PIN, DHT11, 15); 
DHT dht4(DHT4PIN, DHT11, 15); 


void setup() {
  //Inicializa a comunicação serial
  Serial.begin(9600); 
  delay(50); 

  dht1.begin();
  dht2.begin();
  dht3.begin();
  dht4.begin();

//Inicialização da comunicação Wi-Fi
  Serial.println("Conectando a Rede: "); 
  Serial.println(ssid); 

  WiFi.config(ip, gateway, subnet);
  WiFi.begin(ssid, password, false); 

  //Verificação da conexão
  while (WiFi.status() != WL_CONNECTED) { 
    delay(1000);
    Serial.print("."); 
  }

  Serial.println("");
  Serial.println("WiFi Conectado");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP()); //Função para exibir o IP da ESP8266

  Serial.print("Subnet Mask: ");
  Serial.println(WiFi.subnetMask());
  Serial.print("MAC Address: ");
  Serial.println(WiFi.macAddress());
  Serial.print("RSSI: ");
  Serial.println(WiFi.RSSI());

  WiFi.softAPdisconnect (true); // Função para desabilitar o Acess Point

  server.on("/", handle_OnConnect); //Servidor recebe uma solicitação HTTP - chama a função handle_OnConnect
  server.onNotFound(handle_NotFound); //Servidor recebe uma solicitação HTTP não especificada - chama a função handle_NotFound

  //Inicializa o servidor
  server.begin(); 
  Serial.println("Servidor HTTP inicializado");

}

void loop() {
  //Chama o método handleClient()
  server.handleClient(); 
  delay(2000);

}

void handle_OnConnect() {
  //Armazena as informações de leitura na variável e executa Serial.print para leitura serial
  float h1 = dht1.readHumidity();
  float t1 = dht1.readTemperature();
  Serial.print("**********\n");
  Serial.print("Temperatura 1: ");
  Serial.print(t1);
  Serial.println(" ºC");
  Serial.print("Umidade 1: ");
  Serial.print(h1); 
  Serial.println(" %");
  Serial.print("********** \n");
  
  
  float h2 = dht2.readHumidity();
  float t2 = dht2.readTemperature();
  Serial.print("**********\n");
  Serial.print("Temperatura 2: ");
  Serial.print(t2); 
  Serial.println(" ºC");
  Serial.print("Umidade 2: ");
  Serial.print(h2);
  Serial.println(" %");
  Serial.print("********** \n");
  

  float h3 = dht3.readHumidity();
  float t3 = dht3.readTemperature();
  Serial.print("********** \n");
  Serial.print("Temperatura 3: ");
  Serial.print(t3); 
  Serial.println(" ºC");
  Serial.print("Umidade 3: ");
  Serial.print(h3); 
  Serial.println(" %");
  Serial.print("**********\n");
 
 
  float h4 = dht4.readHumidity();
  float t4 = dht4.readTemperature();
  Serial.print("********** \n");
  Serial.print("Temperatura 4: ");
  Serial.print(t4); 
  Serial.println(" ºC");
  Serial.print("Umidade 4: ");
  Serial.print(h4); 
  Serial.println(" %");
  Serial.print("********** \n");

  delay(5000);
  

  server.send(200, "text/html", EnvioHTML(t1, t2, t3, t4)); //Envia as informações usando o código 200, especifica o conteúdo como "text/html" e chama a função EnvioHTML
  
  delay(5000);
}

void handle_NotFound() { //Função para lidar com o erro 404
  server.send(404, "text/plain", "Não encontrado"); //Envia o código 404, especifica o conteúdo como "text/pain" e envia a mensagem "Não encontrado"

}

String EnvioHTML(float t1, float t2, float t3, float t4) { 
  String ptr = "<!DOCTYPE html> <html>\n"; 
  ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n"; 
  ptr += "<meta http-equiv='refresh' content='2'>";//Atualizar browser a cada 2 segundos
  ptr += "<link href=\"https://fonts.googleapis.com/css?family=Open+Sans:300,400,600\" rel=\"stylesheet\">\n";
  ptr += "<title>Monitor de temperatura</title>\n"; //Define o título da página
  ptr += "</head>";
  ptr += "<body>\n";
  
  ptr += "<h1><u>Monitor de temperatura</u></h1>\n";
  ptr += "<h2></h2>\n";
  ptr += "<table>";
  ptr += "<th>&nbsp;</th>";
  ptr += "<tr>";
  ptr += "<td>";
  ptr += "<h1 align=center Sensor>";
  ptr += "<p><b>Sensor1: </b>";
  ptr += (float)t1;
  ptr += "</td>";
  ptr += "</tr>";
  ptr += "<th>&nbsp;</th>";
  ptr += "<tr>";
  ptr += "<td>";
  ptr += "<h1 align=center Sensor>"; 
  ptr += "<p><b>Sensor2: </b>";
  ptr += (float)t2;
  ptr += "</p>"; 
  ptr += "</td>";
  ptr += "</tr>";
  ptr += "<th>&nbsp;</th>";
  ptr += "<tr>";
  ptr += "<td>";
  ptr += "<h1 align=center Sensor>";
  ptr += "<p><b>Sensor3: </b>";
  ptr += (float)t3;
  ptr += "</td>";
  ptr += "</tr>";
  ptr += "<th>&nbsp;</th>";
  ptr += "<tr>";
  ptr += "<td>";
  ptr += "<h1 align=center Sensor>"; 
  ptr += "<p><b>Sensor4: </b>";
  ptr += (float)t4;
  ptr += "</p>"; 
  ptr += "</td>";
  ptr += "</tr>";
  ptr += "</body>\n";
  ptr += "</html>\n";
  return ptr;

}
