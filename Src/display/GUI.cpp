/*
 * GUI.cpp
 *
 *  Created on: Jan 26, 2018
 *      Author: marti
 */

#include <GUI.h>

GUI::GUI (u8g2_t &display) : display(display)
{

}

GUI::~GUI ()
{

}

void GUI::run()
{
  for(;;) {
      draw();
  }
}

void GUI::draw()
{
  char buf[20];

  u8g2_ClearBuffer(&display);

  u8g2_SetFontMode(&display, 1);
  u8g2_SetFontDirection(&display, 0);

  u8g2_SetFont(&display, u8g2_font_logisoso32_tn);

  snprintf(&buf[0], sizeof(buf), "%d:%02d:%02d", time.Hours, time.Minutes, time.Seconds);

  u8g2_DrawStr(&display, 0, 48, buf);

  u8g2_SendBuffer(&display);
}

void GUI::setTime(RTC_TimeTypeDef t) {
  time = t;
}
