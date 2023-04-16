#include <NewPing.h>

byte PIN_TRIG_LEFT = 5;
byte PIN_ECHO_LEFT = 6;

byte PIN_TRIG_RIGHT = 3;
byte PIN_ECHO_RIGHT = 4;

byte PIN_TRIG_PRIMARY = 7;
byte PIN_ECHO_PRIMARY = 8;

byte MAX_DISTANCE = 30;

NewPing sonar_left(PIN_TRIG_LEFT, PIN_ECHO_LEFT, MAX_DISTANCE);
NewPing sonar_right(PIN_TRIG_RIGHT, PIN_ECHO_RIGHT, MAX_DISTANCE);
NewPing sonar_primary(PIN_TRIG_PRIMARY, PIN_ECHO_PRIMARY, MAX_DISTANCE);


byte LED_BACK=12;
byte LED_RIGHT=11;
byte LED_LEFT=10;


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
  pinMode(LED_BACK, OUTPUT);
  pinMode(LED_RIGHT, OUTPUT);
  pinMode(LED_LEFT, OUTPUT);
  Serial.begin(9600);

  //Установки Таймер2: Делитель частоты /8, режим 0
//Частота = 16MHz/8 = 2Mhz или 0.5 мкс
//Делитель /8 дает нам хороший рабочий диапазон
//так что сейчас мы просто жестко запрограммируем это.

  TCCR1A = (1<<WGM11); // сброс по совпадению
  TCCR1B |= 0<<CS10 | 1<<CS11 | 0<<CS12; 

  TIMSK1 |= (1<< OCIE1A); //подключение прерывания по переполнению 
   sei(); //запуск прерываний
}
void TurnPovorotnik(byte LED) {
  timerLED1On = 1;
    cli();
    timerLED1Loop = timerLED1;
    sei();
    if(timerLED1Loop >= 150)
    {
      if(LEDOn == 0)
      {
        LEDOn = 1;
        digitalWrite(LED, LEDOn);
       }
      else
      {
        LEDOn = 0;
        digitalWrite(LED, LEDOn);
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

  // unsigned int distanceSm_primary = sonar_primary.ping(); // Создание сигнала, получение параметра его продолжительности в мкс (uS).
  // unsigned int distance_primary = distanceSm_primary / US_ROUNDTRIP_CM;
  // Serial.print("Ping PRIMARY: ");
  // Serial.print(distance_primary); // Пересчет параметра времени в величину расстояния и вывод результата (0 соответствует выходу за допустимый предел)
  // Serial.println(" cm");
  // delay(50);
  if ((distance_left > 0 && distance_right > 0) && distance_left < 10 && distance_right < 10) {
    digitalWrite(LED_BACK, HIGH);
    }
  else if (distance_left > 0 && distance_left < 10) {
    digitalWrite(LED_BACK, LOW);
    TurnPovorotnik(LED_RIGHT);
  }

  else if (distance_right > 0 && distance_right < 10) {
    digitalWrite(LED_BACK, LOW);
    TurnPovorotnik(LED_LEFT);
  }
  else {
    digitalWrite(LED_RIGHT, LOW);
    digitalWrite(LED_LEFT, LOW);
    digitalWrite(LED_BACK, LOW);
  }
  

  
  delay(300);
}
