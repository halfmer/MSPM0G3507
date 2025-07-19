#include "LED.h"
#include "Key.h"
#include "ti_msp_dl_config.h"

const uint8_t gTxPacket[]="Hello World";
static uint8_t rxData;
volatile bool gCheckUART,gDMADone;

//效果为在串口发送一个字符串
int main(void)
{

    SYSCFG_DL_init();
    LED_GPIO_init();
    KEY_GPIO_init();
    
    //设置DMA通道的源地址
    DL_DMA_setSrcAddr(DMA,DMA_CH0_CHAN_ID,(uint32_t)&gTxPacket[0]);
    //设置DMA通道的目的地址
    DL_DMA_setDestAddr(DMA,DMA_CH0_CHAN_ID,(uint32_t)(&UART_0_INST->TXDATA));
    //为DMA传输设置快的大小
    DL_DMA_setTransferSize(DMA,DMA_CH0_CHAN_ID,sizeof(gTxPacket)/sizeof(gTxPacket[0]));
    
    DL_SYSCTL_disableSleepOnExit();
    
    NVIC_ClearPendingIRQ(UART_0_INST_INT_IRQN);
    NVIC_EnableIRQ(UART_0_INST_INT_IRQN);
    
    gCheckUART = false;
    gDMADone = false;
    
    //为传输启用DMA通道
    DL_DMA_enableChannel(DMA,DMA_CH0_CHAN_ID);
    
    while(false == gDMADone)
    {
        __WFE();
    }
    
    while(false == gCheckUART)
    {
        __WFE();
    }
    
    DL_SYSCTL_disableSleepOnExit();
    
    while(1)
    {
        __WFI();
    }
}

void UART_0_INST_IRQHandler(void)//串口0接收中断函数
{
    switch(DL_UART_Main_getPendingInterrupt(UART_0_INST))
    {
        case DL_UART_MAIN_IIDX_RX:
            rxData = DL_UART_Main_receiveDataBlocking(UART_0_INST);
            break;
        case DL_UART_MAIN_IIDX_EOT_DONE:
            gCheckUART = true;
            break;
        case DL_UART_MAIN_IIDX_DMA_DONE_TX:
            gDMADone = true;
            break;
        default:
            break;
    }
}
