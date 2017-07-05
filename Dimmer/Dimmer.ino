
  #include <ESP8266WebServer.h>
  #include <ESP8266WiFi.h>
  
  ESP8266WebServer server(80); //Webserver wird festgelegt
  int intervalSec = 0; //Timer Zeit
  int timeCounterUntilSec = 0;
  unsigned long startTime = 0; //Zeit nachdem der Timer läuft
  int dimValue = 0; //PWM Wert
  //const int INPUT_STRING_LENGTH = 5; //Anzahl der möglichen Zeichen +1, die in die Konsole eingegeben werden können
  const int LIGHT_PIN = 2; //Pin
  const int TIME_FACTOR = 60000; //1000=Sec, 60000=Min
  const char SSID[] = "WLan-KI-Pro"; //WLAN Adresse
  const char PASS[] = "sVAPHCmo"; //WLAN Passwort
  const String HTML = "<!DOCTYPE html>"
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
    "$(document).ready(function(){" //Setzt den Slider auf den "dimValue" Wert, welcher auf dem Webserver geschpeichert ist
      "$.getJSON(\"/getInfo\", function(result){"
        "$.each(result, function(i, field){"
          "$( \"#slider\" ).slider( \"option\", \"value\", field );"
        "});"
      "});"
    "});"
    "$(function() {"
      "$(\"#slider\").slider({min: 0, max: 1023});"
      "$(\"#slider\").on(\"slide\","
        "function(event, ui) {"
          "$.get(\"/dv?dv=\" + ui.value);"
        "});"
      "$(\"#on\").button().click(function() {$( \"#slider\" ).slider( \"option\", \"value\", 1023 ); $.get(\"/dv?dv=1023\"); });"
      "$(\"#off\").button().click(function() {$( \"#slider\" ).slider( \"option\", \"value\", 0 );  $.get(\"/dv?dv=0\"); });"
      "$(\"#send\").button().click(function(){$.get(\"/timer?timer=\" + $(\"#txt\").val());});"
      "window.setInterval(function() {"
        "$.getJSON(\"/getTimer\", function(result){"
          "$.each(result, function(i, timer){"
            "$(\"#time\").text(\"\"+timer);"
          "});"
        "});"
      "}, 1000);"
      "timer.play();"
    "});"
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
      "font-size: 100%;"
      "font-family: Arial;"
    "}"
  "</style>"
  "<body>"
    "<div class = \"header\">"
      "Lamp Control"
    "</div>"
    "<div class = \"discription\">"
      "Mithilfe dieser Werkzeuge kann die Lampe an und aus geschaltet, sowie ihre Helligkeit und ein Sekunden Auschalttimer eingestellt werden."
    "</div>"
    "<div class = \"dimDiv\" >"
      "<div id = \"on\" style = \"margin-left: 30%;\">" //An Knopf
        "on"
      "</div> "
      "<div id=\"off\">" //Aus Knopf
        "off"
      "</div>"
    "</div>"
    "<div class = \"dimDiv\">"
      "<div id = \"slider\" style=\"width: 50%; margin: auto; margin-top: -1%; \">" //Dimmer Slider
      "</div>"
    "</div>"
    "<div class =\"dimDiv\">"
      "<input type=\"text\" id=\"txt\">"
      "</input>"
      "<div id=\"send\" style=\"margin-left: 2%;\">"
        "Start Timer"
      "</div>"
      "<div id=\"time\" style=\"font-family: Arial; color: #626262; background-color: #f6f6f6; border-radius: 4px; margin-top: 1.5%; border: 1px solid #acacac; text-align: center;\">"
      "</div>"
    "</div>"
    "<div id=\"div\">"
    "</div>"
  "</body>"
  "</html>"; //HTML Code wird eingebunden
  
void setup() { //Wird zu Beginn einmal aufgerufen
  Serial.begin(74880); 
  pinMode(LIGHT_PIN, OUTPUT); //Setzt den Pin Modus auf Output
  wlanConfig();
  webserverInit();
  instructions();
}

void loop() { //Dauerschleife
  //checkInput(); //Konsoleneingabe  (ist, aufgrund der Möglichkeit über die Website Werte einzugeben, unnötig)
  server.handleClient();
  execTimer(); //Timer wird ausgeführt
}

void instructions() { //Gibt die WLAN Adresse und das Passwort an und sagt was man tun muss
  Serial.println("");
  Serial.println("");
  Serial.print("Verbinde dich mit dem WLAN < ");
  Serial.print(SSID);
  Serial.print(" > mit dem Passwort < ");
  Serial.print(PASS);
  Serial.println(" >");
  Serial.println("Verbinde dich, mit der IP-Adresse, mit der Lamp Control Seite");
  //Serial.println("Oder gebe in der Konsole einen Wert ein");
  Serial.println("");
}

