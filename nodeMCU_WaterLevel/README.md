# Water Level Monitor

## Objetive
The objective of this script is to help beginners to create a web server with a sensor DHT11.

## Description:
With a NodeMCU e 3 water sensors that work on bool status you can monitor 4 status of your water resevation and view remotely on your LAN. 

## How to use
Connect all sensors to right pin, if you use less than 3 sensors you have to comment when is methion the informations about the sensor and change the logic.
If you want use the DHCP server you have to comment  <b>'WiFi.config(ip, gateway, subnet);'</b>

## How to improve this project
You can use a script to get the information from the webserver by Webscrapping and insert on your DBA. 

## Contact 
rdf.8888@gmail.com

## References
* https://www.blogdarobotica.com/2020/08/11/monitor-de-temperatura-e-umidade-com-nodemcu-esp8266-como-servidor-web/

* https://www.arduinoecia.com.br/monitore-sua-caixa-dagua-usando-arduino/
