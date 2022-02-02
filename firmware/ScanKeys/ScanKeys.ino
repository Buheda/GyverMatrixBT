// Пример использования библиотеки GyverButton с аналоговой клавиатурой
// аналоговая клавиатура подключена на А7
// Схему смотри на странице библиотеки https://alexgyver.ru/gyverbutton/
// также она есть в папке с примером
#include "GyverButton.h"
// создаём кнопки без привязки к пину
GButton rr, ru, rd, rl;
GButton lr, lu, ld, ll;
GButton t1, t2;
void setup() {
  Serial.begin(9600);
  // меняем тип на LOW_PULL, потому что по умолчанию стоит HIGH_PULL

/*  rr.setType(LOW_PULL); ru.setType(LOW_PULL); rd.setType(LOW_PULL); rl.setType(LOW_PULL);
  lr.setType(LOW_PULL); lu.setType(LOW_PULL); ld.setType(LOW_PULL); ll.setType(LOW_PULL);
  t1.setType(LOW_PULL);
  t2.setType(LOW_PULL);*/
}



void loop() {
  // читаем значение
  int analog = analogRead(A0);
  // для начала нужно вывести и запомнить значение для каждой кнопки

  //t1 400-460
  // проверяем у каждой кнопки свой диапазон (+- 20 от полученного значения)
  lr.tick(analog > 330 && analog < 380);
  lu.tick(analog > 5 && analog < 105);
  ll.tick(analog > 105 && analog < 160);
  ld.tick(analog > 180 && analog < 210);

//t2 800-840
  rr.tick(analog > 720 && analog < 800);
  ru.tick(analog > 600 && analog < 680);
  rl.tick(analog > 480 && analog < 520);
  rd.tick(analog > 860 && analog < 960);

  
  if (analog < 10)
    return;
  Serial.println(analog);


  
  if (lr.isPress()) Serial.println("Click lr");         // проверка на один клик
  if (lu.isPress()) Serial.println("Click lu");         // проверка на один клик
  if (ll.isPress()) Serial.println("Click ll");         // проверка на один клик
  if (ld.isPress()) Serial.println("Click ld");         // проверка на один клик


  
  if (rr.isPress()) Serial.println("Click rr");         // проверка на один клик
  if (ru.isPress()) Serial.println("Click ru");         // проверка на один клик
  if (rl.isPress()) Serial.println("Click rl");         // проверка на один клик
  if (rd.isPress()) Serial.println("Click rd");         // проверка на один клик
  delay(10);  // задержка тут не нужна, чисто для вывода
}
