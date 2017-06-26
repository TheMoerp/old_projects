
  int lightPin = 2;
  int val = 0;
  int inV = 0;
  char a[4];
  byte cI = 0;
  


void setup() {
  Serial.begin(74880);
  
  pinMode(lightPin, OUTPUT);
  Serial.print("Gebe die Value(0-1023)ein: ");
  Serial.print(Serial.available());
  
}

void loop() {
  
  Input(val, inV);
  analogWrite(lightPin, val);
}

int Input(int val, int inV){
   
   if(Serial.available() > 0){
    for(int i = 0; i<=4; i++){
      a[cI]= Serial.read();
      cI++;
    }
      
    
    //if( <= 1023, inV >= 1){
      //int val =inV;
     
      Serial.print(a);
    //}
    //else{
      //int inV=0;
      //Serial.println("Zahl muss >=1 und <=1023 sein.");
    //}
  }
}

