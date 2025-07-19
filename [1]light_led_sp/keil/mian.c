#include "LED.h"

int main(void)
{
    LED_GPIO_init();
    
    while(1)
    {
        LED_ON();
    }
}
