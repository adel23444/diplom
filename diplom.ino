#include <NewPing.h>


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

void go(int newDirection, int speed) 
{
  boolean motorDirection_1, motorDirection_2;
 
  switch ( newDirection ) {
 
    case FORWARD:
        motorDirection_1 = false;
        motorDirection_2 = true;
        break;
    case BACKWARD:
        motorDirection_1 = false;
        motorDirection_2 = true;
        break;        
    case LEFT:
        motorDirection_1 = true;
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
 

byte PIN_TRIG_LEFT = 9;
byte PIN_ECHO_LEFT = 8;

byte PIN_TRIG_RIGHT = 10;
byte PIN_ECHO_RIGHT = 11;

byte PIN_TRIG_PRIMARY = 3;
byte PIN_ECHO_PRIMARY = 4;

byte MAX_DISTANCE = 30;

NewPing sonar_left(PIN_TRIG_LEFT, PIN_ECHO_LEFT, MAX_DISTANCE);
NewPing sonar_right(PIN_TRIG_RIGHT, PIN_ECHO_RIGHT, MAX_DISTANCE);
NewPing sonar_primary(PIN_TRIG_PRIMARY, PIN_ECHO_PRIMARY, MAX_DISTANCE);



volatile unsigned long int timerLED1;
unsigned long int timerLED1Loop;
volatile bool timerLED1On;
bool LEDOn;
ISR (TIMER1_COMPA_vect)
{
  if(timerLED1On)
  	timerLED1++;
}

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);

  for(int i = 4; i <= 7; i++)     
        pinMode(i, OUTPUT);  

  //Установки Таймер2: Делитель частоты /8, режим 0
//Частота = 16MHz/8 = 2Mhz или 0.5 мкс
//Делитель /8 дает нам хороший рабочий диапазон
//так что сейчас мы просто жестко запрограммируем это.

  TCCR1A = (1<<WGM11); // сброс по совпадению
  TCCR1B |= 0<<CS10 | 1<<CS11 | 0<<CS12; 

  TIMSK1 |= (1<< OCIE1A); //подключение прерывания по переполнению 
   sei(); //запуск прерываний
}
void TurnPovorotnik(int Direction) {
  timerLED1On = 1;
    cli();
    timerLED1Loop = timerLED1;
    sei();
    if(timerLED1Loop >= 150)
    {
      if(LEDOn == 0)
      {
        LEDOn = 1;
        // медленно едем назад полторы секунды
        
       }
      else
      {
        go(Direction, 0);
      }
      cli();
      timerLED1=0;
      sei();
    }  
}
void loop() {

  unsigned int distanceSm_left = sonar_left.ping(); // Создание сигнала, получение параметра его продолжительности в мкс (uS).
  unsigned int distance_left = distanceSm_left / US_ROUNDTRIP_CM;
  Serial.print("Ping LEFT: ");
  Serial.print(distance_left); // Пересчет параметра времени в величину расстояния и вывод результата (0 соответствует выходу за допустимый предел)
  Serial.println(" cm");
  delay(50);

  unsigned int distanceSm_right = sonar_right.ping(); // Создание сигнала, получение параметра его продолжительности в мкс (uS).
  unsigned int distance_right = distanceSm_right / US_ROUNDTRIP_CM;
  Serial.print("Ping RIGHT: ");
  Serial.print(distance_right); // Пересчет параметра времени в величину расстояния и вывод результата (0 соответствует выходу за допустимый предел)
  Serial.println(" cm");
  delay(50);

  unsigned int distanceSm_primary = sonar_primary.ping(); // Создание сигнала, получение параметра его продолжительности в мкс (uS).
  unsigned int distance_primary = distanceSm_primary / US_ROUNDTRIP_CM;
  Serial.print("Ping PRIMARY: ");
  Serial.print(distance_primary); // Пересчет параметра времени в величину расстояния и вывод результата (0 соответствует выходу за допустимый предел)
  Serial.println(" cm");
  delay(50);
  if ((distance_left > 0 && distance_right > 0) || distance_left < 30 || distance_right < 30) {
    go(FORWARD, 190);
    delay(50);
  }
  if ((distance_left > 0 && distance_right > 0) && distance_left < 10 && distance_right < 10) {
    go(BACKWARD, 190);
    delay(50);
    }
  else if (distance_left > 0 && distance_left < 10) {
    go(RIGHT, 190);
    delay(50);
  }

  else if (distance_right > 0 && distance_right < 10) {
    go(LEFT, 190);
    delay(50);
  }
  else {
    // Едем секунду вперёд на максимальной скорости
    go(FORWARD, 250);
    delay(50);
  }
  

  delay(500);
}
