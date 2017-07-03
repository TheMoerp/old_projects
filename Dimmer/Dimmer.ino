
  #include <ESP8266WebServer.h>
  #include <ESP8266WiFi.h>
  
  ESP8266WebServer server(80); //Webserver festlegen
  const int LIGHT_PIN = 2; //Variabeln Initialisieren
  //const int INPUT_STRING_LENGTH = 5;
  const char SSID[] = "WLan-KI-Pro";
  const char PASS[] = "sVAPHCmo";
  const String HTML = "<!DOCTYPE html>" //HTML Code einbinden
  "<html>"
  "<head>"
  "<title> Lampen Dimmer </title>"
  "<meta charset = \"UTF-8\">"
  "</head>"
  "<script src = \"https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js\"></script>"
  "<meta name = \"viewport\" content = \"width=device-width, initial-scale=1\">"
  "<link rel = \"stylesheet\" href = \"https://code.jquery.com/ui/1.12.1/themes/base/jquery-ui.css\">"
  "<script src = \"https://code.jquery.com/jquery-1.12.4.js\"></script>"
  "<script src = \"https://code.jquery.com/ui/1.12.1/jquery-ui.js\"></script>"
  "<script>"
    "$( function() {"
      "$(\"#slider\").slider({min: 0, max: 1023, value:[]});"
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
    "div.header {"
      "font-family: Arial;"
      "font-size: 300%;"
      "text-align: center;"
      "background-color: #404040;"
      "color: white;"
      "padding: 3%;"
    "}"
    "div.dimDiv {"
      "border: 2px solid black; "
      "padding: 3%;"
      "margin-top: 2%;"
    "}"
    "div.discription {"
      "text-align: center;"
      "border: 1px solid black;"
      "margin-top: 10%;"
      "padding: 5%;"
      "font-size: 110%;"
      "font-family: Arial;"
    "}"
  "</style>"
  "<body>"
    "<div class = \"header\">"
      "Lamp Control"
    "</div>"
    "<div class = \"discription\">"
      "Mithilfe dieser Werkzeuge kann die Lampe an und aus geschaltet, sowie ihre Helligkeit eingestellt werden."
    "</div>"
    "<div class = \"dimDiv\" >"
      "<div id = \"on\" style = \"margin-left: 30%;\">"
        "on"
      "</div> "
      "<div id=\"off\">"
        "off"
      "</div>"
    "</div>"
    "<div class = \"dimDiv\">"
      "<div id = \"slider\" style=\"width: 50%; margin: auto; margin-top: -1%; \">"
      "</div>"
    "</div>"
  "</body>"
  "</html>";
  
void setup() { //Am Anfang aufrufen
  Serial.begin(74880);
  pinMode(LIGHT_PIN, OUTPUT);
  analogWrite(LIGHT_PIN, 0);
  wlanConfig();
  webserverInit();
  instructions();
}

void loop() { //Schleife
  //checkInput();
  server.handleClient();
}

void instructions() { //Anweisungen geben
  Serial.println("");
  Serial.println("");
  Serial.print("Verbinde dich mit dem WLAN < ");
  Serial.print(SSID);
  Serial.print(" > mit dem Passwort < ");
  Serial.print(PASS);
  Serial.println(" >");
  Serial.println("Verbinde dich, mit der IP-Adresse, mit der Lamp Control Seite");
  //Serial.println("Oder gebe in der Konsole einen Wert ein:");
}

//void checkInput() { //Gucken ob etwas in die Konsole eingegeben wurde und dann die richtigkeit dessen überprüfen
//  if(Serial.available() < INPUT_STRING_LENGTH) {
//    if(Serial.available() > 0) {
//      delay(10);
//      char inputString[INPUT_STRING_LENGTH];
//      input(inputString);
//      int value = atoi(inputString);
//      if(value >= 0 && value <= 1023) {
//        lampControl(value);
//      }
//      else {
//        Serial.println("Die Zahl muss zwischen 0 und 1023 liegen!");
//      }
//    }
//  }
//  else {
//      Serial.println("Die Zahl muss zwischen 0 und 1023 liegen!"); 
//  }
//  while(Serial.read() >= 0);
//}
//
//void input(char * inputString) { //Die in der Konsole eingegebenen Daten einlesen und übergeben
//  int inputCounter = 0;
//  for(int resetCounter = 0; resetCounter <= INPUT_STRING_LENGTH-1; resetCounter++) {
//          inputString[resetCounter] = '\0';
//  }
//  while(Serial.available() > 0, Serial.available() < INPUT_STRING_LENGTH) {
//      char inputChar = Serial.read();
//      inputString[inputCounter] = inputChar;
//      inputCounter++;
//      if(inputCounter >= INPUT_STRING_LENGTH-1) {
//        inputString[INPUT_STRING_LENGTH-1] = '\0';
//        break;
//      }
//  }
//  if(Serial.available() < INPUT_STRING_LENGTH) {  
//    inputString[inputCounter] = '\0';
//  }
//}

void wlanConfig() { //Mit dem WLAN verbinden
  Serial.println("");
  Serial.println("Verbindungsversuch ");
  Serial.print("SSID: ");
  Serial.println(SSID);
  WiFi.begin(SSID, PASS);
  int connectionCounter = 0;
  while (WiFi.status() != WL_CONNECTED && connectionCounter <= 20) {
    delay(500);
    Serial.print(".");
    connectionCounter++;
  }
  Serial.println("");
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Verbindung zum WLAN konnte nicht hergestellt werden");
  }
  else {
    Serial.println("Verbindung zum WLAN wurde hergestellt");
    IPAddress ip = WiFi.localIP();
    Serial.print("Die Ip-Adresse lautet: ");
    Serial.println(ip);
  }
}

void webserverInit() { //Webserver Seiten aufrufen und Daten entgegennehmen 
  server.on("/", []() {
    server.send(200, "text/html", HTML);
  });
  server.onNotFound([]() {
    server.send(404, "text/html", "404/Error");
  });
  server.on("/dv", []() {
    //delay(500);
    String dvString = server.arg("dv");
    int value = atoi(dvString.c_str());
    lampControl(value);
    server.send(200, "text/html", "");
  });
  server.begin();
  Serial.println("Webserver ist online");
}

void lampControl(int value) { //Helligkeitswert setzen
  Serial.println("");
  Serial.print(value);
  Serial.println(" ist jetzt die Value.");
  analogWrite(LIGHT_PIN, value);
}

