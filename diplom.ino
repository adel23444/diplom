#include <NewPing.h>

#define PIN_TRIG_LEFT 5
#define PIN_ECHO_LEFT 6

#define PIN_TRIG_RIGHT 3
#define PIN_ECHO_RIGHT 4

#define PIN_TRIG_PRIMARY 10
#define PIN_ECHO_PRIMARY 9

#define MAX_DISTANCE 30

NewPing sonar_left(PIN_TRIG_LEFT, PIN_ECHO_LEFT, MAX_DISTANCE);
NewPing sonar_right(PIN_TRIG_RIGHT, PIN_ECHO_RIGHT, MAX_DISTANCE);
NewPing sonar_primary(PIN_TRIG_PRIMARY, PIN_ECHO_PRIMARY, MAX_DISTANCE);


byte LED_BACK=13;
byte LED_RIGHT=12;
byte LED_LEFT=11;

void setup() {
  // put your setup code here, to run once:
  delay(1000);
  pinMode(LED_BACK, OUTPUT);
  pinMode(LED_RIGHT, OUTPUT);
  pinMode(LED_LEFT, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned int distanceSm_left = sonar_left.ping(); // Создание сигнала, получение параметра его продолжительности в мкс (uS).
  unsigned int distance_left = distanceSm_left / US_ROUNDTRIP_CM;
  Serial.print("Ping: ");
  Serial.print(distance_left); // Пересчет параметра времени в величину расстояния и вывод результата (0 соответствует выходу за допустимый предел)
  Serial.println("cm");

  if (distance_left < 20) {
    byte i = 0;
    while (i < 10) {
      digitalWrite(LED_RIGHT, HIGH);
      delay(300);
      digitalWrite(LED_RIGHT, LOW);
      delay(300);
    i++;
    }
  }
  delay(1000);
}
