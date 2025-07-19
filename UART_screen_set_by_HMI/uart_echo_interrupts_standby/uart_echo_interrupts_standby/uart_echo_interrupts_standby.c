/*
 * Copyright (c) 2021, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "ti_msp_dl_config.h"
#include "tjc_usart_hmi.h"
#include "stdio.h"

#define FRAME_LENGTH 7
volatile uint32_t delay_times = 0;
volatile uint8_t uart_data = 0;
volatile uint32_t now_time = 0;

// ����δ�ʱ��ʵ�ֵľ�ȷms��ʱ
void delay_ms(unsigned int ms)
{
	delay_times = ms;
	while (delay_times != 0)
		;
}

int main(void)
{
	SYSCFG_DL_init();
	// ��������жϱ�־
	NVIC_ClearPendingIRQ(UART_0_INST_INT_IRQN);
	// ʹ�ܴ����ж�
	NVIC_EnableIRQ(UART_0_INST_INT_IRQN);
	int a = 100;
	char str[100];
	uint32_t last_time = 0;
	while (1)
	{
		if (now_time - last_time >= 1000)
		{
			last_time = now_time;
			sprintf(str, "n0.val=%d", a);
			tjc_send_string(str);
			sprintf(str, "t0.txt=\"hahah%d\"\xff\xff\xff", a);
			tjc_send_string(str);
			sprintf(str, "click b0,1\xff\xff\xff");
			tjc_send_string(str);
			delay_ms(50);
			sprintf(str, "click b0,0\xff\xff\xff");
			tjc_send_string(str);

			a++;
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
							// DL_GPIO_clearPins(LED1_PORT,LED1_PIN_14_PIN);//����͵�ƽ
						}
						else
						{
							// DL_GPIO_setPins(LED1_PORT,LED1_PIN_14_PIN);  //����ߵ�ƽ
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
