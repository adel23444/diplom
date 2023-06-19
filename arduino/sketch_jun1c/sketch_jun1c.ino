#include <SoftwareSerial.h>
#include <iarduino_VCC.h>
#include <NewPing.h>
#include <Thread.h>
// создаём объект для работы с программным Serial
// и передаём ему пины TX и RX
SoftwareSerial BTSerial(2, 3);
SoftwareSerial mySerial(10, 11);

Thread sensor_left = Thread();
Thread sensor_right = Thread();
Thread sensor_primary = Thread();
Thread battery = Thread();


// Код ошибки: код 01; Радиус поворота выше скорости; Код 02; Скорость выше 255;
#define CON_MOTOR1 0
#define CON_MOTOR2 0

// Motor shield использует четыре контакта 4, 5, 6, 7 для управления моторами
// 4 и 7 — для направления, 5 и 6 — для скорости
#define SPEED_1 5
#define DIR_1 4

#define SPEED_2 6
#define DIR_2 7

// Возможные направления движения робота
#define FORWARD 0
#define BACKWARD 1
#define LEFT 2
#define RIGHT 3

// serial-порт к которому подключён Wi-Fi модуль
#define WIFI_SERIAL mySerial
#define MAX_VOLTAGE 5.0
#define MANUAL_BUTTON 8
String command_l, command_r, command_p, command_b;
long lassensor_l = 0, lassensor_r = 0, lassensor_p = 0, lassensor_b = 0;
unsigned int distance_primary = 0, distance_left = 0, distance_right = 0;

int command;                        //Int to store app command state.
byte Speed = 204, SlowSpeed = 191;  // 0 - 255.
byte LED = 13;
int Speedsec;
int buttonState = 0;
int lastButtonState = 0;
int Turnradius = 0;  //Set the radius of a turn, 0 - 255 Note:the robot will malfunction if this is higher than int Speed.
int brakeTime = 45;
int brkonoff = 1;  //1 for the electronic braking system, 0 for normal.

volatile boolean manualMode = true;
volatile uint16_t mydelay = 5000;
volatile uint16_t timecore = 0, last_press = 0;

byte PIN_TRIG_LEFT = 1.4;
byte PIN_ECHO_LEFT = 15;

byte PIN_TRIG_RIGHT = 16;
byte PIN_ECHO_RIGHT = 17;

byte PIN_TRIG_PRIMARY = 18;
byte PIN_ECHO_PRIMARY = 19;

byte MAX_DISTANCE = 40;

NewPing sonar_left(PIN_TRIG_LEFT, PIN_ECHO_LEFT, MAX_DISTANCE);
NewPing sonar_right(PIN_TRIG_RIGHT, PIN_ECHO_RIGHT, MAX_DISTANCE);
NewPing sonar_primary(PIN_TRIG_PRIMARY, PIN_ECHO_PRIMARY, MAX_DISTANCE);

void setup() {
  // открываем последовательный порт для мониторинга действий в программе
  // и передаём скорость 9600 бод
  Serial.begin(9600);
  Serial.print("Serial init OK\r\n");
  // открываем Serial-соединение с Wi-Fi модулем на скорости 115200 бод
  WIFI_SERIAL.begin(9600);
  for (int i = 4; i <= 7; i++)
    pinMode(i, OUTPUT);
  pinMode(LED, OUTPUT);  //Set the LED pin.
  pinMode(MANUAL_BUTTON, INPUT_PULLUP);
  BTSerial.begin(9600);  //Set the baud rate to your Bluetooth module.
  sensor_left.onRun(sensorLeft);
  sensor_left.setInterval(1750);
  sensor_right.onRun(sensorRight);
  sensor_right.setInterval(2000);
  sensor_primary.onRun(sensorPrimary);
  sensor_primary.setInterval(2250);
  battery.onRun(Battery);
  battery.setInterval(6000);
  TCCR2B |= (1 << WGM20);
  TIMSK2 |= (1 << OCIE2B);
  TCCR2B |= (1 << CS21) | (1 << CS20) | (1 << CS22);
  OCR2B = 50;
}

SIGNAL (TIMER2_COMPB_vect) {
  timecore++;
  if (timecore > mydelay) {
    timecore = 0;
  }
  unsigned int button = digitalRead(MANUAL_BUTTON);

  if (button == HIGH && millis() - last_press > 100) {
    Stop();
    manualMode = !manualMode;
    last_press = millis();
  }
}

void loop() {
  unsigned int button = digitalRead(MANUAL_BUTTON);
  long now = millis();
  if (sensor_left.shouldRun())
    sensor_left.run();

  if (sensor_right.shouldRun())
    sensor_right.run();

  if (sensor_primary.shouldRun())
    sensor_primary.run();

  if (battery.shouldRun())
    battery.run();

  if (button == HIGH) {
    Stop();
    manualMode = !manualMode;
  }
  if (manualMode) {
    // Serial.println("MANUAL MODE ON");

    manualDrive();
    if (now - millis() > 1000) {
      digitalWrite(LED, HIGH);
    }
    if (now - millis() > 2000) {
      digitalWrite(LED, LOW);
    }
  } else {
    // Serial.println("MANUAL MODE OFF");
    sei();
    automaticDrive();
  }
}

