
  int lightPin = 2;
  char strInput[5];
  byte inputCtr = 0;
  char *strValPtr = 0;
  int preVal = 0;
  int val = 0;
  
void setup() {
  Serial.begin(74880);
  pinMode(lightPin, OUTPUT);
  analogWrite(lightPin, val);
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

