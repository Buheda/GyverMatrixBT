//игра  тетрис!
#if (USE_SIMPLE_TETRIS == 1)
// **************** НАСТРОЙКИ ТЕТРИС ****************
#define FAST_SPEED 20     // скорость падения при удержании "вниз" (меньше - быстрее)
#define STEER_SPEED 40    // скорость перемещения в бок при удержании кнопки (меньше - быстрее) на BT версии не работает!

// --------------------- ДЛЯ РАЗРАБОТЧИКОВ ----------------------
#include "include.h"

void st_tetrisRoutine() {
  if (loadingFlag) {
    FastLED.clear();
    loadingFlag = false;
    st_newGameTetris();
    gamemodeFlag = true;
    modeCode = 2;
  }

  if (checkButtons(isLockedMode())) {

    if (buttons == 3) {   // кнопка нажата
      buttons = 4;
      st_stepLeft();
    }

    if (buttons == 1) {
      buttons = 4;
      st_stepRight();
    }

    if (buttons == 2) {             // кнопка вниз удерживается
      buttons = 4;
      gameTimer.setInterval(FAST_SPEED);  // увеличить скорость
    }
  }

  /*
    if (bt_left.isStep()) {    // кнопка нажата и удерживается
    st_stepLeft();
    }
    if (bt_right.isStep()) {
    st_stepRight();
    }
  */

  if (gameTimer.isReady()) {        // главный таймер игры
    prev_height = height;

    if (!st_checkArea(0)) {            // проверяем столкновение с другими фигурами
      if (height >= HEIGHT - 2) {   // проиграл по высоте
        st_gameOverTetris();                 // игра окончена, очистить всё
        st_newGameTetris();                 // новый раунд
      } else {                      // если не достигли верха
        st_fixFigure();                // зафиксировать
        st_checkAndClear();            // проверить ряды и очистить если надо
        st_newGameTetris();                 // новый раунд
      }
    } else if (height == 0) {       // если достигли дна
      st_fixFigure();                  // зафиксировать
      st_checkAndClear();              // проверить ряды и очистить если надо
      st_newGameTetris();                   // новый раунд
    } else {                        // если путь свободен
      height--;                             // идём вниз
      st_redrawFigure(pos, prev_height);  // перерисовка
    }
  }
}

// поиск и очистка заполненных уровней
void st_checkAndClear() {
  linesToClear = 1;                 // счётчик заполненных строк по вертикали. Искусственно принимаем 1 для работы цикла
  boolean full_flag = true;         // флаг заполненности
  while (linesToClear != 0) {       // чисти чисти пока не будет чисто!
    linesToClear = 0;
    byte lineNum = 255;       // высота, с которой начинаются заполненные строки (искусственно увеличена)
    for (byte Y = 0; Y < HEIGHT; Y++) {   // сканируем по высоте
      full_flag = true;                   // поднимаем флаг. Будет сброшен, если найдём чёрный пиксель
      for (byte X = 0; X < WIDTH; X++) {  // проходимся по строкам
        if ((long)getPixColorXY(X, Y) == (long)0x000000) {  // если хоть один пиксель чёрный
          full_flag = false;                                 // считаем строку неполной
        }
      }
      if (full_flag) {        // если нашлась заполненная строка
        linesToClear++;       // увеличиваем счётчик заполненных строк
        if (lineNum == 255)   // если это первая найденная строка
          lineNum = Y;        // запоминаем высоту. Значение 255 было просто "заглушкой"
      } else {                // если строка не полная
        if (lineNum != 255)   // если lineNum уже не 255 (значит строки были найдены!!)
          break;              // покинуть цикл
      }
    }
    if (linesToClear > 0) {             // если найденных полных строк больше 1
      lineCleanCounter += linesToClear;   // суммируем количество очищенных линий (игровой "счёт")

      // заполняем весь блок найденных строк белым цветом слева направо
      for (byte X = 0; X < WIDTH; X++) {
        for (byte i = 0; i < linesToClear; i++) {
          leds[getPixelNumber(X, lineNum + i)] = CHSV(0, 0, 255);         // закрашиваем его белым
        }
        FastLED.show();
        delay(5);     // задержка между пикселями слева направо
      }
      delay(10);

      // теперь плавно уменьшаем яркость всего белого блока до нуля
      for (byte val = 0; val <= 30; val++) {
        for (byte X = 0; X < WIDTH; X++) {
          for (byte i = 0; i < linesToClear; i++) {
            leds[getPixelNumber(X, lineNum + i)] = CHSV(0, 0, 240 - 8 * val);  // гасим белый цвет
          }
        }
        FastLED.show();
        delay(5);       // задержка между сменой цвета
      }
      delay(10);

      // и теперь смещаем вниз все пиксели выше уровня с первой найденной строкой
      for (byte i = 0; i < linesToClear; i++) {
        for (byte Y = lineNum; Y < HEIGHT - 1; Y++) {
          for (byte X = 0; X < WIDTH; X++) {
            drawPixelXY(X, Y, getPixColorXY(X, Y + 1));      // сдвигаем вниз
          }
          FastLED.show();
        }
        delay(100);       // задержка между "сдвигами" всех пикселей на один уровень
      }
    }
  }
  gameTimer.reset();
}


