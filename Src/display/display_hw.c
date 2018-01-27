/*
 * display_hw.cpp
 *
 *  Created on: Jan 20, 2018
 *      Author: marti
 */

#include "display_hw.h"

#include "main.h"
#include "stm32l0xx_hal.h"
#include "stm32l0xx_hal_gpio.h"

/*
 * Pin mapping (HW SPI):
 *
 *  SCK: PA5 (D13)
 *  MOSI: PA7 (D11)
 *  CS:   PB6 (D10)
 *  D/C:  PC7 (D9)
 *
 *
 * Pin mapping (SW SPI):
 *
 *  SCK:  PA9 (D8)
 *  MOSI: PA8 (D7)
 *  CS:   PB6 (D10)
 *  D/C:  PC7 (D9)
 *
 *
 */

//extern SPI_HandleTypeDef hspi1;

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
	  //			HAL_SPI_Transmit(&hspi1, &byte, 1, 100);
	}
      break;
    case U8X8_MSG_BYTE_SET_DC:
      if(arg_int == 0){HAL_GPIO_WritePin(
	  DC_GPIO_Port, DC_Pin, GPIO_PIN_RESET);
      }
      if(arg_int == 1){HAL_GPIO_WritePin(
	  DC_GPIO_Port, DC_Pin, GPIO_PIN_SET);
      }
      break;
    case U8X8_MSG_BYTE_START_TRANSFER:
      HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET);
      asm("NOP");
      break;
    case U8X8_MSG_BYTE_END_TRANSFER:
      asm("NOP");
      HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET);
      break;
    default:
      return 0;
  }
  return 1;
}

uint8_t u8g2_gpio_and_delay_stm32(U8X8_UNUSED u8x8_t *u8x8, U8X8_UNUSED uint8_t msg,U8X8_UNUSED uint8_t arg_int, U8X8_UNUSED void *arg_ptr)
{
  // this is called to set the direction of pins and default values - which we do elsewhere
  // and to reset the display - which we don't do

  switch(msg){
    //Initialize SPI peripheral
    case U8X8_MSG_GPIO_AND_DELAY_INIT:
      /*
		__HAL_RCC_GPIOB_CLK_ENABLE();
		// SPI SCK, MOSI GPIO pin configuration
		GPIO_InitStruct.Pin = GPIO_PIN_3 | GPIO_PIN_5;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_PULLDOWN;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

		RST_LCD_GPIO_CLK_ENABLE();
		GPIO_InitStruct.Pin = RST_LCD_PIN;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		HAL_GPIO_Init(RST_LCD_PORT, &GPIO_InitStruct);

		CS_LCD_GPIO_CLK_ENABLE();
		GPIO_InitStruct.Pin = CS_LCD_PIN;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		HAL_GPIO_Init(CS_LCD_PORT, &GPIO_InitStruct);

		CD_LCD_GPIO_CLK_ENABLE();
		GPIO_InitStruct.Pin = CD_LCD_PIN;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		HAL_GPIO_Init(CD_LCD_PORT, &GPIO_InitStruct);
       */
      break;

      //Function which implements a delay, arg_int contains the amount of ms
    case U8X8_MSG_DELAY_MILLI:
      HAL_Delay(arg_int);

      break;
      //Function which delays 10us
    case U8X8_MSG_DELAY_10MICRO:
      for (uint16_t n = 0; n < 320; n++)
	{
	  __NOP();
	}

      break;
      //Function which delays 100ns
    case U8X8_MSG_DELAY_100NANO:
    case U8X8_MSG_DELAY_NANO:
      __NOP();

      break;
      //Function to define the logic level of the clockline
    case U8X8_MSG_GPIO_SPI_CLOCK:
      HAL_GPIO_WritePin(SW_SCK_GPIO_Port, SW_SCK_Pin, arg_int ? SET : RESET);
      break;
      //Function to define the logic level of the data line to the display
    case U8X8_MSG_GPIO_SPI_DATA:
      HAL_GPIO_WritePin(SW_MOSI_GPIO_Port, SW_MOSI_Pin, arg_int ? SET : RESET);
      break;
      // Function to define the logic level of the CS line
    case U8X8_MSG_GPIO_CS:
      HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, arg_int ? SET : RESET);
      break;
      //Function to define the logic level of the Data/ Command line
    case U8X8_MSG_GPIO_DC:
      HAL_GPIO_WritePin(DC_GPIO_Port, DC_Pin, arg_int ? SET : RESET);
      break;
      //Function to define the logic level of the RESET line
    case U8X8_MSG_GPIO_RESET:
      // We don't have a reset line
      break;
    default:
      return 0; //A message was received which is not implemented, return 0 to indicate an error
  }

  return 1; // command processed successfully.
}

u8g2_t display;

void initDisplay() {
  u8g2_Setup_sh1106_128x64_noname_f(&display, U8G2_R0, u8x8_byte_4wire_sw_spi, u8g2_gpio_and_delay_stm32);
  u8g2_InitDisplay(&display);
  u8g2_SetPowerSave(&display, 0);

}




