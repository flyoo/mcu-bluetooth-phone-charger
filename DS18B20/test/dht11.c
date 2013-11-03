//��Ƭ��ʱ��Ϊ11.0592M
#include "dht11.h"
#include <reg52.h>
//������Լ���dht11 �ӵ�IO �����Ķ�λ����
sbit dht11 = P1^3;
//��ֹ����Ӳ��ͨ��ʱ������ѭ���ļ�����Χ
#define NUMBER 20
#define SIZE 5
static unsigned char status;
//������ֽ����ݵ�����
static unsigned char value_array[SIZE];
/*�����������ļ�������ʪ��ֵ,ʵ�����¶ȵ�������10 ��
��dht11 ���ص��¶���26,��temp_value = 260, ʪ��ͬ��*/
int temp_value, humi_value;
static unsigned char ReadValue(void);

extern void Delay_1ms(unsigned int ms)
{
	unsigned int x, y;
	for(x = ms; x > 0; x--)
	{
		for(y = 124; y > 0; y--);
	}
}

static void Delay_10us(void)
{
	unsigned char i;
	i--;
	i--;
	i--;
	i--;
	i--;
	i--;
}

/*��һ���ֽڵ�����*/
static unsigned char ReadValue(void)
{
	unsigned char count, value = 0, i;
	status = OK; //�趨��־Ϊ����״̬
	for(i = 8; i > 0; i--)
	{
		//��λ����
		value <<= 1;
		count = 0;
		//ÿһλ����ǰ����һ��50us �ĵ͵�ƽʱ��.�ȴ�50us �͵�ƽ����
		while(dht11 == 0 && count++ < NUMBER);
		if(count >= NUMBER)
		{
			status = ERROR; //�趨�����־
			return 0; //����ִ�й��̷���������˳�����
		}
		//26-28us �ĸߵ�ƽ��ʾ��λ��0,Ϊ70us �ߵ�ƽ���λ1
		Delay_10us();
		Delay_10us();
		Delay_10us();
		//��ʱ30us �����������Ƿ��Ǹߵ�ƽ
		if(dht11 != 0)
		{
			//���������ʾ��λ��1
			value++;
			//�ȴ�ʣ��(Լ40us)�ĸߵ�ƽ����
			while(dht11 != 0 && count++ < NUMBER)
			{
				dht11 = 1;
			}
			if(count >= NUMBER)
			{
				status = ERROR; //�趨�����־
				return 0;
			}
		}
	}
	return (value);
}

//��һ�ε�����,�����ֽ�
extern unsigned char ReadTempAndHumi(void)
{
	unsigned char i = 0, check_value = 0,count = 0;
	EA = 0;
	dht11 = 0; //���������ߴ���18ms ���Ϳ�ʼ�ź�
	Delay_1ms(20); //�����18 ����
	dht11 = 1; //�ͷ�������,���ڼ��͵�ƽ��Ӧ���ź�
	//��ʱ20-40us,�ȴ�һ��ʱ�����Ӧ���ź�,Ӧ���ź��Ǵӻ�����������80us
	Delay_10us();
	Delay_10us();
	Delay_10us();
	Delay_10us();
	if(dht11 != 0) //���Ӧ���ź�,Ӧ���ź��ǵ͵�ƽ
	{
		//ûӦ���ź�
		EA = 1;
		return ERROR;
	}
	else
	{
		//��Ӧ���ź�
		while(dht11 == 0 && count++ < NUMBER); //�ȴ�Ӧ���źŽ���
		if(count >= NUMBER) //���������Ƿ񳬹����趨�ķ�Χ
		{
			dht11 = 1;
			EA = 1;
			return ERROR; //�����ݳ���,�˳�����
		}
		count = 0;
		dht11 = 1;//�ͷ�������
		//Ӧ���źź����һ��80us �ĸߵ�ƽ���ȴ��ߵ�ƽ����
		while(dht11 != 0 && count++ < NUMBER);
		if(count >= NUMBER)
		{
			dht11 = 1;
			EA = 1;
			return ERROR; //�˳�����
		}
		//����ʪ.�¶�ֵ
		for(i = 0; i < SIZE; i++)
		{
			value_array[i] = ReadValue();
			if(status == ERROR)//����ReadValue()�����ݳ�����趨status ΪERROR
			{
				dht11 = 1;
				EA = 1;
				return ERROR;
			}
			//���������һ��ֵ��У��ֵ�������ȥ
			if(i != SIZE - 1)
			{
				//���������ֽ������е�ǰ���ֽ����ݺ͵��ڵ����ֽ����ݱ�ʾ�ɹ�
				check_value += value_array[i];
			}
		}	//end for
		//��û�÷�����������ʧ��ʱ����У��
		if(check_value == value_array[SIZE - 1])
		{
			humi_value = value_array[0]?value_array[0]:value_array[1];
			temp_value = value_array[2]?value_array[2]:value_array[3];
			dht11 = 1;
			EA = 1;
			return OK; //��ȷ�Ķ���dht11 ���������
		}
		else
		{
			//У�����ݳ���
			EA = 1;
			return ERROR;
		}
	}
}
