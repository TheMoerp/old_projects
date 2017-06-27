
  int lightPin = 2;
  char a[5];
  byte cI = 0;
  
void setup() {
  Serial.begin(74880);
  //pinMode(lightPin, OUTPUT);
  Serial.print("Gebe die Value(0-1023)ein: ");
  Serial.print(Serial.available()); 
}

void loop() {
  if(Serial.available() > 0){
    delay(10);
    Serial.println(Input());
  }
  //analogWrite(lightPin, );
}

char * Input(){
  for(int i = 0; i<=4; i++){
          a[i]='\0';
          cI=0; 
  }
  while(Serial.available() > 0){
      char t = Serial.read();
      a[cI]= t;
      cI++;
      if(cI>=4){
        a[4]='\0';
        return a;
        }
      }
    
    a[cI]='\0';
  return a;
}

