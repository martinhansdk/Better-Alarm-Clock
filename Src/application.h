/*
 * application.h
 *
 *  Created on: Jan 26, 2018
 *      Author: marti
 */

#ifndef APPLICATION_H_
#define APPLICATION_H_

#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif

#include "display_hw.h"

EXTERNC void runGUITask();
EXTERNC void guiSetTime(RTC_HandleTypeDef *hrtc);

#endif /* APPLICATION_H_ */
