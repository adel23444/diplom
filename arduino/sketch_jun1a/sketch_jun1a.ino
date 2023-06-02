#include <ESP8266WiFi.h>
#include <PubSubClient.h>
IPAddress mqttServer(192, 168, 0, 106);
const char* SSID = "malcon";
const char* PWD = "1234fav1234";
long lastime = 0;

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.println("Callback");
  Serial.println((char)payload[0]);
}
WiFiClient wifiClient = WiFiClient();
PubSubClient mqttClient(mqttServer, 1883, callback, wifiClient);

void connectToWiFi() {
  Serial.print("Connecting to ");
  Serial.println(SSID);
  WiFi.begin(SSID, PWD);
  while (WiFi.status() != WL_CONNECTED) {
    
    Serial.print("Cannot connect!");
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

void loop() {

  Serial.println("CONNECTED!!!");

  if (!mqttClient.connected())
    reconnect();
  mqttClient.loop();


  // Publishing data to MQTT
  long now = millis();
  if (now - lastime > 10000) {
    Serial.println("Publishing data..");
    // read temperature from sensor
    mqttClient.publish("robot/test", "hello_temp");
    delay(200);
    // read humidity from sensor
    mqttClient.publish("robot/test", "hello_humidity");
    lastime = now;
  }
  delay(300);
}


