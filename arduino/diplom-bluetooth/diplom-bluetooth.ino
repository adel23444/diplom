#include <SoftwareSerial.h>
// Код ошибки: код 01; Радиус поворота выше скорости; Код 02; Скорость выше 255;
#define CON_MOTOR1     0
#define CON_MOTOR2     0

// Motor shield использует четыре контакта 4, 5, 6, 7 для управления моторами
// 4 и 7 — для направления, 5 и 6 — для скорости
#define SPEED_1      5
#define DIR_1        4

#define SPEED_2      6
#define DIR_2        7

// Возможные направления движения робота
#define FORWARD   0
#define BACKWARD  1
#define LEFT      2
#define RIGHT     3

int command; //Int to store app command state.
int Speed = 204; // 0 - 255.
int LED = 13;
int Speedsec;
int buttonState = 0;
int lastButtonState = 0;
int Turnradius = 0; //Set the radius of a turn, 0 - 255 Note:the robot will malfunction if this is higher than int Speed.
int brakeTime = 45;
int brkonoff = 1; //1 for the electronic braking system, 0 for normal.
SoftwareSerial BTSerial(2, 3);


void setup() {
  // Настраивает выводы платы 4, 5, 6, 7 на вывод сигналов
    for(int i = 4; i <= 7; i++)
        pinMode(i, OUTPUT);
    pinMode(LED, OUTPUT); //Set the LED pin.
  BTSerial.begin(9600);  //Set the baud rate to your Bluetooth module.
}

void loop() {
  if (BTSerial.available() > 0) {
    command = BTSerial.read();
    Stop(); //Initialize with motors stoped.
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

void go(int newDirection, int speed)
{
  boolean motorDirection_1, motorDirection_2;

  switch ( newDirection ) {

    case FORWARD:

        motorDirection_1 = false;
        motorDirection_2 = true;


        break;
    case BACKWARD:

        motorDirection_1 = true;
        motorDirection_2 = false;

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