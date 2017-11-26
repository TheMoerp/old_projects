
  #include <ESP8266WebServer.h>
  #include <ESP8266WiFi.h>
  ESP8266WebServer server(80); //Webserver wird festgelegt
  
  //Änderbare Variablen
  const int LIGHT_PIN = 2; //Nummer des Pins
  const int MAX_WLAN_CON_TRYS = 3; //Maximale WLAN Verbindungsversuch Anzahl
  const char SSID[] = "TV_WLAN"; //WLAN Adresse
  const char PASS[] = "geheim"; //WLAN Passwort
  const String TIME_UNIT = "Minuten"; //Timer: Sekunden / Minuten
  const unsigned long DIMMER_INTERVAL_MS = 0.1; //Dim Geschwindigkeit
  const unsigned long HEART_BEAT_MS = 60000;
  
  //Nicht änderbare Variablen
  boolean consoleStatus = false;
  int intervalTimeUnit = 0; //Timer Zeit
  int timerTimeTimeUnit = 0; //Zeigt die übrige Timerzeit an
  int dimValue = 0; //PWM Wert
  int lampValue = 0;
  int timeFactor = 0;
  unsigned long startTimeMs = 0; //Zeit nachdem der Timer läuft
  const int INPUT_STRING_LENGTH = 5; //Anzahl der möglichen Zeichen +1, die in die Konsole eingegeben werden können
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
      "}, 100);"
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
      "border-radius: 3px"
    "}"
    "div.dimDiv {"
      "border: 2px solid #404040; "
      "padding: 3%;"
      "margin-top: 2%;"
      "border-radius: 3px"
    "}"
    "div.discription {"
      "text-align: center;"
      "border: 1px solid #404040;"
      "border-radius: 3px"
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
      "Mithilfe dieser Werkzeuge kann die Lampe an und aus geschaltet, sowie ihre Helligkeit und einen Auschalttimer eingestellt werden."
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
  checkTimeFactor();
  wlanConfig();
  if(WiFi.status() != WL_CONNECTED) {
  } else {
    webserverInit();
    instructions();
  }
}

void loop() { //Dauerschleife
  if(consoleStatus == true) { //Konsoleneingabe wird bei fehlender WLAN Verbindung aktiviert
    checkInput();
  }
  server.handleClient();
  execTimer(); //Timer wird ausgeführt
  execLampValue();
  checkWlan();
  heartBeat();
}

void checkTimeFactor() {
  if(TIME_UNIT == "Sekunden") {
    timeFactor = 1000;
  }
  if(TIME_UNIT == "Minuten") {
    timeFactor = 60000;
  } else {
    Serial.print("Der Zeitfaktor ");
    Serial.print(TIME_UNIT);
    Serial.println(" ist falsch. Du kannst nur zwischen Sekunden und Minuten wählen.");
    while(timeFactor != 1000 && timeFactor != 60000) {
    }
  }
}

void checkWlan() {
  if(WiFi.status() != WL_CONNECTED) {
    Serial.println("");
    Serial.println("< Die WLAN-Verbindung wurde unterbrochen >");
  }
}
void instructions() { //Gibt die WLAN Adresse und das Passwort an und sagt was man tun muss
  IPAddress ip = WiFi.localIP();
  Serial.println("");
  Serial.println("");
  Serial.print("Du kannst dich jetzt mit dem WLAN verbinden und auf die Lamp Control Website zugreifen");
  //Serial.println("Oder Gebe in der Konsole einen Wert für die Lampe ein.");
  Serial.println("");
  Serial.println("");
  Serial.println("");
  Serial.println("|-----------INFOS-----------|");
  Serial.println("");
  Serial.print("  WLAN: ");
  Serial.println(SSID);
  Serial.println("");
  Serial.print("  PASSWORT: ");
  Serial.println(PASS);
  Serial.println("");
  Serial.print("  WEBSITE: ");
  Serial.println(ip);
  Serial.println("");
  Serial.println("|-----------INFOS-----------|");
  Serial.println("");
  Serial.println("");
  Serial.println("|------------LOG------------|");
  Serial.println("");
}

void checkInput() { //Prüft ob die, in die Konsole eingegebenen Werte, mit den Richtlinien übereinstimmen
  if(Serial.available() < INPUT_STRING_LENGTH) { //Guckt ob es nicht zu viele Zeichen sind
    if(Serial.available() > 0) { //Guckt ob es genug Zeichen sind
      delay(10);
      char inputString[INPUT_STRING_LENGTH];
      input(inputString);
      int value = atoi(inputString);
      if(value >= 0 && value <= 1023) { //Guckt ob die Zahl in dem möglichem Bereich liegt
        setLampValue(value);
      }
      else {
        Serial.println("Die Zahl muss zwischen 0 und 1023 liegen!"); //Gibt Fehlermeldung aus
      }
    }
  }
  else {
      Serial.println("Die Zahl muss zwischen 0 und 1023 liegen!"); //Gibt Fehlermeldung aus
  }
  while(Serial.read() >= 0); //Löscht den Inhalt des Eingabefeldes
}

void input(char * inputString) { //Liest den Inhalt des Eingabefeldes aus
  int inputCounter = 0;
  for(int resetCounter = 0; resetCounter <= INPUT_STRING_LENGTH-1; resetCounter++) { //Setzt die Counter zurück
          inputString[resetCounter] = '\0';
  } 
  while(Serial.available() > 0, Serial.available() < INPUT_STRING_LENGTH) { //Wird ausgeführt solange etwas im Eingabefeld steht
      char inputChar = Serial.read();
      inputString[inputCounter] = inputChar;
      inputCounter++;
      if(inputCounter >= INPUT_STRING_LENGTH-1) { //Beendet die Funktion, wenn die Maximale Anzahl an Zeichen erfasst wurde
        inputString[INPUT_STRING_LENGTH-1] = '\0';
        break;
      }
  }
  if(Serial.available() < INPUT_STRING_LENGTH) {  
    inputString[inputCounter] = '\0'; //Beendet die Funktion
  }
}

