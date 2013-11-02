#include <reg52.h>
#include<intrins.h>
 
sbit DS18b20_DataPort=P1^1; //����ѡ��

void delay_us(unsigned int t)
{
	for(;t>0;t--);
}

bit DS18b20_reset(void)  //����0-���豸����1-���豸����
{
    unsigned char i;
    bit RstFlag;
     RstFlag=1;
     DS18b20_DataPort=0;
    delay_us(600);//������Ӧʱ������480us
     DS18b20_DataPort=1;
     delay_us(60); //15us-60us
     for (i=0;i<16;i++) //60us-240us
     {
          delay_us(15);
          if (DS18b20_DataPort==0)
            RstFlag=0;
    }
     delay_us(240);//  �͵�ƽ����ʱ������240us
     return RstFlag;
}



void DS1820_write(unsigned char command)
{
    unsigned char i;
     DS18b20_DataPort=1;
     delay_us(1);
     for (i=0;i<8;i++)
     {
          EA=0;
         DS18b20_DataPort=0;
          delay_us(15);
          DS18b20_DataPort=ch&0x1;
          EA=1;
          delay_us(50);
          DS18b20_DataPort=1;
          command=command>>1;
          delay_us(1);
     }
}


unsigned char DS1820_read(void)
{
    unsigned char i,date = 0;
     DS18b20_DataPort = 1;
     delay_us(1);
     for (i = 0;i < 8;i++)
     {
         EA = 0;
         DS18b20_DataPort = 0;
         delay_us(10);
          DS18b20_DataPort = 1;
          delay_us(2);
          date = date>>1;
          if (DS18b20_DataPort == 1)
          {
              date += 0x80;
          }
          EA = 1;
          delay_us(50);
 }
 return date;
}



void DS1820_skipROM(void)  //�����������������кŵĲ���ָ��
{
     DS18b20_reset();
     DS18b20_write(0xcc);
}
void DS1820_startADC(void)
{

     DS18b20_reset();  //��λ
     DS18b20_write(0xcc);  //�㲥
     DS18b20_write(0x44);  //����ADת�� 12bit700ms
}



unsigned int DS1820_getTempValue(void)
{
    unsigned char i,j;
    unsigned int T;
     DS18b20_skipROM();
     DS18b20_write(0xBE); //��ȡ�¶ȵ�����
     i=DS18b20_read();
     j=DS18b20_read();
     DS18b20_startADC();
     T = i+j*256;
     if (T==0xffff)
        return 0xffff;
     if (T>0x8000) //�¶�Ϊ����
     {
          T = -T;
          return (0x8000+T*5/80);   // �����¶ȵ�ֵ
     }
     else                //�¶�Ϊ��ֵ
          return (T*5/80);
}


//������
/*
void main(void)
{   
    int w;
    Init();//ϵͳ��ʼ��
      
       while(1)
       {
        w = DS1820_getTempValue();  //���¶�ֵ�ͳ�
        _nop_;
        _nop_;
        _nop_;
       }
    //while(1);
}
*/