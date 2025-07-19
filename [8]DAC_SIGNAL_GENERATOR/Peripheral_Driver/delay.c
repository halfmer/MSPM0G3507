#include "delay.h"

//延时函数
void Delay_ms(unsigned int ms)
{
    while(ms--)
    delay_cycles(CPUCLK_FREQ/1000);
}
void Delay_us(unsigned int us)
{
    while(us--)
    delay_cycles(CPUCLK_FREQ/1000000);
}
