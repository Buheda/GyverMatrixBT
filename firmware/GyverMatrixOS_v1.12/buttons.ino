
bool isRightLeftKeysOnly = false;
bool isDublicatedKeys = false;

#if (USE_BUTTONS == 1)
#include "GyverButton.h"

#if analogKeyBoard==1

GButton bt_rright, bt_rup, bt_rdown, bt_rleft;
GButton bt_right, bt_up, bt_down, bt_left;
GButton t2;
GButton bt_adm(BUTT_ADM_SET);
GButton bt_set;

#elif
GButton bt_left(BUTT_LEFT);
GButton bt_right(BUTT_RIGHT);
GButton bt_up(BUTT_UP);

#if (MCU_TYPE == 1)   // у esp8266 пин 8 подтянут к земле
GButton bt_set(BUTT_SET, LOW_PULL);
#else
GButton bt_set(BUTT_SET);
#endif
#endif

timerMinim stepTimer(100);
#endif

void setupButtons() {
#if analogKeyBoard == 1
  /*  bt_rright.setType(LOW_PULL); bt_rup.setType(LOW_PULL); bt_rdown.setType(LOW_PULL); bt_rleft.setType(LOW_PULL);
    bt_right.setType(LOW_PULL); bt_up.setType(LOW_PULL); bt_down.setType(LOW_PULL); bt_left.setType(LOW_PULL);*/
  bt_set.setType(LOW_PULL);
  t2.setType(LOW_PULL);
  bt_adm.setDirection(NORM_CLOSE);
#endif
}

bool isLockedMode() {
  return bt_adm.isHold();
}

bool checkIsSETBtn(bool _isHold = false) {
  if (_isHold) {
    return bt_set.isHold();
  } else {
    return bt_set.isPress();
  }
}

bool checkIsLeftBtns(bool _isHold = false) {
  if (_isHold) {
    if (isRightLeftKeysOnly) {
      return bt_right.isHold() || bt_up.isHold() || bt_down.isHold() || bt_left.isHold();
    } else {
      return bt_left.isHold() || (isDublicatedKeys && bt_rleft.isHold());
    }
  } else {
    if (isRightLeftKeysOnly) {
      return bt_right.isPress() || bt_up.isPress() || bt_down.isPress() || bt_left.isPress();
    } else {
      return bt_left.isPress() || (isDublicatedKeys && bt_rleft.isPress());
    }
  }
}

bool checkIsRightBtns(bool _isHold = false) {
  if (_isHold) {
    if (isRightLeftKeysOnly) {
      return bt_rright.isHold() || bt_rup.isHold() || bt_rdown.isHold() || bt_rleft.isHold();
    } else {
      return bt_right.isHold() || (isDublicatedKeys && bt_rright.isHold());
    }
  } else {
    if (isRightLeftKeysOnly) {
      return bt_rright.isPress() || bt_rup.isPress() || bt_rdown.isPress() || bt_rleft.isPress();
    } else {
      return bt_right.isPress() || (isDublicatedKeys && bt_rright.isPress());
    }
  }
}

bool checkIsUpBtns(bool _isHold = false) {
  if (isRightLeftKeysOnly)
    return false;

  if (_isHold) {
    return bt_up.isHold() || (isDublicatedKeys && bt_rup.isHold());
  } else {
    return bt_up.isPress() || (isDublicatedKeys && bt_rup.isPress());
  }

}

bool checkIsDownBtns(bool _isHold = false) {
  if (isRightLeftKeysOnly)
    return false;

  if (_isHold) {
    return bt_down.isHold() || (isDublicatedKeys && bt_rdown.isHold());
  } else {
    return bt_down.isPress() || (isDublicatedKeys && bt_rdown.isPress());
  }
}

void analogKeysTick() {
#if USE_BUTTONS == 1
#if analogKeyBoard == 1
  int analog = analogRead(KEY_PIN);

  bt_set.tick(analog > 400 && analog < 450);
  bt_left.tick(analog > 210 && analog < 290);
  bt_right.tick(analog > 290 && analog < 350);
  bt_up.tick(analog > 5 && analog < 105);
  bt_down.tick(analog > 150 && analog < 180);

  bt_rleft.tick(analog > 460 && analog < 520);
  bt_rright.tick(analog > 660 && analog < 705);
  bt_rup.tick(analog > 520 && analog < 620);
  bt_rdown.tick(analog > 705 && analog < 780);

  /*bt_set.tick(analog > 380 && analog < 410);
  bt_left.tick((analog > 170 && analog < 240) || (analog > 90 && analog < 140));
  bt_right.tick(analog > 240 && analog < 380);
  bt_up.tick(analog > 5 && analog < 90);
  bt_down.tick(analog > 140 && analog < 170);

  bt_rleft.tick(analog > 420 && analog < 480);
  bt_rright.tick(analog > 600 && analog < 660);
  bt_rup.tick(analog > 480 && analog < 590);
  bt_rdown.tick(analog > 660 && analog < 750);*/


  bt_adm.tick();
#elif
  bt_left.tick();
  bt_right.tick();
  bt_up.tick();
  bt_down.tick();
#endif
#endif
}

boolean checkButtons(bool _isRightLeftKeysOnly = false, bool _isDublicatedKeys = true) {
  isRightLeftKeysOnly = _isRightLeftKeysOnly;
  isDublicatedKeys = _isDublicatedKeys;

#if (USE_BUTTONS == 1)
  if (!gameDemo) {
    if (checkIsLeftBtns()) {
#if DEBUG
      Serial.println(F("bt_left.isPress"));
#endif
      buttons = 3;
      controlFlag = true;
    }
    if (checkIsRightBtns()) {
#if DEBUG
      Serial.println(F("bt_right.isPress"));
#endif
      buttons = 1;
      controlFlag = true;
    }
    if (checkIsUpBtns()) {
#if DEBUG
      Serial.println(F("bt_up.isPress"));
#endif
      buttons = 0;
      controlFlag = true;
    }
    if (checkIsDownBtns()) {
#if DEBUG
      Serial.println(F("btbbtn_down.isPress"));
#endif
      if (stepTimer.isReady())
        buttons = 2;
    }

    if (checkIsLeftBtns(true)) {
#if DEBUG
      Serial.println(F("bt_left.isHold"));
#endif
      if (stepTimer.isReady())
        buttons = 3;
    }
    if (checkIsRightBtns(true)) {
#if DEBUG
      Serial.println(F("bt_right.isHold"));
#endif
      if (stepTimer.isReady())
        buttons = 1;
    }
    if (checkIsUpBtns(true)) {
#if DEBUG
      Serial.println(F("bt_up.isHold"));
#endif
      if (stepTimer.isReady())
        buttons = 0;
    }
    if (checkIsDownBtns(true)) {
#if DEBUG
      Serial.println(F("bt_down.isHold"));
#endif
      if (stepTimer.isReady())
        buttons = 2;
    }
  }
#endif

  // если нажали кнопку, возврат к обычному режиму
  if (controlFlag) {
    idleTimer.reset();
    controlFlag = false;
    idleState = false;
    gameDemo = false;
    return true;
  }

  if (buttons != 4) return true;
  return false;
}
