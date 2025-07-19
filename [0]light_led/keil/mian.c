#include "ti_msp_dl_config.h"

int main(void)
{
    SYSCFG_DL_init();//初始化
    
    while(1)
    {
        DL_GPIO_clearPins(GPIOB,DL_GPIO_PIN_14);//关闭led
        delay_cycles(8000000);//1/4s
        DL_GPIO_setPins(GPIOB,DL_GPIO_PIN_14);//打开led
        delay_cycles(8000000);//1/4s
    }
}
