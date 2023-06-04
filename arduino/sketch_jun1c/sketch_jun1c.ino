#include <SoftwareSerial.h>

// создаём объект для работы с программным Serial
// и передаём ему пины TX и RX
SoftwareSerial mySerial(2, 3);

// serial-порт к которому подключён Wi-Fi модуль
#define WIFI_SERIAL mySerial

String command;

void setup() {
  // открываем последовательный порт для мониторинга действий в программе
  // и передаём скорость 9600 бод
  Serial.begin(9600);
  Serial.print("Serial init OK\r\n");
  // открываем Serial-соединение с Wi-Fi модулем на скорости 115200 бод
  WIFI_SERIAL.begin(115200);
}

void loop() {
  while (!WIFI_SERIAL.readString() == "WAITSENL") {
    command = "SENL" + String(25);
  }


  WIFI_SERIAL.println(command);
  delay(300);
  while (!WIFI_SERIAL.readString() == "WAITSENR") {
    command = "SENR" + String(18);
  }
  WIFI_SERIAL.println(command);
  delay(1000);
  command = "SENP" + String(39);
  WIFI_SERIAL.println(command);
  delay(1000);
}