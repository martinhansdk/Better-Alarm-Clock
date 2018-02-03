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

#include "FreeRTOS.h"
#include "event_groups.h"

#include "timekeeper.h"

class GUI
{
  u8g2_t &display;
  TimeKeeper &timeKeeper;
  RTC_TimeTypeDef time;

  // event groups
  const unsigned long int TIME_CHANGED_BIT = (1 << 0);
  EventGroupHandle_t eventGroupHandle;
  StaticEventGroup_t eventGroup;

public:
  GUI (u8g2_t &display, TimeKeeper &timeKeeper);
  virtual ~GUI ();
  void run();
  void timeChanged();

private:
  void draw();
};

#endif /* DISPLAY_GUI_H_ */
