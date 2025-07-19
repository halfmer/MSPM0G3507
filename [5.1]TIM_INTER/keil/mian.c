#include "ti_msp_dl_config.h"
#include "LED.h"
#include "Key.h"

uint8_t key;

int main(void)
{
    SYSCFG_DL_init();
    LED_GPIO_init();
    KEY_GPIO_init();
    
    NVIC_EnableIRQ(TIMER_0_INST_INT_IRQN);
    DL_TimerA_startCounter(TIMER_0_INST);
    
    while(1)
    {
        __WFI();
    }
}

void TIMER_0_INST_IRQHandler(void)
{
    static uint16_t t=0;
    switch(DL_TimerA_getPendingInterrupt(TIMER_0_INST)){
        case DL_TIMERA_IIDX_LOAD:
                t++;
                if(t == 1000)
                {
                    t=0;
                    LED_Toggle();
                }
            break;
        default:
            break;
    }
}

