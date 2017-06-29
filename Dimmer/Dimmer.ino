
  #include <ESP8266WebServer.h>
  #include <ESP8266WiFi.h>
  int lightPin = 2;
  char strInput[5];
  byte inputCtr = 0;
  char *strValPtr = 0;
  int preVal = 0;
  int val = 0;
  char ssid[] = "WLan-KI-Pro";
  char pass[] = "sVAPHCmo";
  int conCtr = 0;
  String html = "<!DOCTYPE html>"
"<html>"
"<head>"
"<title>Lampen Dimmer</title>"
"<meta charset=\"UTF-8\">"
"</head>"
"<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js\"></script>"
"<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
"<title>jQuery UI Slider - Default functionality</title>"
"<link rel=\"stylesheet\" href=\"https://code.jquery.com/ui/1.12.1/themes/base/jquery-ui.css\">"
"<script src=\"https://code.jquery.com/jquery-1.12.4.js\"></script>"
"<script src=\"https://code.jquery.com/ui/1.12.1/jquery-ui.js\"></script>"
"<script>"
  "$( function() {"
    "$(\"#slider\").slider({min: 0, max: 1023});"
    "$(\"#slider\").on(\"slide\","
      "function(event, ui) {"
        "$.get(\"/dv?dv=\" + ui.value);"
      "}"
    ");"
    "$(\"#on\").button().click(function() { $.get(\"/dv?dv=1023\"); });"
    "$(\"#off\").button().click(function() { $.get(\"/dv?dv=0\"); });"
  "} );"
"</script>"
"<style>"
  "header{"
    "color: white;"
    "background-color: #404040;"
    "text-align: center;"
    "font-family: verdana;"
        "font-size: 200%;"
    "}  "
    "div.middle {"
      "border: 1px solid gray;"
      "margin-top: 20px;"
      "height: 620px;"
    "}"
    "div.text {"
      "height: 600px;"
      "width: 500px;"
      "border-right: 1px solid gray;"
      "padding-left: 20px;"
      "padding-top: 20px;"
    "}"
    "normalText{"
      "font-family: verdana;"
    "}"
"</style>"
"<body>"
  "<header>"
    "<h1>Lampen Dimmer</h1>"
  "</header>"
  "<div id=\"slider\">"
  "</div>"
  "<div class=\"middle\">"
    "<div class=\"text\">"
      "<div id=\"on\">"
      "on"
    "</div>"
    "<div id=\"off\">"
      "off"
    "</div>"
      "<normalText>"
        "Hier kann man die Lampe an und aus schalten!"
      "</normalText>"
    "</div>"
  "</div>"
"</body>"
"</html>";

  ESP8266WebServer server(80);
  
void setup() {
  Serial.begin(74880);
  pinMode(lightPin, OUTPUT);
  analogWrite(lightPin, val);
  wlanCon();
  webServerInit();
  Serial.println(" ");
  Serial.println(" ");
  Serial.println("Gebe eine Zahl zwischen 1 und 1023 ein:"); 
}

void loop() {
  delay(10);
  if(Serial.available() < 5){
    if(Serial.available() > 0){
      delay(10);
      strValPtr = Input();
      preVal = atoi(strValPtr);
      if(preVal >0, preVal <=1023){
        val = preVal;
        Serial.println(val);
        Serial.println("Zahl wurde als Value uebernommen.");
      }
      else{
        Serial.println("Die Zahl muss zwischen 1 und 1023 liegen!");
      }
    }
  }
  else{
      Serial.println("Die Zahl muss zwischen 1 und 1023 liegen!"); 
  }
  while (Serial.read() >=0);
  analogWrite(lightPin, val);
  server.handleClient();
}

char * Input(){
  inputCtr=0;
  for(int i = 0; i<=4; i++){
          strInput[i]='\0';
  }
  while(Serial.available() > 0, Serial.available() < 5){
      char iChar = Serial.read();
      strInput[inputCtr]= iChar;
      inputCtr++;
      if(inputCtr>=4){
        strInput[4]='\0';
        return strInput;
      }
  }
  if(Serial.available() < 5){  
    strInput[inputCtr]='\0';
    return strInput;
  }
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
    server.send(200, "text/html", html);
  });
  server.onNotFound([](){
    server.send(200, "text/html", "404/Error");
  });
  server.on("/dv", [](){
    Serial.println("dv");
  });
  server.begin();
  Serial.println("Webserver ist online");
 
}
