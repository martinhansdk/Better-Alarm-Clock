/*
 * timerkeeper.h
 *
 *  Created on: Jan 29, 2018
 *      Author: marti
 */

#ifndef DISPLAY_TIMEKEEPER_H_
#define DISPLAY_TIMEKEEPER_H_

#include "stm32l0xx_hal.h"

class TimeKeeper {
public:
  virtual RTC_TimeTypeDef getTime() = 0;
  virtual ~TimeKeeper() {}
};


#endif /* DISPLAY_TIMEKEEPER_H_ */