void wlanConfig() { //Die Verbindung mit dem WLAN wird hergestellt
  WiFi.mode(WIFI_STA);
  int wlanConTrys = 0;
  while(WiFi.status() != WL_CONNECTED && wlanConTrys <= MAX_WLAN_CON_TRYS - 1) {
    delay(1000);
    wlanConTrys++;
    Serial.println("");
    Serial.print(wlanConTrys);
    Serial.print(". ");
    Serial.println("Verbindungsversuch");
    WiFi.begin(SSID, PASS); //WLAN Verbindung wird mit der Adresse und dem Passwort aufgebaut
    int connectionCounter = 0;
    while (WiFi.status() != WL_CONNECTED && connectionCounter <= 20) { //Wartet 10 Sekunden bis die WLAN Verbindung hergestellt worden ist
      delay(500);
      Serial.print(".");
      connectionCounter++;
    }
    Serial.println("");
    if (WiFi.status() != WL_CONNECTED) { //Bricht den Verbindungsversuch ab wenn die WLAN Verbindung nicht besteht
      Serial.print("< Verbindung zum WLAN konnte nicht hergestellt werden");
      if(wlanConTrys <= MAX_WLAN_CON_TRYS - 1){
        Serial.print(". Noch ");
        Serial.print(MAX_WLAN_CON_TRYS - wlanConTrys); //Hat eine Bestimmte Anzahl von Verbindungsversuchen
        Serial.print(" Verbindungsversuche");
      }
      Serial.println(" >");
    }
    else { //Teilt, bei WLAN Verbindung, die IP-Adresse mit
      Serial.println("< Verbindung zum WLAN wurde hergestellt >");
    }
  }
  if(WiFi.status() != WL_CONNECTED) {
    consoleStatus = true;
    Serial.println("");
    Serial.print("Die Verbindung zum WLAN, konnte, nach ");
    Serial.print(wlanConTrys);
    Serial.println(" Verbindungsversuchen, nicht hergestellt werden.");
    Serial.println("Die Konsole kann jetzt zum Eingeben der Werte benutzt werden.");
    Serial.println("");
    Serial.println("");
    Serial.println("|------------LOG------------|");
    Serial.println("");
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
    setLampValue(dimValue);
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
    timeJson += timerTimeTimeUnit;
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
  Serial.println("< Webserver ist online >");
}

void setLampValue(int newValue) { //Setzt den Wert der Lampe
  lampValue = newValue;
  Serial.println("");
  Serial.print("Die Value wurde auf ");
  Serial.print(lampValue);
  Serial.print(" gesetzt.");
  if(timerTimeTimeUnit > 0) {
    Serial.print(" Noch ");
    Serial.print(timerTimeTimeUnit);
    Serial.print(" ");
    Serial.print(TIME_UNIT);
    Serial.print(" bis die Lampe ausgeschaltet wird.");
  }
  Serial.println("");
}

void execLampValue() {  //Schaltet die Lampe mit "DIMMER_INTERVAK_MS" Geschwindigkeit aus
  static int previousValue = 0;
  unsigned long currentTimeMs = millis();
  static unsigned long previousTimeMs = 0;
  if(lampValue != previousValue) {
    if(previousTimeMs + DIMMER_INTERVAL_MS < currentTimeMs) {
      previousTimeMs = currentTimeMs;
      int directionFactor = (previousValue > lampValue) ? 1 : -1;
      previousValue = previousValue - 1 * directionFactor;
      analogWrite(LIGHT_PIN, previousValue);
    }
  }
}

void heartBeat() {  //Schaltet die Lampe mit "DIMMER_INTERVAK_MS" Geschwindigkeit aus
  static int previousValue = 0;
  unsigned long currentTimeMs = millis();
  static unsigned long previousTimeMs = 0;

    if(previousTimeMs + HEART_BEAT_MS < currentTimeMs) {
      previousTimeMs = currentTimeMs;
      Serial.println("< System ist online >");
    }
  
}

void setTimer(int offTimerTimeUnit) { //Die Timer Zeit sowie die Start Zeit wird gesetzt
  intervalTimeUnit = offTimerTimeUnit;
  startTimeMs = millis();
  Serial.println("");
  Serial.print("Der Timer wurde auf ");
  Serial.print(offTimerTimeUnit);
  Serial.print(" ");
  Serial.print(TIME_UNIT);
  Serial.println(" gesetzt.");
}

void execTimer() { //Der Timer wird ausgeführt
  if(intervalTimeUnit != 0) {
    unsigned long intervalMs = intervalTimeUnit * timeFactor;
    unsigned long currentMs = millis();
    unsigned long timeCounterUntilMs = 0;
    unsigned long timeCounterMs = currentMs - startTimeMs;
    timeCounterUntilMs = intervalMs - timeCounterMs;
    timerTimeTimeUnit = timeCounterUntilMs / timeFactor; //Restliche Zeit wird ausgerechnet
    timerTimeTimeUnit = timerTimeTimeUnit + 1;
    if(timeCounterMs >= intervalMs) { //Guckt ob die Zeit abgelaufen ist
      Serial.println("");
      Serial.println("Der Timer ist abgelaufen.");
      intervalTimeUnit = 0;
      timerTimeTimeUnit = 0;
      setLampValue(0); //Lampe wird ausgeschaltet
    }
  }
}



