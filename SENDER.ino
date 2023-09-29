#include <ArduinoMqttClient.h>
#if defined(ARDUINO_SAMD_MKRWIFI1010) || defined(ARDUINO_SAMD_NANO_33_IOT) || defined(ARDUINO_AVR_UNO_WIFI_REV2)
#include <WiFiNINA.h>
#elif defined(ARDUINO_SAMD_MKR1000)
#include <WiFi101.h>
#elif defined(ARDUINO_ARCH_ESP8266)
#include <ESP8266WiFi.h>
#elif defined(ARDUINO_ARCH_ESP32)
#include <WiFi.h>
#endif

char ssid[] = "Moto G";  // your network SSID (name)
char pass[] = "12341234";  // your network password (use for WPA, or use as key for WEP)

const int trigPin = 2;
const int echoPin = 3;
const int ledPin = 13;  // Change this to the actual pin connected to your LED

float duration, distance;

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

const char broker[] = "broker.mqttdashboard.com";
int port = 1883;
const char topic[] = "SIT210/waves";

const long interval = 1000;
unsigned long previousMillis = 0;

int count = 0;

void setup() {
  // Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ;  // wait for serial port to connect. Needed for the native USB port only
  }

  // Attempt to connect to WiFi network:
  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(ssid);
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    // Failed, retry
    Serial.print(".");
    delay(5000);
  }

  Serial.println("You're connected to the network");
  Serial.println();

  // You can provide a unique client ID, if not set, the library uses Arduino-millis()
  // Each client must have a unique client ID
  // mqttClient.setId("clientId");

  // You can provide a username and password for authentication
  // mqttClient.setUsernamePassword("username", "password");

  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);

  if (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());

    while (1);
  }

  Serial.println("You're connected to the MQTT broker!");
  Serial.println();

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);  // Set the initial LED state
}

void loop() {
  // Call poll() regularly to allow the library to send MQTT keep-alives, which
  // avoids being disconnected by the broker
  mqttClient.poll();

  // To avoid having delays in the loop, we'll use the strategy from BlinkWithoutDelay
  // See: File -> Examples -> 02.Digital -> BlinkWithoutDelay for more info
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // Save the last time a message was sent
    previousMillis = currentMillis;

    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    duration = pulseIn(echoPin, HIGH);
    distance = (duration * 0.0343) / 2;
    Serial.print("Distance: ");
    Serial.println(distance);

    // Prepare the message
    String message = "saksham&parikshit : Wave is detected, Distance: " + String(distance);
    mqttClient.beginMessage(topic);
    mqttClient.print(message);
    mqttClient.endMessage();
    delay(1000);

    // Check the message content and control the LED accordingly
    if (message.indexOf("pat") != -1) {
      // If "pat" is found in the message, turn the LED on and off quickly
      digitalWrite(ledPin, HIGH);
      delay(100);
      digitalWrite(ledPin, LOW);
      delay(100)
      digitalWrite(ledPin, HIGH);
      delay(100);
      digitalWrite(ledPin, LOW);
      delay(100)
      digitalWrite(ledPin, HIGH);
      delay(100);
      digitalWrite(ledPin, LOW);
    } else if (message.indexOf("wave") != -1) {
      // If "wave" is found in the message, blink the LED slowly
      for (int i = 0; i < 5; i++) {
      digitalWrite(ledPin, HIGH);
      delay(500);
      digitalWrite(ledPin, LOW);
      delay(500);
      digitalWrite(ledPin, HIGH);
      delay(500);
      digitalWrite(ledPin, LOW);
      delay(500)
      digitalWrite(ledPin, HIGH);
      delay(500);
      digitalWrite(ledPin, LOW);
      }
    }

    Serial.println();

    count++;
  }
}
