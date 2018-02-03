/*
 * GUI.cpp
 *
 *  Created on: Jan 26, 2018
 *      Author: marti
 */

#include <GUI.h>

GUI::GUI (u8g2_t &display, TimeKeeper &timekeeper) : display(display), timeKeeper(timekeeper)
{
  eventGroupHandle = xEventGroupCreateStatic( &eventGroup );
}

GUI::~GUI ()
{
  vEventGroupDelete(eventGroupHandle);
}

void GUI::run()
{
  const TickType_t xTicksToWait = 600000 / portTICK_PERIOD_MS;

  for(;;) {
      /* Wait a maximum of 100ms for either bit to be set within
      the event group.  Clear the bits before exiting. */

      EventBits_t evBits = xEventGroupWaitBits(
                eventGroupHandle,   // The event group being tested.
                TIME_CHANGED_BIT, // The bits within the event group to wait for.
                pdTRUE,        // bits should be cleared before returning.
                pdFALSE,       // Don't wait for all bits, either bit will do.
                xTicksToWait );// Wait a maximum of 100ms for either bit to be set.

      if(evBits & TIME_CHANGED_BIT) {
	  time = timeKeeper.getTime();
      }

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

void GUI::timeChanged() {
  BaseType_t xHigherPriorityTaskWoken, xResult;
  // xHigherPriorityTaskWoken must be initialized to pdFALSE.
  xHigherPriorityTaskWoken = pdFALSE;

  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);

  // Set bit in xEventGroup.
  xResult = xEventGroupSetBitsFromISR(
                              eventGroupHandle,   // The event group being updated.
                              TIME_CHANGED_BIT, // The bits being set.
                              &xHigherPriorityTaskWoken );

  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);

  // Was the message posted successfully?
  if( xResult != pdFAIL )
  {
      // If xHigherPriorityTaskWoken is now set to pdTRUE then a context
      // switch should be requested.
      portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
  }
}
