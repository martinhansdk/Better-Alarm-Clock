/*
 * GUI.h
 *
 *  Created on: Jan 26, 2018
 *      Author: marti
 */

#ifndef DISPLAY_GUI_H_
#define DISPLAY_GUI_H_

#include "u8g2.h"
#include "stm32l0xx_hal.h"

class GUI
{
  u8g2_t &display;
  RTC_TimeTypeDef time;

public:
  GUI (u8g2_t &display);
  virtual ~GUI ();
  void run();
  void setTime(RTC_TimeTypeDef t);

private:
  void draw();
};

#endif /* DISPLAY_GUI_H_ */
