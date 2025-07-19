#include "ti_msp_dl_config.h"
#include <ti/driverlib/m0p/dl_interrupt.h>

#define LED_ON()          DL_GPIO_setPins(GPIO_SWITCH_PORT,GPIO_SWITCH_PIN_LED_PIN)
#define LED_OFF()         DL_GPIO_clearPins(GPIO_SWITCH_PORT,GPIO_SWITCH_PIN_LED_PIN)
#define LED_Toggle()      DL_GPIO_togglePins(GPIO_SWITCH_PORT,GPIO_SWITCH_PIN_LED_PIN)

#define DELAY_MS             (32000)//内部时钟CLK为32M，1/1000即为1ms

void GROUP1_IRQHandler(void)
{
    uint32_t gpioB = DL_GPIO_getEnabledInterruptStatus(GPIOB,GPIO_SWITCH_PIN_KEY_PIN);
    
    if((gpioB & GPIO_SWITCH_PIN_KEY_PIN) == GPIO_SWITCH_PIN_KEY_PIN)
    {
        delay_cycles(DELAY_MS*5);
        LED_Toggle();
        DL_GPIO_clearInterruptStatus(GPIOB,GPIO_SWITCH_PIN_KEY_PIN);
    }
}

int main(void)
{
    SYSCFG_DL_init();

    NVIC_EnableIRQ(GPIO_SWITCH_INT_IRQN);
    
    while(1)
    {
        __WFI();
    }
}

