/*
 * application.cpp
 *
 *  Created on: Jan 26, 2018
 *      Author: marti
 */

#include "application.h"
#include "GUI.h"
#include "stm32l0xx_hal.h"

GUI gui(display);

EXTERNC void runGUITask() {
  gui.run();
}

/**
  * @brief  Alarm callback
  * @param  hrtc : RTC handle
  * @retval None
  */
EXTERNC void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{
  RTC_TimeTypeDef time;
  RTC_DateTypeDef date;

  if(HAL_RTC_GetTime(hrtc, &time, RTC_FORMAT_BIN) != HAL_OK)
    {
      _Error_Handler(__FILE__, __LINE__);
    }
  HAL_RTC_GetDate(hrtc, &date, RTC_FORMAT_BCD);

  gui.setTime(time);
}