void manualDrive() {
  if (BTSerial.available() > 0) {
    command = BTSerial.read();
    Stop();  //Initialize with motors stoped.
    switch (command) {
      case 'F':
        go(FORWARD, Speed);
        break;
      case 'B':
        go(BACKWARD, Speed);
        break;
      case 'L':
        go(LEFT, Speed);
        break;
      case 'R':
        go(RIGHT, Speed);
        break;
      case '0':
        Speed = 100;
        break;
      case '1':
        Speed = 140;
        break;
      case '2':
        Speed = 153;
        break;
      case '3':
        Speed = 165;
        break;
      case '4':
        Speed = 178;
        break;
      case '5':
        Speed = 191;
        break;
      case '6':
        Speed = 204;
        break;
      case '7':
        Speed = 216;
        break;
      case '8':
        Speed = 229;
        break;
      case '9':
        Speed = 242;
        break;
      case 'q':
        Speed = 255;
        break;
    }
    Speedsec = Turnradius;
    if (brkonoff == 1) {
      brakeOn();
    } else {
      brakeOff();
    }
  }
}

void automaticDrive() {


  for (byte i = 0; i < 100; i++) {
    if (manualMode == HIGH) {
      break;
    }
    if ((distance_left < 30 && distance_left > 20) || ((distance_left < 30 && distance_left > 20) && (distance_primary < 30 && distance_primary > 20))
        || (distance_right < 30 && distance_right > 20) || ((distance_right < 30 && distance_right > 20) && (distance_primary < 30 && distance_primary > 20))) {
      go(FORWARD, SlowSpeed);
    } else if (distance_left < 20 || (distance_left < 20 && distance_primary < 20)) {
      go(RIGHT, SlowSpeed);
    } else if (distance_right < 20 || (distance_right < 20 && distance_primary < 20)) {
      go(LEFT, SlowSpeed);
    } else if (distance_right < 20 && distance_left < 20 && distance_primary < 20) {
      go(BACKWARD, Speed);
    }

    go(FORWARD, Speed);
  }
}

void Stop() {
  analogWrite(SPEED_1, 0);
  analogWrite(SPEED_2, 0);
  analogWrite(DIR_1, 0);
  analogWrite(DIR_2, 0);
}

void brakeOn() {
  //Here's the future use: an electronic braking system!
  // read the pushbutton input pin:
  buttonState = command;
  // compare the buttonState to its previous state
  if (buttonState != lastButtonState) {
    // if the state has changed, increment the counter
    if (lastButtonState == 'F') {
      if (buttonState == 'S') {
        go(BACKWARD, Speed);

        delay(brakeTime);
        Stop();
      }
    }
    if (lastButtonState == 'B') {
      if (buttonState == 'S') {
        go(FORWARD, Speed);
        delay(brakeTime);
        Stop();
      }
    }
    if (lastButtonState == 'L') {
      if (buttonState == 'S') {
        go(LEFT, Speed);

        delay(brakeTime);
        Stop();
      }
    }
    if (lastButtonState == 'R') {
      if (buttonState == 'S') {
        go(RIGHT, Speed);

        delay(brakeTime);
        Stop();
      }
    }
  }
  // save the current state as the last state,
  //for next time through the loop
  lastButtonState = buttonState;
}
void brakeOff() {
}

void go(int newDirection, int speed) {
  boolean motorDirection_1, motorDirection_2;

  switch (newDirection) {
    case FORWARD:
      motorDirection_1 = true;
      motorDirection_2 = false;

      break;
    case BACKWARD:
      motorDirection_1 = false;
      motorDirection_2 = true;
      break;
    case LEFT:
      motorDirection_1 = false;
      motorDirection_2 = false;
      break;
    case RIGHT:
      motorDirection_1 = true;
      motorDirection_2 = true;
      break;
  }

  // Если мы ошиблись с подключением - меняем направление на обратное
  motorDirection_1 = CON_MOTOR1 ^ motorDirection_1;
  motorDirection_2 = CON_MOTOR2 ^ motorDirection_2;

  // Поехали! Скорость может меняться в пределах от 0 до 255.
  analogWrite(SPEED_1, speed);
  analogWrite(SPEED_2, speed);

  digitalWrite(DIR_1, motorDirection_1);
  digitalWrite(DIR_2, motorDirection_2);
}

void sensorLeft() {
  unsigned int distanceSm_left = sonar_left.ping();  // Создание сигнала, получение параметра его продолжительности в мкс (uS).
  distance_left = distanceSm_left / US_ROUNDTRIP_CM;
  delay(20);
  command_l = "SENL" + String(distance_left);
  Serial.println(command_l);
  WIFI_SERIAL.println(command_l);
  Serial.println("LEFT sended");
  command_l = "";
}
void sensorRight() {
  unsigned int distanceSm_right = sonar_right.ping();  // Создание сигнала, получение параметра его продолжительности в мкс (uS).
  distance_right = distanceSm_right / US_ROUNDTRIP_CM;
  delay(20);
  command_r = "SENR" + String(distance_right);
  Serial.println(command_r);
  WIFI_SERIAL.println(command_r);
  Serial.println("RIGHT sended");
  command_r = "";
}
void sensorPrimary() {
  unsigned int distanceSm_primary = sonar_primary.ping();  // Создание сигнала, получение параметра его продолжительности в мкс (uS).
  distance_primary = distanceSm_primary / US_ROUNDTRIP_CM;
  delay(20);
  command_p = "SENP" + String(distance_primary);
  Serial.println(command_p);
  WIFI_SERIAL.println(command_p);
  Serial.println("PRIMARY sended");
  command_p = "";
}
void Battery() {
  float battery_value = analogRead_VCC();
  delay(20);
  command_b = "BATT" + String(round((battery_value / 5.0) * 100));
  Serial.println(command_b);
  WIFI_SERIAL.println(command_b);
  Serial.println("BATTERY sended");
  command_p = "";
}