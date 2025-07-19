#include "LED.h"
#include "Key.h"


static uint8_t key;

int main(void)
{
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
