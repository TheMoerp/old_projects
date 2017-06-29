
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
    server.send(200, "text/html", "Hallo");
  });
  server.onNotFound([](){
    server.send(200, "text/html", "404/Error");
  });
  server.begin();
  Serial.println("Webserver ist online");
 
}
