#include "ti_msp_dl_config.h"
#include "LED.h"
#include "Key.h"


int main(void)
{

    
    SYSCFG_DL_init();
    LED_GPIO_init();
    KEY_GPIO_init();
    
//    //开启计时，在通道0引脚和通道1引脚用示波器可以测出占空比（PA0已损坏，无法测出，可以换引脚）
    DL_TimerA_startCounter(PWM_0_INST);
    
    while(1);
}

