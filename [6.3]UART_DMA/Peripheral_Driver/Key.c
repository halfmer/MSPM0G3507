#include "Key.h"

//KEY的GPIO口初始化函数
void KEY_GPIO_init(void)
{
    DL_GPIO_initDigitalInputFeatures(GPIO_KEY_IOMUX,
                                     DL_GPIO_INVERSION_DISABLE,
                                     DL_GPIO_RESISTOR_PULL_UP,
                                     DL_GPIO_HYSTERESIS_DISABLE,
                                     DL_GPIO_WAKEUP_DISABLE);
}
//按键输入函数，mode表示模式
//1为允许连续按
//0为不允许连续按
uint8_t keyscan(uint8_t mode)
{
    uint8_t key_temp = 0;
    static uint8_t key_up = 1;  // 按键按松开标志
    
    if (mode) key_up = 1;       /* 支持连按 */
    
    if(KEY == 0 && key_up)
    {
        Delay_ms(50);//去除抖动
        key_up =0;
        if(KEY == 0){key_temp |= KEY_Value;}
    }
    else if(KEY == GPIO_KEY_PIN)
    {
        key_up = 1;
    }
    
    return key_temp;
}
