#include "LED.h"

//延时函数
void Delay_ms(unsigned int t)
{
    delay_cycles(t*DELAY_MS);
}
//LED的GPIO口初始化函数
void LED_GPIO_init(void)
{
    DL_GPIO_initDigitalOutput(GPIO_LED_IOMUX);

    DL_GPIO_clearPins(GPIO_LED, GPIO_LED_PIN);
    DL_GPIO_enableOutput(GPIO_LED, GPIO_LED_PIN);

}
//LED闪烁----每隔0.5s
void LED_Toggle_500ms(void)
{
    LED_Toggle();
    Delay_ms(500);
}
