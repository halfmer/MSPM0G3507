#ifndef _LED_H
#define _LED_H

//本代码驱动LED是适用于MSP-LITO-G3507系统板
//该板LED1连接PB14引脚
#include "ti_msp_dl_config.h"

/* Port definition for Pin Group GPIO_LED */
#define GPIO_LED                                      (GPIOB)

/* Defines for PIN_14: GPIOB.14 with pinCMx 31 on package pin 2 */
#define GPIO_LED_PIN                                  (DL_GPIO_PIN_14)
#define GPIO_LED_IOMUX                                (IOMUX_PINCM31)

#define LED_ON()          DL_GPIO_setPins(GPIO_LED,GPIO_LED_PIN)
#define LED_OFF()         DL_GPIO_clearPins(GPIO_LED,GPIO_LED_PIN)
#define LED_Toggle()      DL_GPIO_togglePins(GPIO_LED,GPIO_LED_PIN)

#define DELAY_MS             (32000)//内部时钟CLK为32M，1/1000即为1ms

void Delay_ms(unsigned int t);
void LED_GPIO_init(void);
void LED_Toggle_500ms(void);

#endif
