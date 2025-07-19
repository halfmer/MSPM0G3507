#include "LED.h"
#include "Key.h"
#include "ti_msp_dl_config.h"


static uint8_t key;

int main(void)
{
    SYSCFG_DL_init();
    LED_GPIO_init();
    KEY_GPIO_init();
    
    while(1)
    {
        key=keyscan(0);
        if(key & KEY_Value)
        {
            LED_Toggle();
        }
    }
}
