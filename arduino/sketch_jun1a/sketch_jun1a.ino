#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "base64_utils.h"
IPAddress mqttServer(185, 152, 81, 104);
const char* SSID = "malcon";
const char* PWD = "1234fav1234";
long lastime = 0, lassensor_l = 0, lassensor_r = 0, lassensor_p = 0, lassensor_b = 0;
String command_left = "", command_right = "", command_primary = "", command_battery = "";
#define ROBOT_NAME "ROBOT1"
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.println("Callback: ");
  Serial.print((char*)payload);
  Serial.println();
}
WiFiClient wifiClient = WiFiClient();
PubSubClient mqttClient(mqttServer, 1883, callback, wifiClient);

void connectToWiFi() {
  Serial.print("Connecting to ");
  Serial.println(SSID);
  WiFi.begin(SSID, PWD);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
    // we can even make the ESP32 to sleep
  }

  Serial.print("Connected - ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  Serial.println("Connecting to MQTT Broker...");
  while (!mqttClient.connected()) {
    Serial.println("Reconnecting to MQTT Broker..");
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);

    if (mqttClient.connect(clientId.c_str())) {
      Serial.println("Connected.");
      // subscribe to topic
    }
  }
}

void setup() {
  Serial.begin(9600);
  connectToWiFi();
}

void sendData(String command, char* type) {
  long now = millis();
  command = String(ROBOT_NAME) + command;
  int command_len = command.length();
  int encoded_len = b64_enc_len(command_len);
  char* encoded_data = new char[encoded_len];
  int b64len = b64_encode(encoded_data, (char*)command.c_str(), command_len);
  Serial.println("Publishing data..");
  Serial.println(type);
  String encoded = String(encoded_data);
  encoded.trim();
  // read temperature from sensor
  mqttClient.publish("esp8266/sensor/", encoded.c_str());
  // read humidity from sensor
  mqttClient.publish("esp8266/test", String(now / 1110).c_str());
  delay(40);
}
String getData(char* str) {
  String command = "";

  while (Serial.available()) {
    command = Serial.readString();
    command.trim();
  }
  delay(40);
  if (command.substring(0, 4) == str) {
    return command;
  }
  return command;
}
void loop() {

  if (!mqttClient.connected())
    reconnect();
  mqttClient.loop();
  long now = millis();

  if (now - lassensor_l > 4000) {
    Serial.println("SL");
    delay(50);
    command_left = getData("SENL");

    lassensor_l = now;
    if (command_left.length() > 0) {
      sendData(command_left, "left");
    }
  }


  if (now - lassensor_r > 7000) {
    Serial.println("SR");
    delay(50);
    command_right = getData("SENR");
    lassensor_r = now;
    if (command_right.length() > 0) {
      sendData(command_right, "right");
    }
  }

  if (now - lassensor_p > 10000) {
    Serial.println("SP");
    delay(50);
    command_primary = getData("SENP");
    lassensor_p = now;
    if (command_primary.length() > 0) {
      sendData(command_primary, "primary");
    }
  }

  if (now - lassensor_b > 12000) {
    Serial.println("SB");
    delay(50);
    command_battery = getData("BATT");
    lassensor_b = now;
    if (command_battery.length() > 0) {
      sendData(command_battery, "battery");
    }
  }

  command_primary = "";
  command_left = "";
  command_right = "";
  command_battery = "";
  // Publishing data to MQTT
}