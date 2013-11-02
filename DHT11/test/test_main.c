#include <reg52.h>
#include <lcm_1602.h>
#include "dht11.h"

sbit BUZZER = P1^0;

//�жϼ�����־
unsigned char flag=0;

extern int temp_value, humi_value;
static void InitTime(void);
void Delay_1ms(unsigned int ms);

void main()
{
/*
	uchar temperature = 0;
	//BUZZER = 0;
	lcm_1602_init();
	lcm_1602_show_string(0, 0, "zhengzekai");

	while(1)
	{
		lcm_1602_show_word(0, 1, '0' + temperature/10);	 
		lcm_1602_show_word(1, 1, '0' + temperature/10%10);
	}	
	//lcm_1602_show_string(6, 1, uctech);
*/

	int i;
	unsigned char mark = 1;
	//�ȵ��ϵ��ȶ�
	Delay_1ms(1000);
	//��Ϊ��һ������dht11 �Żᴥ��һ�βɼ�����.
	//������ʹ������ʱ�ɼ�һ������
	ReadTempAndHumi();
	//��Ϊ�����βɼ�������һ����ʱ����,���ﻹ�ɼ���
	Delay_1ms(3000);
	//�趨��ʱ��
	InitTime();

	lcm_1602_init();

	lcm_1602_show_string(0, 0, "temp:00");
	lcm_1602_show_string(0, 1, "hmdt:00");

	while(1)
	{
		//�����һ����ʪ��
		if(flag == 60)
		{
			flag = 0;
			mark++;
			/*
			//����ʪ��,�ɼ�⺯�������Ƿ�ʧ��,
			//��������OK(1)��ʾ�ɹ�,����ERROR(0)��ʾʧ��
			//OK��ERROR����DHT11.H�ж���ĺ�
			*/
			ReadTempAndHumi();
		}
		if(mark % 2 == 0)
		{
			//��ʾ�¶�
			//LED1=0;
			//LED2=1;
			for(i=0;i<2;i++)
			{
				//display(temp_value); 
				lcm_1602_show_word(5, 0, '0' + temp_value/10%10);
				lcm_1602_show_word(6, 0, '0' + temp_value%10);
			}		
		}
		else
		{
			//LED1=1;
			//LED2=0;
		//	mark++;
			for(i=0;i<2;i++)
			{
				//��ʾʪ��
				//display(humi_value);
				lcm_1602_show_word(5, 1, '0' + humi_value/10%10);
				lcm_1602_show_word(6, 1, '0' + humi_value%10);
			}
		}
	}
	//return 0;	
}

//�趨��ʱ��
static void InitTime(void)
{
	TH0 = (65536-50000);    		//(unsigned char)((65535U - 50000) >> 8);
	TL0 = (65536-50000);    		//(unsigned char)(65535U - 50000) ;
	TMOD = 0X01;
	TR0 = 1;
	ET0 = 1;
	EA = 1;
}

void timer(void) interrupt 1
{
	TH0 =(65535 - 50000);	// (unsigned char)((65535 - 50000) >> 8);
	TL0 =(65535 - 50000);	// (unsigned char)(65535 - 50000);
	flag++;
}