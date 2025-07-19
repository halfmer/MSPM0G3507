#include "LED.h"
#include "Key.h"
#include "ti_msp_dl_config.h"

static uint8_t gEchoData;

int main(void)
{
    

    SYSCFG_DL_init();
    LED_GPIO_init();
    KEY_GPIO_init();
    
    NVIC_ClearPendingIRQ(UART_0_INST_INT_IRQN);
    NVIC_EnableIRQ(UART_0_INST_INT_IRQN);
    
    while(1)
    {
    }
}
void UART_0_INST_IRQHandler(void)
{
    gEchoData = DL_UART_Main_receiveData(UART_0_INST);
    DL_UART_Main_transmitData(UART_0_INST,gEchoData);
    if(gEchoData == 0x31)//十六进制0x31即数字1
    {
        LED_Toggle();//接受串口数据翻转LED
    }
}
