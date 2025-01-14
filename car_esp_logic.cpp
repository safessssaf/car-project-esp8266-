#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>


WebSocketsServer server = WebSocketsServer(81);
String idd = "esp";
String pass = "";// this is for privacy reasons;
bool forward = false;
bool backward = false;
bool left = false;
bool right = false;
int actual_speed = 0;
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length);

int forward_pin = D0;
int backward_pin = D1;
int right_pin = D8;
int left_pin = D4;
int speed_control_pin = D7;

// the setup function runs once when you press reset or power the board

void setup()
{
  Serial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(forward_pin, OUTPUT);
  pinMode(backward_pin, OUTPUT);
  pinMode(left_pin, OUTPUT);
  pinMode(right_pin, OUTPUT);
  pinMode(speed_control_pin, OUTPUT);
  //Turing the esp into access point for outdoor occasions.
  WiFi.softAP(idd, pass);

  IPAddress IP = WiFi.softAPIP();
  digitalWrite(LED_BUILTIN, LOW);

  Serial.print(IP);


  server.begin();
  server.onEvent(webSocketEvent);
}

// the loop function runs over and over again forever
void loop() 
{  
    
    server.loop();
    digitalWrite(forward_pin, forward);
    digitalWrite(backward_pin, backward);
    digitalWrite(left_pin, left);
    digitalWrite(right_pin, right);
    analogWrite(speed_control_pin, actual_speed);
    
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
    if (type == WStype_TEXT) 
    {
        String message = String((char*)payload);

        DynamicJsonDocument doc(1024);
        deserializeJson(doc, message);

        forward = doc["forward"];
        backward = doc["backward"];
        left = doc["left"];
        right = doc["right"];
        actual_speed = doc["speed"];
        
    }
}         

