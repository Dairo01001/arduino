#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "CHIVA_AMAZONICA";
const byte MIN_DIST = 5;

const byte ECHO = 12;
const byte TRIG = 13;
const byte LED = 2;

String answer = "";

#define S0 27
#define S1 26
#define S2 25
#define S3 33
#define OUT 32

#define RED 1
#define BLUE 2
#define GREEN 3

#define MOTOR_A1 35
#define MOTOR_A2 34
#define MOTOR_B1 2
#define MOTOR_B2 4

WebServer server(80);

void setup() {
  Serial.begin(115200);
  Serial.println();

  pinMode(LED, OUTPUT);
  
  pinMode(ECHO, INPUT);
  pinMode(TRIG, OUTPUT);

  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(OUT, INPUT);
  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);

  pinMode(MOTOR_A1, OUTPUT);
  pinMode(MOTOR_A2, OUTPUT);
  pinMode(MOTOR_B1, OUTPUT);
  pinMode(MOTOR_B2, OUTPUT);
  
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
  Serial.print("Server HTTP init on: ");
  Serial.println(ip);
  
  delay(100);
}

void loop() {
  server.handleClient();
  delay(1000);
  answer = "<!DOCTYPE html> <html> <body> <h1> Server ESP32 </h1>";
  int distance = getDistance();
  Serial.print("Distance(CM): ");
  Serial.println(distance);
  answer = answer + "<h2> Distancia(CM): " + distance + "</h2>";
  if(distance < MIN_DIST) {
    digitalWrite(LED, HIGH);
    int color = getColor();
    if(color == RED) {
      Serial.println("Izquierda!");
      turnLeft();
    }
    if(color == GREEN) {
      stay();
      Serial.println("Verde!");
    }
    if(color == BLUE) {
      turnRight();
      Serial.println("Derecha!");
    }
  } else {
    digitalWrite(LED, LOW);
    forward();
  }
}


void handleRoot() {
  answer = answer + "</body> </html>";
  server.send(200, "text/html", answer);
}

String notFound = "<!DOCTYPE html> <html> <body> <h1>Error 404!</h1> </body> </html>";
void handleNotFound() {
  server.send(404, "text/html", notFound);
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
  Serial.print("RGB (");
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  delay(100);
  int red = pulseIn(OUT, LOW);
  Serial.print(red);
  Serial.print(", ");
  delay(100);

  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);
  delay(100);
  int green = pulseIn(OUT, LOW);
  Serial.print(green);
  Serial.print(", ");
  delay(100);

  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);
  delay(100);
  int blue = pulseIn(OUT, LOW);
  Serial.print(blue);                                  
  Serial.println(")");
  delay(100);

  if(red < 85 && green > 80 && blue > 80) {
    Serial.println("RED");
    answer = answer + "<h2> Color(rgb): (" + red + ", " + green + ", " + blue + ") RED</h2>";
    return RED;                  
  }

  if(red > 80 && green > 100 && blue < 80) {
    Serial.println("BLUE");
    answer = answer + "<h2> Color(rgb): (" + red + ", " + green + ", " + blue + ") BLUE</h2>";
    return BLUE;
  }

  if(red > 185 && green < 140 && blue < 130) {
    Serial.println("GREEN");
    answer = answer + "<h2> Color(rgb): (" + red + ", " + green + ", " + blue + ") GREEN</h2>";
    return GREEN;
  }
}

void stay() {
  digitalWrite(MOTOR_A1, LOW);
  digitalWrite(MOTOR_A2, LOW);
  digitalWrite(MOTOR_B1, LOW);
  digitalWrite(MOTOR_B2, LOW);
  answer = answer + "<h3> Detenido... </h3>";
  Serial.println("Parar");
}

void forward() {
  digitalWrite(MOTOR_A1, LOW);
  digitalWrite(MOTOR_A2, HIGH);
  digitalWrite(MOTOR_B1, LOW);
  digitalWrite(MOTOR_B2, HIGH);
  answer = answer + "<h3> Avanzando... </h3>";
  Serial.println("Avanzar");
}

void back() {
  digitalWrite(MOTOR_A1, HIGH);
  digitalWrite(MOTOR_A2, LOW);
  digitalWrite(MOTOR_B1, HIGH);
  digitalWrite(MOTOR_B2, LOW);
  answer = answer + "<h3> Retrocediendo... </h3>";
  Serial.println("Atras");
}

void turnRight() {
  digitalWrite(MOTOR_A1, HIGH);
  digitalWrite(MOTOR_A2, LOW);
  digitalWrite(MOTOR_B1, LOW);
  digitalWrite(MOTOR_B2, HIGH);
  answer = answer + "<h3> Derecha... </h3>";
  Serial.println("Derecha");
}

void turnLeft() {
  digitalWrite(MOTOR_A1, LOW);
  digitalWrite(MOTOR_A2, HIGH);
  digitalWrite(MOTOR_B1, HIGH);
  digitalWrite(MOTOR_B2, LOW);
  answer = answer + "<h3> Izquierda... </h3>";
  Serial.println("Izquierda");
}
