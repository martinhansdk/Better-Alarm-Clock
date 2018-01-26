/*
 * display_hw.hpp
 *
 *  Created on: Jan 23, 2018
 *      Author: marti
 */

#ifndef DISPLAY_DISPLAY_HW_H_
#define DISPLAY_DISPLAY_HW_H_

#include "u8g2.h"
#include "stm32l0xx_hal.h"

extern u8g2_t display;
extern void initDisplay();
extern void draw(u8g2_t *u8g2, RTC_HandleTypeDef *hrtc);

#endif /* DISPLAY_DISPLAY_HW_H_ */
