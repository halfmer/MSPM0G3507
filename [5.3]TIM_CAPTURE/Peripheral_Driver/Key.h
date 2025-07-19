#ifndef _KEY_H
#define _KEY_H

//本代码驱动KEY是适用于MSP-LITO-G3507系统板
//该板KEY2连接PB21引脚
#include "ti_msp_dl_config.h"
#include "LED.h"

/* Port definition for Pin Group GPIO_LED */
#define GPIO_KEY                                      (GPIOB)

/* Defines for PIN_14: GPIOB.14 with pinCMx 31 on package pin 2 */
#define GPIO_KEY_PIN                                  (DL_GPIO_PIN_21)
#define GPIO_KEY_IOMUX                                (IOMUX_PINCM49)

#define KEY DL_GPIO_readPins(GPIO_KEY,GPIO_KEY_PIN)

#define KEY_Value   0x01

void KEY_GPIO_init(void);
uint8_t keyscan(uint8_t mode);

#endif