//void checkInput() { //Prüft ob die, in die Konsole eingegebenen Werte, mit den Richtlinien übereinstimmen
//  if(Serial.available() < INPUT_STRING_LENGTH) { //Guckt ob es nicht zu viele Zeichen sind
//    if(Serial.available() > 0) { //Guckt ob es genug Zeichen sind
//      delay(10);
//      char inputString[INPUT_STRING_LENGTH];
//      input(inputString);
//      int value = atoi(inputString);
//      if(value >= 0 && value <= 1023) { //Guckt ob die Zahl in dem möglichem Bereich liegt
//        lampControl(value);
//      }
//      else {
//        Serial.println("Die Zahl muss zwischen 0 und 1023 liegen!"); //Gibt Fehlermeldung aus
//      }
//    }
//  }
//  else {
//      Serial.println("Die Zahl muss zwischen 0 und 1023 liegen!"); //Gibt Fehlermeldung aus
//  }
//  while(Serial.read() >= 0); //Löscht den Inhalt des Eingabefeldes
//}
//
//void input(char * inputString) { //Liest den Inhalt des Eingabefeldes aus
//  int inputCounter = 0;
//  for(int resetCounter = 0; resetCounter <= INPUT_STRING_LENGTH-1; resetCounter++) { //Setzt die Counter zurück
//          inputString[resetCounter] = '\0';
//  } 
//  while(Serial.available() > 0, Serial.available() < INPUT_STRING_LENGTH) { //Wird ausgeführt solange etwas im Eingabefeld steht
//      char inputChar = Serial.read();
//      inputString[inputCounter] = inputChar;
//      inputCounter++;
//      if(inputCounter >= INPUT_STRING_LENGTH-1) { //Beendet die Funktion, wenn die Maximale Anzahl an Zeichen erfasst wurde
//        inputString[INPUT_STRING_LENGTH-1] = '\0';
//        break;
//      }
//  }
//  if(Serial.available() < INPUT_STRING_LENGTH) {  
//    inputString[inputCounter] = '\0'; //Beendet die Funktion
//  }
//}

void wlanConfig() { //Die Verbindung mit dem WLAN wird hergestellt
  Serial.println("");
  Serial.println("Verbindungsversuch ");
  Serial.print("SSID: ");
  Serial.println(SSID);
  WiFi.begin(SSID, PASS); //WLAN Verbindung wird mit der Adresse und dem Passwort aufgebaut
  int connectionCounter = 0;
  while (WiFi.status() != WL_CONNECTED && connectionCounter <= 20) { //Wartet 10 Sekunden bis die WLAN Verbindung hergestellt worden ist
    delay(500);
    Serial.print(".");
    connectionCounter++;
  }
  Serial.println("");
  if (WiFi.status() != WL_CONNECTED) { //Bricht den Verbindungsversuch ab wenn die WLAN Verbindung nicht besteht
    Serial.println("Verbindung zum WLAN konnte nicht hergestellt werden");
  }
  else { //Teilt, bei WLAN Verbindung, die IP-Adresse mit
    Serial.println("Verbindung zum WLAN wurde hergestellt");
    IPAddress ip = WiFi.localIP();
    Serial.print("Die Ip-Adresse lautet: ");
    Serial.println(ip);
  }
}

void webserverInit() { //Webserver wird Initialisiert 
  server.on("/", []() { //Zeigt die Website an
    server.send(200, "text/html", HTML);
  });
  server.onNotFound([]() { //Zeigt, beim Aufruf einer nicht verhandenen Seite, eine Fehlermeldung an
    server.send(404, "text/html", "404/Error");
  });
  server.on("/dv", []() { //Reagiert auf HTTP Get für die Lampe
    String dvString = server.arg("dv");
    dimValue = atoi(dvString.c_str());
    lampControl(dimValue);
    server.send(200, "text/html", "");  //Sendet zurück, dass er die Nachricht bekommen hat
  });
  server.on("/getInfo", []() { //Übergibt dem HTML Code die Variable "dimValue"
    String valueJson = "{\"value\":";
    valueJson += dimValue;
    valueJson += "}";
    server.send(200, "text/json", valueJson);
  });
  server.on("/getTimer", []() { //Übergibt dem HTML Code die verbliebende Timer Zeit
    String timeJson = "{\"time\":";
    timeJson += timeCounterUntilSec;
    timeJson += "}";
    server.send(200, "text/json", timeJson);
  });
  server.on("/timer", []() { //Reagiert auf HTTP Get für den Timer
    String timerString = server.arg("timer");
    int timerInt = atoi(timerString.c_str());
    setTimer(timerInt);
    server.send(200, "text/html", "");
  });
  server.begin(); //Webserver wird gestartet
  Serial.println("Webserver ist online");
}

void lampControl(int value) { //Schaltet die Lampe mit dem Wert "Value" ein
  Serial.println("");
  Serial.print("Die Value wurde auf ");
  Serial.print(value);
  Serial.print(" gesetzt.");
  if(timeCounterUntilSec > 0) {
    Serial.print(" Noch ");
    Serial.print(timeCounterUntilSec);
    Serial.print(" Sekunden bis die Lampe ausgeschaltet wird.");
  }
  Serial.println("");
  analogWrite(LIGHT_PIN, value);
}

void setTimer(int offTimerSec) { //Die Timer Zeit sowie die Start Zeit wird gesetzt
  intervalSec = offTimerSec;
  Serial.println("");
  Serial.print("Der Timer wurde auf ");
  Serial.print(offTimerSec);
  Serial.println(" Sekunden gesetzt.");
  startTime = millis();
}

void execTimer() { //Der Timer wird ausgeführt
  if(intervalSec != 0) {
    unsigned long intervalMillis = intervalSec * TIME_FACTOR;
    unsigned long currentMillis = millis();
    static unsigned long timeCounterUntilMillis = 0;
    unsigned long timeCounter = currentMillis - startTime;
    timeCounterUntilMillis = intervalMillis - timeCounter;
    timeCounterUntilSec = timeCounterUntilMillis / TIME_FACTOR; //Restliche Zeit wird ausgerechnet
    if(timeCounter >= intervalMillis) { //Guckt ob die Zeit abgelaufen ist
      Serial.println("");
      Serial.println("Der Timer ist abgelaufen.");
      lampControl(0); //Lampe wird ausgeschaltet
      intervalSec = 0;
    }
  }
}

