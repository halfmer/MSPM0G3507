/*

 */
//#include "dl_uart_main.h" 
#include <ti/driverlib/dl_uart_main.h>
#include "ti_msp_dl_config.h"
#include "tjc_usart_hmi.h" 
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stddef.h>

#define STR_LENGTH 100

typedef struct
{
	uint16_t Head;
	uint16_t Tail;
	uint16_t Lenght;
	uint8_t  Ring_data[RINGBUFF_LEN];
}RingBuff_t;

RingBuff_t ringBuff;	//����һ��ringBuff�Ļ�����



//����0���͵����ַ�
void uart0_send_char(char ch)
{
    //������0æ��ʱ��ȴ�����æ��ʱ���ٷ��ʹ��������ַ�
    while( DL_UART_isBusy(UART_0_INST) == true );
    //���͵����ַ�
    DL_UART_Main_transmitData(UART_0_INST, ch);
}
//����0�����ַ���
void uart0_send_string(char* str)
{
    //��ǰ�ַ�����ַ���ڽ�β ���� �ַ����׵�ַ��Ϊ��
    while(*str!=0&&str!=0)
    {
        //�����ַ����׵�ַ�е��ַ��������ڷ������֮���׵�ַ����
        uart0_send_char(*str++);
    }
}





//���ڷ����ַ����ͽ�����
void tjc_send_string(char* str)
{
    //��ǰ�ַ�����ַ���ڽ�β ���� �ַ����׵�ַ��Ϊ��
    while(*str!=0&&str!=0)
    {
        //�����ַ����׵�ַ�е��ַ��������ڷ������֮���׵�ַ����
        uart0_send_char(*str++);
    }
		uart0_send_char(0xff);
		uart0_send_char(0xff);
		uart0_send_char(0xff);
}



/********************************************************
��������  	TJC_SendData
���ߣ�    	
���ڣ�    	2024.08.18
���ܣ�    	�򴮿ڴ�ӡ����,��Ҫ�ƶ����͵��ַ�������
���������	
����ֵ�� 		
�޸ļ�¼��
**********************************************************/
void TJC_SendData(uint8_t *data, uint16_t len)
{
	while(len--)
	{
		//�ȴ��������ݼĴ���Ϊ��
		
		uart0_send_char(*data++);
		
	}

}




/********************************************************
��������  	TJCPrintf
���ߣ�    	wwd
���ڣ�    	2022.10.08
���ܣ�    	�򴮿ڴ�ӡ����,ʹ��ǰ�뽫USART_SCREEN_write���������Ϊ��ĵ�Ƭ���Ĵ��ڷ��͵��ֽں���
���������	0�ο�printf
����ֵ�� 		��ӡ�����ڵ�����
�޸ļ�¼��
**********************************************************/

void TJCPrintf(const char *str, ...)
{


	uint8_t end = 0xff;
	char buffer[STR_LENGTH+1];  // ���ݳ���
	va_list arg_ptr;
	va_start(arg_ptr, str);
	int len = vsnprintf(buffer, STR_LENGTH+1, str, arg_ptr);
	va_end(arg_ptr);
	for(int i = 0; i < len; i++)
	{
		uart0_send_char(buffer[i]);
		
	}

	uart0_send_char(end);			//���������Ϊ��ĵ�Ƭ���Ĵ��ڷ��͵��ֽں���
	uart0_send_char(end);			//���������Ϊ��ĵ�Ƭ���Ĵ��ڷ��͵��ֽں���
	uart0_send_char(end);			//���������Ϊ��ĵ�Ƭ���Ĵ��ڷ��͵��ֽں���
}


/********************************************************
��������  	initRingBuff
���ߣ�
���ڣ�    	2022.10.08
���ܣ�    	��ʼ�����λ�����
���������
����ֵ�� 		void
�޸ļ�¼��
**********************************************************/
void initRingBuff(void)
{
  //��ʼ�������Ϣ
  ringBuff.Head = 0;
  ringBuff.Tail = 0;
  ringBuff.Lenght = 0;
}




/********************************************************
��������  	writeRingBuff
���ߣ�
���ڣ�    	2022.10.08
���ܣ�    	�����λ�����д������
���������
����ֵ�� 		void
�޸ļ�¼��
**********************************************************/
void writeRingBuff(uint8_t data)
{
  if(ringBuff.Lenght >= RINGBUFF_LEN) //�жϻ������Ƿ�����
  {
    return ;
  }
  ringBuff.Ring_data[ringBuff.Tail]=data;
  ringBuff.Tail = (ringBuff.Tail+1)%RINGBUFF_LEN;//��ֹԽ��Ƿ�����
  ringBuff.Lenght++;

}



/********************************************************
��������  	deleteRingBuff
���ߣ�
���ڣ�    	2022.10.08
���ܣ�    	ɾ�����ڻ���������Ӧ���ȵ�����
���������		Ҫɾ���ĳ���
����ֵ�� 		void
�޸ļ�¼��
**********************************************************/
void deleteRingBuff(uint16_t size)
{
	if(size >= ringBuff.Lenght)
	{
	    initRingBuff();
	    return;
	}
	for(int i = 0; i < size; i++)
	{

		if(ringBuff.Lenght == 0)//�жϷǿ�
		{
		initRingBuff();
		return;
		}
		ringBuff.Head = (ringBuff.Head+1)%RINGBUFF_LEN;//��ֹԽ��Ƿ�����
		ringBuff.Lenght--;

	}

}



/********************************************************
��������  	read1BFromRingBuff
���ߣ�
���ڣ�    	2022.10.08
���ܣ�    	�Ӵ��ڻ�������ȡ1�ֽ�����
���������		position:��ȡ��λ��
����ֵ�� 		����λ�õ�����(1�ֽ�)
�޸ļ�¼��
**********************************************************/
uint8_t read1BFromRingBuff(uint16_t position)
{
	uint16_t realPosition = (ringBuff.Head + position) % RINGBUFF_LEN;

	return ringBuff.Ring_data[realPosition];
}




/********************************************************
��������  	getRingBuffLenght
���ߣ�
���ڣ�    	2022.10.08
���ܣ�    	��ȡ���ڻ���������������
���������
����ֵ�� 		���ڻ���������������
�޸ļ�¼��
**********************************************************/
uint16_t getRingBuffLenght()
{
	return ringBuff.Lenght;
}


/********************************************************
��������  	isRingBuffOverflow
���ߣ�
���ڣ�    	2022.10.08
���ܣ�    	�жϻ��λ������Ƿ�����
���������
����ֵ�� 		1:���λ��������� , 2:���λ�����δ��
�޸ļ�¼��
**********************************************************/
uint8_t isRingBuffOverflow()
{
	return ringBuff.Lenght == RINGBUFF_LEN;
}







