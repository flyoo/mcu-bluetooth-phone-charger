#include <reg52.h>
#include <lcm_1602.h>
#include "dht11.h"
#include "ds18b20.h"

sbit BUZZER = P1^0;	//Ϊ0ʱ��

unsigned char ds18b20_temp = 0;

//�жϼ�����־
unsigned char flag=0;

extern int temp_value, humi_value;
static void InitTime(void);
void Delay_1ms(unsigned int ms);

void main()
{
	int i;
	unsigned char mark = 1;
	lcm_1602_init();

	//�ȵ��ϵ��ȶ�
	Delay_1ms(1000);
	//��Ϊ��һ������dht11 �Żᴥ��һ�βɼ�����.
	//������ʹ������ʱ�ɼ�һ������
	ReadTempAndHumi();
	//��Ϊ�����βɼ�������һ����ʱ����,���ﻹ�ɼ���
	Delay_1ms(3000);
	//�趨��ʱ��
	InitTime();

	lcm_1602_show_string(0, 0, "temp:00");
	lcm_1602_show_string(10, 0, "ds:");
	lcm_1602_show_string(0, 1, "hmdt:00");

	while(1)
	{
		//�����һ����ʪ��
		if(flag == 60)
		{
			flag = 0;
			mark++;
			ReadTempAndHumi();
		}
		if(mark % 2 == 0)
		{
			//��ʾ�¶�
			for(i=0;i<2;i++)
			{
				lcm_1602_show_word(5, 0, '0' + temp_value/10%10);
				lcm_1602_show_word(6, 0, '0' + temp_value%10);
			}		
		}
		else
		{
			for(i=0;i<2;i++)
			{
				//��ʾʪ��
				lcm_1602_show_word(5, 1, '0' + humi_value/10%10);
				lcm_1602_show_word(6, 1, '0' + humi_value%10);
			}
		}

		//for DSB18B20
		{
			tmpchange();
			ds18b20_temp = tmp()/16;
			if(ds18b20_temp<0){	//�¶�Ϊ��ֵ
				ds18b20_temp = 0-ds18b20_temp;
				lcm_1602_show_word(13, 0, '-');
				lcm_1602_show_word(14, 0, '0' + ds18b20_temp/10%10);
				lcm_1602_show_word(15, 0, '0' + ds18b20_temp%10);
			}else{
				lcm_1602_show_word(13, 0, '0' + ds18b20_temp/10%10);
				lcm_1602_show_word(14, 0, '0' + ds18b20_temp%10);
			}
		}

	}
	//return 0;	
}

/***********************************************************************************/

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