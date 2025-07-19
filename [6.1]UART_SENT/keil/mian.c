#include "LED.h"
#include "Key.h"
#include "ti_msp_dl_config.h"



int main(void)
{
    
    char uart_buf[25]="Uart Test!\n";

    SYSCFG_DL_init();
    LED_GPIO_init();
    KEY_GPIO_init();
    
    while(1)
    {
        for(uint8_t i=0;i<11;i++)
        {
            DL_UART_transmitData(UART_0_INST,uart_buf[i]);
            while(DL_UART_isBusy(UART_0_INST));
            Delay_ms(250);
        }
            Delay_ms(1000);
    }
}
