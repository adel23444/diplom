#include <ESP8266WiFi.h>
#include <PubSubClient.h>
IPAddress mqttServer(185, 152, 81, 104);
const char* SSID = "xiaomi11lite";
const char* PWD = "favorit123";
long lastime = 0;
String command_str = "";
boolean recievedFlag;
int tm = 300;
#define ROBOT_NAME "ROBOT1"
enum signals {
  LEFT: "WAITSENL",
  RIGHT: "WAITSENR",
  PRIMARY: "WAITSENP"
}
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

    Serial.println("Cannot connect!");
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
      mqttClient.subscribe("robot/test");
      mqttClient.publish("robot/test", (String(ROBOT_NAME)+"WI1").c_str());
      delay(200);
    }
  }
}
void setup() {
  Serial.begin(115200);
  connectToWiFi();
}
void DataSend(String command) {
  if (tm = 0) {
    
    Serial.println("Publishing data..");
    mqttClient.publish("robot/test", command.c_str(), command.length());
    Serial.println(command_str);
    tm = 300;  // пауза меду отправками значений температуры коло 3 секунд
  }
  tm--;
  delay(10);
}
void loop() {

  if (!mqttClient.connected())
    reconnect();
  mqttClient.loop();
  while (Serial.available()) {
    char symbol = (char)Serial.read();
    command_str += String(symbol);

    delay(5);
  }
  command_str = String(ROBOT_NAME) + command_str;
  recievedFlag = true;
  DataSend(command_str);
  command_str = "";
}
