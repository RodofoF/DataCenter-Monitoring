#include<ESP8266WiFi.h>
#include<ESP8266WebServer.h>

#define buzzer D1
#define sensor D0

const char* ssid = "";
const char* password = "";

IPAddress ip(192, 168, 111, 2);
IPAddress gateway(192, 168, 111, 1);
IPAddress subnet(255, 255, 255, 0);

ESP8266WebServer server(80);


int verifica[50];
String statusMotor = "Desligado";

void setup () {
  Serial.begin(9600);
  pinMode(buzzer, OUTPUT);

  
//Inicialização da comunicação Wi-Fi
  Serial.println("Conectando a Rede: "); 
  Serial.println(ssid); 

  //WiFi.config(ip, gateway, subnet);
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

void loop () {
  //Chama o método handleClient()
  server.handleClient(); 
  delay(2000);

}

void handle_OnConnect() {
  int motor = digitalRead(sensor);
  statusMotor = "Desligado";
  Serial.println(statusMotor);
  digitalWrite(buzzer, LOW);
  //delay(500);

  if (motor != 0) {
    for (int i = 0; i < 50; i++) {
      verifica[i] = motor;
      statusMotor = "Ligado";
      digitalWrite(buzzer, HIGH);
      Serial.println(statusMotor);
      //Serial.println(verifica[i]);
      delay(100);
    }
  }
  //delay(1000);
  

  server.send(200, "text/html", EnvioHTML(statusMotor)); //Envia as informações usando o código 200, especifica o conteúdo como "text/html" e chama a função EnvioHTML
  
  delay(100);
  
}
void handle_NotFound() { //Função para lidar com o erro 404
  server.send(404, "text/plain", "Não encontrado"); //Envia o código 404, especifica o conteúdo como "text/pain" e envia a mensagem "Não encontrado"

}

String EnvioHTML(String statusMotor) { 
  String ptr = "<!DOCTYPE html> <html>\n"; 
  ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n"; 
  ptr += "<meta http-equiv='refresh' content='1'>";//Atualizar browser a cada 2 segundos
  ptr += "<link href=\"https://fonts.googleapis.com/css?family=Open+Sans:300,400,600\" rel=\"stylesheet\">\n";
  ptr += "<title>Monitor de acionamento do GMG 1</title>\n"; //Define o título da página
  ptr += "</head>";
  ptr += "<body>\n";
  
  ptr += "<h1><u>Monitor de acionamento do GMG 1</u></h1>\n";
  ptr += "<h2></h2>\n";
  ptr += "<table>";
  ptr += "<th>&nbsp;</th>";
  ptr += "<tr>";
  ptr += "<td>";
  ptr += "<h1 align=center Sensor>";
  ptr += "<p><b>GMG 1: </b>";
  ptr += (String)statusMotor;
  ptr += "</td>";
  ptr += "</tr>";
  ptr += "</body>\n";
  ptr += "</html>\n";
  return ptr;

}
