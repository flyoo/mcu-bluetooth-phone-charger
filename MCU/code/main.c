#include<reg52.h> 
#include <stdio.h>

const unsigned char num[10] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0xff, 0x6f};

unsigned char flag, a, temp;

void num_show(unsigned char para)
{
	if(para < 10)
		P0 = num[para];
}

void uart0_init()
{		
	//9600�����ʶ�Ӧ FFD9H
	RCAP2L = 0xD9;
	RCAP2H = 0xFF;
	//RCLK��TCLK��1
	//TR2��1
	T2CON = 0x34;
	//���ڹ���ģʽ1������ʹ��
	SCON = 0x50;
	
	REN=1;    //�����������
	EA=1;     //CPU�ж�����
	
	//�������ж�
	ES = 1;
}

void delay_ms(int ms)
{
	int j = 5000;
	for(;ms>0;ms--)
		for(;j>0;j--);
}

void send_byte(unsigned char c)
{
	SBUF = c;
	while(!TI);	
	TI = 0;
}

void send_str(unsigned char *s)
{
	while(*s != '\0')
	{
		send_byte(*s);
		s++;
	}
}

void main()
{
	uart0_init();

	P0 = 0x01;
	temp = 0;

	while(1){
		send_str("zhengzk\t");
		num_show(temp++);
		if(temp == 10) temp = 0;
		delay_ms(500);delay_ms(500);
	}
}

/*
void ser() interrupt 4
{
	RI=0;
	a=SBUF;
	//send_str("�ж�");
	//send_byte(a);//ע�⣬�Ѵ�log���������ԭ���ǲ����Եġ����ǵ�Ƭ�������ٶ��������������������ö��С�
	SBUF = 0xaa;
	while(!TI);	
	TI = 0;
	flag=1;
}
*/


