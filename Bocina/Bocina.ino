#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

const int PIR = 5;
const int LED = 13;

int state = 0;
int pirState = LOW;

SoftwareSerial mySoftwareSerial(2, 3);
DFRobotDFPlayerMini myDFPlayer;

void setup() {
  mySoftwareSerial.begin(9600);
  Serial.begin(115200);

  pinMode(PIR, INPUT);
  pinMode(LED, OUTPUT);
  delay(20000);
  Serial.println("Start PIR");
  
  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));
  if (!myDFPlayer.begin(mySoftwareSerial)) {
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true);
  }
  Serial.println(F("DFPlayer Mini online."));
  
  myDFPlayer.setTimeOut(500);
  myDFPlayer.volume(30);
  
  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);
}

void loop() {
  state = digitalRead(PIR);

  if(state == HIGH) {
    if(pirState == LOW) {
      pirState = HIGH;
      myDFPlayer.play(1);
    }
    
    digitalWrite(LED, HIGH);
    delay(50);
    digitalWrite(LED, LOW);
    delay(50);
  } else {
    if(pirState == HIGH) {
      pirState = LOW;
      myDFPlayer.pause();
    }
    digitalWrite(LED, LOW);
  }
}
