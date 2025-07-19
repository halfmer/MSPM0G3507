/*__attribute__((unused))
 *这是 GCC 编译器特有的语法，其作用是告诉编译器，即便这个变量没有被使用，也不要发出警告。
 *一般在调试或者定义一些预留变量的时候会用到这种语法
 *
 *volatile
 *该关键字表明变量的值可能会在意料之外的情况下被改变，比如硬件自动修改
 *编译器不会对使用了volatile关键字的变量进行优化，每次读取该变量时都会直接从内存中读取
*/

#include "ti_msp_dl_config.h"
#include "LED.h"
#include "Key.h"


int main(void)
{

    
    SYSCFG_DL_init();
    LED_GPIO_init();
    KEY_GPIO_init();
    
    DL_TimerG_startCounter(CAPTURE_0_INST);
    
    while(1)
    {

    }
}

