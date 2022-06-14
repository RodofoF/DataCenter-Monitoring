//Bibliotecas
#include <Wire.h>
#include<ESP8266WiFi.h> 
#include<ESP8266WebServer.h> 

//Informações de acesso ao WI-FI
const char* ssid = "";  
const char* password = "";  


//Pinos ESP8266
#define sensor1 D0 
#define sensor2 D1           
#define sensor3 D2           

//Varialvel com valor inicial
int valor_s1 = 0;
int valor_s2 = 0;
int valor_s3 = 0;

int nivel_anterior = 0;

char cheio = cheio;

//DEFINIÇÃO DE IP FIXO PARA O NODEMCU
IPAddress ip(192,168,111,2);  
IPAddress gateway(192,168,111,1); 
IPAddress subnet(255,255,255,0); 

ESP8266WebServer server(80); //server na porta 80

void setup() {

  Serial.begin(9600);
  delay(50); 

  //Define os pinos dos sensores como entrada
  pinMode(sensor1, INPUT);
  pinMode(sensor2, INPUT);
  pinMode(sensor3, INPUT);

  //Mensagem serial
  Serial.println("Monitoracao de Caixa Dagua");
  Serial.println();

  Serial.println("Conectando a Rede: "); 
  Serial.println(ssid); 

  WiFi.config(ip, gateway, subnet);
  WiFi.begin(ssid, password, false); 

  //Verificação da conexão
  while (WiFi.status() != WL_CONNECTED) { 
    delay(1000);
    Serial.print("."); 
  }
  
  //Exibe informações de conexão via serial
  Serial.println("");
  Serial.println("WiFi Conectado");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP()); 

  Serial.print("Subnet Mask: ");
  Serial.println(WiFi.subnetMask());
  Serial.print("MAC Address: ");
  Serial.println(WiFi.macAddress());
  Serial.print("RSSI: ");
  Serial.println(WiFi.RSSI());

  WiFi.softAPdisconnect (true); //Desconecta Access Point

  server.on("/", handle_OnConnect); //Servidor recebe uma solicitação HTTP - chama a função handle_OnConnect
  server.onNotFound(handle_NotFound); //Servidor recebe uma solicitação HTTP não especificada - chama a função handle_NotFound

  //Inicializa o servidor
  server.begin(); 
  Serial.println("Servidor HTTP inicializado");
  
}

void loop(){
  server.handleClient(); 
  delay(500);  
}

void handle_OnConnect() {
  //Leitura dos sensores: indica 1 quando esta vazio
  valor_s1 = digitalRead(sensor1); 
  valor_s2 = digitalRead(sensor2);
  valor_s3 = digitalRead(sensor3);
  
  //Mostra valor dos sensores no serial monitor
  Serial.print("S1: ");
  Serial.print(valor_s1);
  Serial.print(" S2: ");
  Serial.print(valor_s2);
  Serial.print(" S3: ");
  Serial.println(valor_s3);
  delay(1000);
  

  String estado;

  /*Lógica de programação
   * S1  S2  S3  Status
   * 0   0   0   Full
   * 0   0   1   Full
   * 0   1   0   Full
   * 0   1   1   Full
   * 1   0   0   half
   * 1   0   1   half
   * 1   1   0   Attention
   * 1   1   1   Empty
   * */
  

 
           if(valor_s1 == 0 && valor_s2 == 0 && valor_s3 == 0){
            Serial.print("Cheio\n");
            estado = "Full";
            }
            
            if(valor_s1 == 0 && valor_s2 == 0 && valor_s3 == 1){
            Serial.print("Cheio\n");
            estado = "Full";
            }
          
            if(valor_s1 == 0 && valor_s2 == 1 && valor_s3 == 0){
            Serial.print("Cheio\n");
            estado = "Full";
            }
          
            if(valor_s1 == 0 && valor_s2 == 1 && valor_s3 == 1){
            Serial.print("Cheio\n");
            estado = "Full";
            }

            if(valor_s1 == 1 && valor_s2 == 0 && valor_s3 == 0){
            Serial.print("Meio\n");
            estado = "Half";
            }

            if(valor_s1 == 1 && valor_s2 == 0 && valor_s3 == 1){
            Serial.print("Meio\n");
            estado = "Half";
            }

            if(valor_s1 == 1 && valor_s2 == 1 && valor_s3 == 0){
            Serial.print("Reserva\n");
            estado = "Attention";
            }

            if(valor_s1 == 1 && valor_s2 == 1 && valor_s3 == 1){
            Serial.print("Vazio\n");
            estado = "Empty";
            }
            if(valor_s1 == 0){
            Serial.print("Cheio\n");
            estado = "Full";
            }
            if(valor_s1 == 1){
            Serial.print("Cheio\n");
            estado = "Empty";
            }
  
  server.send(200, "text/html", EnvioHTML(estado)); 
  delay(5000);
}

void handle_NotFound() { 
  server.send(404, "text/plain", "Não encontrado"); 
}

String EnvioHTML(String estado) { 
  String ptr = "<!DOCTYPE html> <html>\n"; 
  ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n"; 
  ptr += "<meta http-equiv='refresh' content='2'>";//Atualizar browser a cada 2 segundos
  ptr += "<link href=\"https://fonts.googleapis.com/css?family=Open+Sans:300,400,600\" rel=\"stylesheet\">\n";
  ptr += "<title>Sensor de Nivel de agua</title>\n"; //Define o título da página
  ptr += "<style>html { font-family: 'Open Sans', sans-serif; display: block; margin: 0px auto; text-align: center;color: #000000;}\n";
  ptr += "body{margin-top: 50px;}\n";
  ptr += "h1 {margin: 50px auto 30px;}\n";
  ptr += "h2 {margin: 40px auto 20px;}\n";
  ptr += "p {font-size: 24px;color: #000000;margin-bottom: 10px;}\n";
  ptr += "</style>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";
  ptr += "<div id=\"webpage\">\n";
  ptr += "<h1>Sensor de Nivel d agua</h1>\n";
  ptr += "<h2></h2>\n";

  //Exibe as informações de temperatura e umidade na página web
  ptr += "<p><b>Status: </b>";
  ptr += (String)estado;
  ptr += "</p>"; 
  
  ptr += "</div>\n";
  ptr += "</body>\n";
  ptr += "</html>\n";
  return ptr;

}
