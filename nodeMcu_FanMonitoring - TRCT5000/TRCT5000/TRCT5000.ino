/* 
 * Referências:  
https://blog.fazedores.com/sensor-optico-reflexivo-tcrt5000-com-arduino/
https://blogmasterwalkershop.com.br/arduino/arduino-utilizando-o-sensor-reflexivo-tcrt5000 
https://blog.eletrogate.com/sensor-optico-tcrt5000-com-arduino/
*/

#define pinoSensor D1

int contador = 0;

void setup() {
  Serial.begin(9600);
  pinMode(pinoSensor,INPUT);

}

void loop() {
  if(digitalRead(pinoSensor) == LOW) {
    contador++;
  }
  Serial.println(contador);
}
