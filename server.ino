/*
  ESP32 Web Server / WebSocket - AP Mode
  Created on April 11 2019
  by Meshal Alhazmi @ Estidama
 
*/
 
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include <WebServer.h>
#include <WebSocketsServer.h>

// SSID & Password
const char* ssid = "Estidamah";  //  SSID Name
const char* password = "12345678";  //WiFi Password 
 
// IP Address Configuration
IPAddress local_ip(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
 
WebServer server(80);  //  WebServer defult port  80  
WebSocketsServer webSockets = WebSocketsServer(8080);//  WebSocketsServer defult port  8080  


void onWebSocketEvent(uint8_t num,WStype_t type,uint8_t * payload, size_t length){

  switch(type){
    case WStype_DISCONNECTED:
     Serial.printf("[%u] Disconnected\n", num);
     break;
    case WStype_CONNECTED:
     {
      IPAddress ip = webSockets.remoteIP(num);
      Serial.printf("[%u] connected\n", num);
      Serial.println(ip.toString());

     }
     break;

     case WStype_TEXT:
     Serial.printf("[%u] Text %s\n", num, payload);
     webSockets.sendTXT(num, payload);
     break;

    case WStype_ERROR:
    case WStype_BIN:
    case WStype_FRAGMENT_TEXT_START:
    case WStype_FRAGMENT_BIN_START:
    case WStype_FRAGMENT:
    case WStype_FRAGMENT_FIN:
    default:
      break;
  }
}
void setup() {
  Serial.begin(115200);

  
  // Create SoftAP
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
 
 
  Serial.print("Connect to My access point: ");
  Serial.println(ssid);
//  while ( WiFi.status() != WL_CONNECTED ){
//    delay(500);
//    Serial.print(".");
//    Serial.print(WiFi.status());
//  }
  server.on("/", handle_root);
 
  server.begin();
  webSockets.begin();
    server.sendHeader("Access-Control-Allow-Origin", "*");

//  server.sendHeader("Access-Control-Allow-Origin", "*");


  webSockets.onEvent(onWebSocketEvent),

  Serial.println("HTTP server started");
//  delay(100);
}
 
void loop() {
  server.handleClient();
  webSockets.loop();
}
 
// HTML & CSS contents which display on web server
String HTML = "<!DOCTYPE html>\
<html>\
<body>\
<h1>My First Web Server with ESP32 - AP Mode &#128522;</h1>\
</body>\
</html>";
 
// Handle root url (/)
void handle_root() {
  server.send(200, "text/html", HTML);
}
