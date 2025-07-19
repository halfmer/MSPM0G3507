#include "ti_msp_dl_config.h"
#include "tjc_usart_hmi.h"
#include "LED.h"
#include "delay.h"
#include "stdio.h"

#define FRAME_LENGTH 7
volatile uint32_t delay_times = 0;
volatile uint8_t uart_data = 0;
volatile uint32_t now_time = 0;


int main(void)
{
   SYSCFG_DL_init();
   LED_GPIO_init();
   // ��������жϱ�־
   NVIC_ClearPendingIRQ(UART_0_INST_INT_IRQN);
   // ʹ�ܴ����ж�
   NVIC_EnableIRQ(UART_0_INST_INT_IRQN);
   int a = 100;
   char str[100];
   uint32_t last_time = 0;
    LED_ON();
   while (1)
   {

      if (now_time - last_time >= 1000)
      {
         last_time = now_time;
         sprintf(str, "n0.val=%d", a);
         tjc_send_string(str);
         sprintf(str, "t0.txt=\"%d\"\xff\xff\xff", a);
         tjc_send_string(str);
         sprintf(str, "click b0,1\xff\xff\xff");
         tjc_send_string(str);
         Delay_ms(50);
         sprintf(str, "click b0,0\xff\xff\xff");
         tjc_send_string(str);

         a++;
            if(a == 105)
            {
                LED_OFF();
                sprintf(str, "\x55\x01\x01\x01\xff\xff\xff");
                tjc_send_string(str);

            }
      }
      // stm32f103��GND�Ӵ������򴮿ڹ��ߵ�GND,����
      // stm32f103��TX1(PA9)�Ӵ������򴮿ڹ��ߵ�RX
      // stm32f103��RX1(PA10)�Ӵ������򴮿ڹ��ߵ�TX
      // stm32f103��5V�Ӵ�������5V,����Ǵ��ڹ���,���ý�5VҲ����

      // �������ݸ�ʽ��
      // ��������֡���ȣ�7�ֽ�
      // ֡ͷ     ����1    ����2   ����3       ֡β
      // 0x55     1�ֽ�   1�ֽ�    1�ֽ�     0xffffff
      // ��������01ʱ
      // ֡ͷ     ����1    ����2   ����3       ֡β
      // 0x55     01     led���  led״̬    0xffffff
      // ����1����λ������  printh 55 01 01 00 ff ff ff  ���壺1��led�ر�
      // ����2����λ������  printh 55 01 04 01 ff ff ff  ���壺4��led��
      // ����3����λ������  printh 55 01 00 01 ff ff ff  ���壺0��led��
      // ����4����λ������  printh 55 01 04 00 ff ff ff  ���壺4��led�ر�

      // ��������02��03ʱ
      // ֡ͷ     ����1    ����2   ����3       ֡β
      // 0x55     02/03   ����ֵ    00    0xffffff
      // ����1����λ������  printh 55 02 64 00 ff ff ff  ���壺h0.val=100
      // ����2����λ������  printh 55 02 00 00 ff ff ff  ���壺h0.val=0
      // ����3����λ������  printh 55 03 64 00 ff ff ff  ���壺h1.val=100
      // ����4����λ������  printh 55 03 00 00 ff ff ff  ���壺h1.val=0

      // �����ڻ��������ڵ���һ֡�ĳ���ʱ
      while (usize >= FRAME_LENGTH)
      {
         // У��֡ͷ֡β�Ƿ�ƥ��
         if (usize >= FRAME_LENGTH && u(0) == 0x55 && u(4) == 0xff && u(5) == 0xff && u(6) == 0xff)
         {
            // ƥ�䣬���н���
            if (u(1) == 0x01)
            {
               sprintf(str, "msg.txt=\"led %d is %s\"", u(2), u(3) ? "on" : "off");
               tjc_send_string(str);
               if (u(2) == 0x00)
               {
                  if (u(3) == 0x00)
                  {
                     LED_ON();
                  }
                  else
                  {
                     LED_OFF();
                  }
               }
            }
            else if (u(1) == 0x02)
            {
               // �·�����h0����������Ϣ

               sprintf(str, "msg.txt=\"h0.val is %d\"", u(2));
               tjc_send_string(str);
            }
            else if (u(1) == 0x03)
            {
               // �·�����h1����������Ϣ
               sprintf(str, "msg.txt=\"h1.val is %d\"", u(2));
               tjc_send_string(str);
            }

            udelete(7); // ɾ��������������
         }
         else
         {

            // ��ƥ��ɾ��1�ֽ�
            udelete(1);
            break;
         }
      }
   }
}

void SysTick_Handler()
{
   if (delay_times != 0)
   {
      delay_times--;
   }
   now_time++;
}

// ���ڵ��жϷ�����
void UART_0_INST_IRQHandler(void)
{
   // ��������˴����ж�
   switch (DL_UART_Main_getPendingInterrupt(UART_0_INST))
   {
   case DL_UART_MAIN_IIDX_RX: // ����ǽ����ж�
      // �ӷ��͹��������ݱ����ڱ�����
      uart_data = DL_UART_Main_receiveData(UART_0_INST);
      // ������������ٷ��ͳ�ȥ
      // uart0_send_char(uart_data);
      writeRingBuff(uart_data);
      break;

   default: // �����Ĵ����ж�
      break;
   }
}