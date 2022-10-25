#include <WiFi.h>
#include <WebServer.h>

#define ENA 13
#define IN1 12
#define IN2 14
#define IN3 27
#define IN4 26
#define ENB 25

#define ECHO 32
#define TRIG 33

#define S0 35
#define S1 34
#define S2 18
#define S3 5
#define OUT 19

#define RED 1
#define BLUE 2
#define GREEN 3

const char* ssid = "CHIVA_AMAZONICA";

const byte MIN_DIST = 5;

const int SPEED = 255; // 0 - 255
const int TIME = 500;

void initializeHBridgePins() {
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);
}

void initializeDistanceSensor() {
  pinMode(ECHO, INPUT);
  pinMode(TRIG, OUTPUT);
}

void initializeColorSensor() {
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(OUT, INPUT);

  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);
}

WebServer server(80);

void setup() {
  Serial.begin(115200);
  initializeDistanceSensor();
  initializeColorSensor();
  initializeHBridgePins();

  Serial.println("Setting Soft-AP mode...");
  Serial.println(WiFi.softAP(ssid) ? "Done!" : "Error!");
  IPAddress ip = WiFi.softAPIP();

  Serial.print("Name RED: ");
  Serial.println(ssid);
  Serial.print("IP: ");
  Serial.println(ip);

  server.on("/", handleRoot);
  server.onNotFound(handleNotFound);
   
  server.begin();
}

void loop() {
  server.handleClient();
  int distance = getDistance();
  // Serial.print("Distance(CM): ");
  // Serial.println(distance);
  if(distance < MIN_DIST || distance > 1000) {
    stopWheels();
    
    int color = getColor();
    
    if(color == RED) {
      turnLeft(SPEED);
    }
    
    if(color == GREEN) {
      stopWheels();
    }
    
    if(color == BLUE) {
      turnRight(SPEED);
    }
    
  } else {
    forward(SPEED);
  }
}

void forward(int spe) {
  rightWheelForward(spe);
  leftWheelForward(spe);
}

void turnRight(int spe) {
  leftWheelForward(spe);
  rightWheelBack(spe);
  delay(TIME);
}

void turnLeft(int spe) {
  rightWheelForward(spe);
  leftWheelBack(spe);
  delay(TIME);
}

void stopWheels() {
  digitalWrite(ENA, LOW);
  digitalWrite(ENB, LOW);
}

void rightWheelBack(int spe) {
  analogWrite(ENA, spe);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
}

void rightWheelForward(int spe) {
  analogWrite(ENA, spe);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
}

void leftWheelBack(int spe) {
  analogWrite(ENB, spe);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void leftWheelForward(int spe) {
  analogWrite(ENB, spe);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

int getDistance() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(20);
  digitalWrite(TRIG, LOW);
  float distance = pulseIn(ECHO, HIGH);
  distance = distance / 58;
  return (int) distance;
}

int getColor() {
  // Serial.print("RGB (");
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  delay(100);
  int red = pulseIn(OUT, LOW);
  // Serial.print(red);
  // Serial.print(", ");
  delay(100);

  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);
  delay(100);
  int green = pulseIn(OUT, LOW);
  // Serial.print(green);
  // Serial.print(", ");
  delay(100);

  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);
  delay(100);
  int blue = pulseIn(OUT, LOW);
  // Serial.print(blue);                                  
  // Serial.println(")");
  delay(100);

  if(red < 85 && green > 80 && blue > 80) {
    // Serial.println("RED");
    return RED;                  
  }

  if(red > 80 && green > 100 && blue < 80) {
    // Serial.println("BLUE");
    return BLUE;
  }

  if(red > 185 && green < 140 && blue < 130) {
    // Serial.println("GREEN");
    return GREEN;
  }
}

void handleRoot() {
  server.send(200, "text/html", "<!DOCTYPE html> <html> <body> <h1>Hello World!</h1> </body> </html>");
}

void handleNotFound() {
  server.send(404, "text/html", "<!DOCTYPE html> <html> <body> <h1>Error 404!</h1> </body> </html>");
}
