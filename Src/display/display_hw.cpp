/*
 * display_hw.cpp
 *
 *  Created on: Jan 20, 2018
 *      Author: marti
 */

#include "u8g2.h"


uint8_t u8x8_byte_4wire_hw_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  uint8_t byte;
  uint8_t *data;

  switch(msg)
  {
    case U8X8_MSG_BYTE_SEND:
      data = (uint8_t *)arg_ptr;
      while( arg_int > 0 )
      {
        byte = *data;
        data++;
        arg_int--;
        HAL_SPI_Transmit(&hspi1, &byte, 1, 100);
      }
      break;
    case U8X8_MSG_BYTE_START_TRANSFER:
      HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_SET);
      __nop(); // 21 ns
      break;
    case U8X8_MSG_BYTE_END_TRANSFER:
      __nop(); // 21 ns
      HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_RESET);
      break;
    default:
      return 0;
  }
  return 1;
}
