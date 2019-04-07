#include <Servo.h>

Servo speedController;
// ---------- ПОЛЬЗОВАТЕЛЬСКИЕ НАСТРОЙКИ ---------- //////////////////////
int flyTime = 10;       // Время полёта, сек                            //
int delayCutOff = 3;    // Задержка перед отключением, сек              //
int idle = 10;          // Нулевое значение газа                        //
int fullPower = 150;    // Максимальная мощность                        //
//////////////////////////////////////////////////////////////////////////
int throttle;

void setup() {
  pinMode(A0, INPUT_PULLUP);    // Вход кнопки
  speedController.attach(3);    // Сигнал на регулятор с цифрового выхода 2

  if (!digitalRead(A0)) {       // Если при включении зажата кнопка
    calibration();              // Переходим в режим калибровки
  }
}

void loop() {
  throttle = idle;
  speedController.write(idle);
  delay(1000);

  if (!digitalRead(A0)) {
    btnHold();
  }
}


// ---------- Проверка удержания кнопки ----------
void btnHold() {
  int time2go = 3;        // Сколько нужно держать кнопку, сек.
  float timePast = 0.0;   // Счётчик времени

  while (!digitalRead(A0)) {
    delay(100);           // 0,1 сек интервал
    timePast += 0.1;
    if (timePast >= time2go) {
      motorStart();
    }
  }

}


void motorStart() {
  while (throttle < fullPower) {
    speedController.write(throttle++);
    delay(10);
  }

  flyingTimer();
}


// ---------- Режим полёта ----------
void flyingTimer() {
  delay(flyTime * 1000);
  alarm();          // Предупреждение
  delay(delayCutOff * 1000);      // Задержка перед отключением
  speedController.write(idle);
}


// ---------- Предупреждение о скором отключении ----------
void alarm() {
  speedController.write(fullPower * 0.5);
  delay(500);
  speedController.write(fullPower * 0.75);
  delay(500);
  speedController.write(fullPower * 0.5);
  delay(500);
  speedController.write(fullPower);
}


// ---------- Калибровка рабочего диапазона регулятора ----------
void calibration() {
  digitalWrite(13, HIGH);
  speedController.write(fullPower);
  delay(3000);
  speedController.write(idle);
  digitalWrite(13, LOW);

  while (!digitalRead(A0)) {        // Если зажата кнопка
    digitalWrite(13, HIGH);         // не даём выйти
    delay(100);                     // и мигаем светодиодом
    digitalWrite(13, LOW);
    delay(100);
  }

}
