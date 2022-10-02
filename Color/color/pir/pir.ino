const int PIR = 2;
const int LED = 13;

int state = 0;
int pirState = LOW;

void setup() {
  Serial.begin(9600);
  
  pinMode(PIR, INPUT);
  pinMode(LED, OUTPUT);
  
  delay(20000);
  Serial.println("Inicio");
}

void loop() {
  state = digitalRead(PIR);
  
  if(state == HIGH) {

    if(pirState == LOW) {
      Serial.println("Sensor activado!");
      pirState = HIGH;
    }
    
    digitalWrite(LED, HIGH);
    delay(50);
    digitalWrite(LED, LOW);
    delay(50);
    
  } else {
    
    if(pirState == HIGH) {
      Serial.println("Sensor parado!");
      pirState = LOW;
    }
    
    digitalWrite(LED, LOW);
  }
}
