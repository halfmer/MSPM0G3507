/*
 * OLED.c
 *
 *  Created on: Jul 3, 2023
 *      The original author: 北辰远_code
 *
 *  modified on: Jul 16, 2025
 *      modifier: halfmer
 */

//用的是6针OLED，分别为VCC,GND,SCL,SDA,RST,D/C
//CS默认接地,用SPI通讯
/*引脚连接情况:
 *SCL:PA12
 *SDA:PA14
 *RST:PA15
 *D/C:PA13
 */

#ifndef OLED_H_
#define OLED_H_

#include "ti_msp_dl_config.h"


#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t
//-----------------OLED端口定义----------------
//#define OLED_RST_Clr() HAL_GPIO_WritePin(OLED_RST_GPIO_Port, OLED_RST_Pin, GPIO_PIN_RESET)   //RST
//#define OLED_RST_Set() HAL_GPIO_WritePin(OLED_RST_GPIO_Port, OLED_RST_Pin, GPIO_PIN_SET)   //RST

//#define OLED_RS_Clr() HAL_GPIO_WritePin(OLED_RS_GPIO_Port, OLED_RS_Pin, GPIO_PIN_RESET)    //DC
//#define OLED_RS_Set() HAL_GPIO_WritePin(OLED_RS_GPIO_Port, OLED_RS_Pin, GPIO_PIN_SET)    //DC

//#define OLED_SCLK_Clr()  HAL_GPIO_WritePin(OLED_SCLK_GPIO_Port, OLED_SCLK_Pin, GPIO_PIN_RESET)  //SCL
//#define OLED_SCLK_Set()  HAL_GPIO_WritePin(OLED_SCLK_GPIO_Port, OLED_SCLK_Pin, GPIO_PIN_SET)  //SCL

//#define OLED_SDIN_Clr()  HAL_GPIO_WritePin(OLED_SDIN_GPIO_Port, OLED_SDIN_Pin, GPIO_PIN_RESET)   //SDA
//#define OLED_SDIN_Set()  HAL_GPIO_WritePin(OLED_SDIN_GPIO_Port, OLED_SDIN_Pin, GPIO_PIN_SET)   //SDA

/* Port definition for Pin Group GPIO_OLED */
#ifndef GPIO_OLED_PORT
#define GPIO_OLED_PORT                                                   (GPIOA)
#endif

/* Defines for RST: GPIOA.15 with pinCMx 37 on package pin 8 */
#ifndef GPIO_OLED_RST_PIN
#define GPIO_OLED_RST_PIN                                       (DL_GPIO_PIN_15)
#define GPIO_OLED_RST_IOMUX                                      (IOMUX_PINCM37)
#endif

/* Defines for DC: GPIOA.13 with pinCMx 35 on package pin 6 */
#ifndef GPIO_OLED_DC_PIN
#define GPIO_OLED_DC_PIN                                        (DL_GPIO_PIN_13)
#define GPIO_OLED_DC_IOMUX                                       (IOMUX_PINCM35)
#endif

/* Defines for SCL: GPIOA.12 with pinCMx 34 on package pin 5 */
#ifndef GPIO_OLED_SCL_PIN
#define GPIO_OLED_SCL_PIN                                       (DL_GPIO_PIN_12)
#define GPIO_OLED_SCL_IOMUX                                      (IOMUX_PINCM34)
#endif

/* Defines for SDA: GPIOA.14 with pinCMx 36 on package pin 7 */
#ifndef GPIO_OLED_SDA_PIN
#define GPIO_OLED_SDA_PIN                                       (DL_GPIO_PIN_14)
#define GPIO_OLED_SDA_IOMUX                                      (IOMUX_PINCM36)
#endif

/* clang-format on */


#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据
//OLED控制用函数
void OLED_WR_Byte(u8 dat,u8 cmd);
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Refresh_Gram(void);
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode);
void OLED_ShowNumber(u8 x,u8 y,u32 num,u8 len,u8 size);
void OLED_ShowString(u8 x,u8 y,const u8 *p);
void OLED_ShowFloatNum(u8 x,u8 y,float num,u8 size1);
void OLED_ShowFloat(float value, uint8_t decimalPlaces, uint8_t x, uint8_t y);
void OLED_ShowFNum(u8 x,u8 y,float num,u8 len,u8 size,u8 mode);

#endif /* OLED_H_ */
