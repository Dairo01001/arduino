const int PIN_PULSADOR = 7;

const int ESTADO_APRENDER_COLOR_UNO = 1;
const int ESTADO_APRENDER_COLOR_DOS = 2;
const int ESTADO_CLASIFICAR = 3;
int estado = 0;

int pullOld = 1;
int pullState = 0;
int pullInput;



void clasificarColor() {
  
}

void setup() {
  Serial.begin(9600);
  pinMode(PIN_PULSADOR, INPUT_PULLUP);
}

void loop() {
  pullInput = digitalRead(PIN_PULSADOR);
  
  if(ESTADO_APRENDER_COLOR_UNO == estado) {
    Serial.print("Estado: ");
    Serial.print(estado);
    Serial.println();
  } else if(ESTADO_APRENDER_COLOR_DOS == estado) {
    Serial.print("Estado: ");
    Serial.print(estado);
    Serial.println();
  } else if(ESTADO_CLASIFICAR == estado) {
    Serial.print("Estado: ");
    Serial.print(estado);
    Serial.println();
    clasificarColor();
  }

  if(estado == 0) {
    Serial.print("Estado: ");
    Serial.print(estado);
    Serial.println();
  }

  
  if(pullInput == LOW && pullOld == HIGH){
    if(pullState == LOW) {
      pullState = HIGH;
      estado = estado + 1;
      if(estado > 3) {
        estado = 0;
      }
    } else {
      pullState = LOW;
    }
  }
  pullOld = pullInput;
  
  delay(1000);
}
