// игра змейка!
#if (USE_WALKING_PIXEL == 1)
// **************** ДЛЯ РАЗРАБОТЧИКОВ ****************
int8_t wp_X, wp_Y, wp_nextX, wp_nextY;

void walkingPixelRoutine() {
  if (loadingFlag) {
    FastLED.clear();
    loadingFlag = false;
    gamemodeFlag = true;
    modeCode = 2;
    st_newGameWP();
  }

  buttonsTickWalkingPixel();

  if (gameTimer.isReady()) {
    if (gameDemo)
      walkDemo();

    // смещение
    wp_nextX += wp_X;
    wp_nextY += wp_Y;
    if (wp_nextX != wp_X || wp_nextY != wp_Y) {
      goNext();
    }
  }
}

void goNext() {
  // если внутри границ поля
  if (wp_nextX >= 0 && wp_nextX < WIDTH && wp_nextY >= 0 && wp_nextY < HEIGHT) {
    uint32_t new_color;
    do {
      new_color = colors[random(6)];
    } while (new_color == color);

    color = new_color;
    drawPixelXY(wp_X, wp_Y, 0x000000);
    drawPixelXY(wp_nextX, wp_nextY, color);
    FastLED.show();
    wp_X = wp_nextX;
    wp_Y = wp_nextY;
    wp_nextX = 0;
    wp_nextY = 0;
  }
}

void walkDemo() {
  wp_nextX = random(-1, 2);
  wp_nextY = 0;
  if (!wp_nextX)
    wp_nextY = random(-1, 2) || 1;
}

void buttonsTickWalkingPixel() {
  // 0 - право, 1 - лево, 2 - вверх, 3 - вниз
  if (checkButtons(false, true)) {
    if (buttons == 3) {   // кнопка нажата
      wp_nextX = - 1;
      wp_nextY = 0;
      buttons = 4;
    }
    if (buttons == 1) {   // кнопка нажата
      wp_nextX = 1;
      wp_nextY = 0;
      buttons = 4;
    }
    if (buttons == 0) {   // кнопка нажата
      wp_nextY = 1;
      wp_nextX = 0;
      buttons = 4;
    }
    if (buttons == 2) {   // кнопка нажата
      wp_nextY = -1;
      wp_nextX = 0;
      buttons = 4;
    }
  }
}

// новый раунд
void st_newGameWP() {
  Serial.println("lolkek");   // без этого работает некорректно! магия ебаная
  delay(10);
  randomSeed(millis());

  color = colors[random(6)];      // случайный цвет
  // возвращаем обычную скорость падения
  gameTimer.setInterval(gameSpeed);
  delay(10);
}


#elif (USE_WALKING_PIXEL == 0)
void walkingPixelRoutine() {
  return;
}
#endif
