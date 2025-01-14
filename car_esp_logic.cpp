#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>

// Create a WebSocketsServer instance on port 81
WebSocketsServer server = WebSocketsServer(81);

// WiFi credentials
String idd = "WIFI ";
String pass = "Password"; // For privacy, replace with your actual credentials

// Movement and speed control variables
bool forward = false;
bool backward = false;
bool left = false;
bool right = false;
int actual_speed = 0;

// Function prototype for WebSocket events
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length);

// GPIO pin assignments
int forward_pin = D0;
int backward_pin = D1;
int right_pin = D8;
int left_pin = D4;
int speed_control_pin = D7;

void setup()
{
  // Start the serial monitor for debugging
  Serial.begin(9600);

  // Initialize pins for output
  pinMode(LED_BUILTIN, OUTPUT);  // Onboard LED for status indication
  pinMode(forward_pin, OUTPUT); 
  pinMode(backward_pin, OUTPUT); 
  pinMode(left_pin, OUTPUT); 
  pinMode(right_pin, OUTPUT); 
  pinMode(speed_control_pin, OUTPUT); 

  // Connect to WiFi
  WiFi.begin(idd, pass);
  while (WiFi.status() != WL_CONNECTED)
  {
    // Blink LED to indicate connection progress
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
    
    Serial.print("."); // Debugging WiFi connection status
  }
  
  // Turn off LED and print success message once connected
  digitalWrite(LED_BUILTIN, LOW);
  Serial.print("!!!");
  Serial.print(WiFi.localIP()); // Print device IP address

  // Start the WebSocket server
  server.begin();
  server.onEvent(webSocketEvent); // Assign the event handler for WebSocket events
}

void loop() 
{
    // Handle WebSocket events
    server.loop();
    
    // Control motor pins based on received instructions
    digitalWrite(forward_pin, forward);
    digitalWrite(backward_pin, backward);
    digitalWrite(left_pin, left);
    digitalWrite(right_pin, right);
    
    // Control speed using PWM
    analogWrite(speed_control_pin, actual_speed);
}

// Event handler for WebSocket messages
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
    if (type == WStype_TEXT) 
    {
        // Convert received payload to a string
        String message = String((char*)payload);

        // Parse JSON message
        DynamicJsonDocument doc(1024);
        deserializeJson(doc, message);

        // Update movement and speed control variables from JSON data
        forward = doc["forward"];
        backward = doc["backward"];
        left = doc["left"];
        right = doc["right"];
        actual_speed = doc["speed"];
    }
}
