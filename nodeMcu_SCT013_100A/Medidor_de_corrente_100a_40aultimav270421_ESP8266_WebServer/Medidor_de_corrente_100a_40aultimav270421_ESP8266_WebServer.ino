
//Bibliotecas
#include "EmonLib.h"
#include<ESP8266WiFi.h> 
#include<ESP8266WebServer.h> 

const char* ssid = "";  // Rede WiFi
const char* password = "";  //Senha da Rede WiFi

ESP8266WebServer server(80); //server na porta 80

EnergyMonitor SCT013;
int pinSCT = A0;   //Pino analógico conectado ao SCT-013
int tensao = 127;
int potencia;

//DEFINIÇÃO DE IP FIXO PARA O NODEMCU
IPAddress ip(192,168,111,2); //COLOQUE UMA FAIXA DE IP DISPONÍVEL DO SEU ROTEADOR. 
IPAddress gateway(192,168,111,1); //GATEWAY DE CONEXÃO 
IPAddress subnet(255,255,255,0); //MASCARA DE REDE

void setup(){
    SCT013.current(pinSCT, 35.36);
    Serial.begin(9600);

  Serial.println("Monitoracao de Corrente");
  Serial.println();

  Serial.println("Conectando a Rede: "); //Imprime na serial a mensagem
  Serial.println(ssid); //Imprime na serial o nome da Rede Wi-Fi

  //WiFi.config(ip, gateway, subnet);
  WiFi.begin(ssid, password, false); //Inicialização da comunicação Wi-Fi

  //Verificação da conexão
  while (WiFi.status() != WL_CONNECTED) { //Enquanto estiver aguardando status da conexão
    delay(1000);
    Serial.print("."); //Imprime pontos
  
  }

  Serial.println("");
  Serial.println("WiFi Conectado");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP()); //Função para exibir o IP da ESP

  Serial.print("Subnet Mask: ");
  Serial.println(WiFi.subnetMask());
  Serial.print("MAC Address: ");
  Serial.println(WiFi.macAddress());
  Serial.print("RSSI: ");
  Serial.println(WiFi.RSSI());

  WiFi.softAPdisconnect (true); // Desabilita o Access Point

  server.on("/", handle_OnConnect); //Servidor recebe uma solicitação HTTP - chama a função handle_OnConnect
  server.onNotFound(handle_NotFound); //Servidor recebe uma solicitação HTTP não especificada - chama a função handle_NotFound

  server.begin(); //Inicializa o servidor
  Serial.println("Servidor HTTP inicializado");
 
}

void loop(){
  server.handleClient(); //Chama o método handleClient()
  //delay(2000);  
} 

void handle_OnConnect(){
    double Irms = SCT013.calcIrms(1480);   // Calcula o valor da Corrente
    
    potencia = Irms * tensao;          // Calcula o valor da Potencia Instantanea    

        String Status;

  if (Irms>2) {

    Status = "LIGADO";
    Serial.print("LIGADO");
    Serial.print("\n");
  }

  else {
    Status = "DESLIGADO";
    Serial.print("DESLIGADO");
    Serial.print("\n");  
  }
  
    Serial.print("Corrente = ");
    Serial.print(Irms);
    Serial.println(" A");
    
    Serial.print("Potencia = ");
    Serial.print(potencia);
    Serial.println(" W");
   
    
    Serial.print(".");
   
    Serial.print(".");
    
    Serial.println(".");
    

 
   
  server.send(200, "text/html", EnvioHTML(Irms, Status)); //Envia as informações usando o código 200, especifica o conteúdo como "text/html" e chama a função EnvioHTML
  delay(2000);
}

void handle_NotFound() { //Função para lidar com o erro 404
  server.send(404, "text/plain", "Não encontrado"); //Envia o código 404, especifica o conteúdo como "text/pain" e envia a mensagem "Não encontrado"
}

String EnvioHTML(double Irms, String Status) { //Exibindo a página da web em HTML
  String ptr = "<!DOCTYPE html> <html>\n"; //Indica o envio do código HTML
  ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n"; //Torna a página da Web responsiva em qualquer navegador Web
  ptr += "<meta http-equiv='refresh' content='2'>";//Atualizar browser a cada 2 segundos
  ptr += "<link href=\"https://fonts.googleapis.com/css?family=Open+Sans:300,400,600\" rel=\"stylesheet\">\n";
  ptr += "<title>Monitoracao de corrente - Fase R</title>\n"; //Define o título da página

  //Configurações de fonte do título e do corpo do texto da página web
  ptr += "<style>html { font-family: 'Open Sans', sans-serif; display: block; margin: 0px auto; text-align: center;color: #000000;}\n";
  ptr += "body{margin-top: 50px;}\n";
  ptr += "h1 {margin: 50px auto 30px;}\n";
  ptr += "h2 {margin: 40px auto 20px;}\n";
  ptr += "p {font-size: 24px;color: #000000;margin-bottom: 10px;}\n";
  ptr += "</style>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";
  ptr += "<div id=\"webpage\">\n";
  ptr += "<h1>Monitoracao de corrente</h1>\n";
  ptr += "<h2></h2>\n";

  //Exibe as informações de temperatura e umidade na página web
  ptr += "<p><b>Status: </b>";
  ptr += (String)Status;
  ptr += "  - </p>"; // informação após a varialvel
  ptr += "<p><b>Medicao: </b>";
  ptr += (double)Irms;
  ptr += " - A</p>"; // informação após a varialvel


  ptr += "</div>\n";
  ptr += "</body>\n";
  ptr += "</html>\n";
  return ptr;

}