// функция фиксации фигуры
void st_fixFigure() {
  color += ADD_COLOR;                   // чутка перекрасить
  st_redrawFigure(pos, prev_height);  // перерисовать
}

// проигрыш
void st_gameOverTetris() {
  FastLED.clear();
  FastLED.show();

  // тут можно вывести счёт lineCleanCounter
  if (!gameDemo) displayScore(lineCleanCounter);
  delay(1000);
  lineCleanCounter = 0;   // сброс счёта
  FastLED.clear();
  FastLED.show();
  delay(20);
}

// новый раунд
void st_newGameTetris() {
  Serial.println("lolkek");   // без этого работает некорректно! магия ебаная
  delay(10);
  buttons = 4;
  height = HEIGHT;    // высота = высоте матрицы
  pos = WIDTH / 2;    // фигура появляется в середине
  //color = colors[random(6)];      // случайный цвет

  color_index = ++color_index % 6;  // все цвета по очереди
  color = colors[color_index];

  // если включен демо-режим, позицию по горизонтали выбираем случайно
  if (gameDemo) pos = random(1, WIDTH - 1);

  // возвращаем обычную скорость падения
  gameTimer.setInterval(gameSpeed);
  down_flag = false;  // разрешаем ускорять кнопкой "вниз"
  delay(10);
}

// управление фигурами вправо и влево
void st_stepRight() {
  if (st_checkArea(1)) {
    prev_pos = pos;
    if (++pos > WIDTH) pos = WIDTH;
    st_redrawFigure(prev_pos, height);
  }
}
void st_stepLeft() {
  if (st_checkArea(2)) {
    prev_pos = pos;
    if (--pos < 0) pos = 0;
    st_redrawFigure(prev_pos, height);
  }
}

// проверка на столкновения
boolean st_checkArea(int8_t check_type) {
  // check type:
  // 0 - проверка лежащих фигур и пола
  // 1 - проверка стенки справа и фигур
  // 2 - проверка стенки слева и фигур
  // 3 - проверка обеих стенок и пола

  boolean flag = true;
  int8_t X = pos, Y = height;
      
  boolean offset = 1;

  // этот режим для проверки поворота. Поэтому "поворачиваем"
  // фигуру на следующий угол чтобы посмотреть, не столкнётся ли она с чем
  if (check_type == 3) {
    offset = 0;   // разрешаем оказаться вплотную к стенке
  }
  // границы поля
  if (check_type == 1 || check_type == 3) {
    if (X + 1 > WIDTH - 1) flag = false;    // смотрим следующий справа
    uint32_t getColor;
    if (Y < HEIGHT)
      getColor = getPixColorXY(X + offset, Y);
    if (getColor != 0x000000) {
      flag = false;         // если не СВОЙ цвет и не чёрный
    }
  }

  if (check_type == 2 || check_type == 3) {
    if (X - 1 < 0) flag = false;    // смотрим следующий слева
    uint32_t getColor;
    if (Y < HEIGHT)
      getColor = getPixColorXY(X - offset, Y);
    if (getColor != 0x000000) {
      flag = false;         // если не СВОЙ цвет и не чёрный
    }
  }

  if (check_type == 0 || check_type == 3) {
    uint32_t getColor;
    if (Y < HEIGHT) {
      getColor = getPixColorXY(X, Y - 1);
      if (getColor != color && getColor != 0x000000) {
        flag = false;         // если не СВОЙ цвет и не чёрный
      }
    }
  }
  return flag;    // возвращаем глобальный флаг, который говорит о том, столкнёмся мы с чем то или нет
}

// функция, которая удаляет текущую фигуру (красит её чёрным)
// а затем перерисовывает её в новом положении
void st_redrawFigure(int8_t clr_pos, int8_t clr_height) {
  st_drawFigure(clr_pos, clr_height, 0x000000);            // стереть фигуру
  st_drawFigure(pos, height, color);                           // отрисовать
  FastLED.show();
}

// функция, отрисовывающая фигуру заданным цветом и под нужным углом
void st_drawFigure( byte x, byte y, uint32_t color) {
  drawPixelXY(x, y, color);         // рисуем точку начала координат фигуры
}

#elif (USE_SIMPLE_TETRIS == 0)
void st_tetrisRoutine() {
  return;
}
#endif
