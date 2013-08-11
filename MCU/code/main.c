#include <reg52.h> 
#include <stdio.h>
#include <lcm_1602.h>

uchar code asdf[] = {"zhengzekai"};	//���ӡ�code����������ʾ����������֡���

unsigned char flag, a, temp;

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
	lcm_1602_init();

	P0 = 0x01;
	temp = 0;

	lcm_1602_show_string(0, 0, "helloworld=");
	while(1){
		lcm_1602_show_word(11, 0, temp%100/10+'0');	//ʮλ
		lcm_1602_show_word(12, 0, temp%10+'0');		//��λ

		send_str("zhengzk\t");

		temp++;
		if(temp == 100) temp = 0;
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


