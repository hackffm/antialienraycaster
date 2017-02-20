int ledPin = 2;

void setup() {
  pinMode(ledPin, OUTPUT);
}

void loop() {
  digitalWrite(ledPin, HIGH);   
  //delay(1);
  delayMicroseconds(200);                 
  digitalWrite(ledPin, LOW);  
  delayMicroseconds(200);
  //delay(2); 
}
