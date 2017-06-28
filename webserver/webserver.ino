#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>


char ssid[] = "WLan-KI-Pro";
char pass[] = "sVAPHCmo";
int conCtr = 0;
ESP8266WebServer server(80);


void setup() {
  Serial.begin(74880);
  delay(100);
  Serial.println(" ");
  wlanCon();
  webServerInit();
}

void loop() {
  server.handleClient();
}

void wlanCon(){
  Serial.println("Verbindungsversuch ");
  Serial.print("SSID: ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  conCtr = 0;
  while (WiFi.status() != WL_CONNECTED && conCtr <=20) {
    delay(500);
    Serial.print(".");
    conCtr++;
  }
  Serial.println("");
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Verbindung zum WLAN konnte nicht hergestellt werden");
  }
  else {
    Serial.println("Verbindung zum WLAN wurde hergestellt");
    IPAddress ip = WiFi.localIP();
    Serial.print("Die Ip-Addresse lautet: ");
    Serial.println(ip);
  }
}

void webServerInit(){
  server.on("/", [](){
    server.send(200, "text/html", "Hallo");
  });
  server.onNotFound([](){
    server.send(200, "text/html", "404/Error");
  });
  server.begin();
  Serial.println("Webserver ist online");
 
}


  
  

  
