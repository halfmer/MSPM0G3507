#include "LED.h"
#include "Key.h"
#include "OLED.h"
#include "delay.h"
#include "ti_msp_dl_config.h"

//用的是6针OLED，分别为VCC,GND,SCL,SDA,RST,D/C
//CS默认接地,用SPI通讯
/*引脚连接情况:
 *SCL:PA12
 *SDA:PA14
 *RST:PA15
 *D/C:PA13
 */


static uint8_t key;
static uint16_t gCheckADC,gAdcResult0;
const char uart_buf[10]="voltage:";


int main(void)
{
    float l;
    SYSCFG_DL_init();
    LED_GPIO_init();
    KEY_GPIO_init();
    
    //OLED初始化
    OLED_Init();
    OLED_Clear();
    OLED_Display_On();
    
    //使能ADC
    NVIC_EnableIRQ(ADC12_0_INST_INT_IRQN);
    
                           
    while(1)
    {
       
        Delay_ms(10);
        OLED_Refresh_Gram();

        //开始接收
        DL_ADC12_startConversion(ADC12_0_INST);
        //等待所有数据通道接收完成
        while(gCheckADC == false)
        {
            __WFE();
        }
        
        //光敏,MSP-LITO-G3507没有这个元件，所以要另装
        //可添加多通道
        gAdcResult0 = DL_ADC12_getMemResult(ADC12_0_INST,DL_ADC12_MEM_IDX_0);

        //电压转换
        l = (float)gAdcResult0/4096.0f*3.3f;
        
        /*==运用OLED显示采集的数据==*/
        OLED_ShowString(30,17,uart_buf);
        OLED_ShowFloat(l,2,45,30);
        /*======================*/
        
        DL_ADC12_enableConversions(ADC12_0_INST);//使能ADC转换
        
        //按键实现翻转LED
        key=keyscan(0);
        if(key & KEY_Value)
        {
            LED_Toggle();
        }
    }
}

//ADC转换完成后进入中断
void ADC12_0_INST_IRQHandler(void)
{
    switch(DL_ADC12_getPendingInterrupt(ADC12_0_INST))
    {
        case DL_ADC12_IIDX_MEM0_RESULT_LOADED:
            gCheckADC = true;
            break;
        default:
            break;
    }
}
