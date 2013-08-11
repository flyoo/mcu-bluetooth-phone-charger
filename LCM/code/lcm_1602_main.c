/******************************************************************
** 1602AҺ������������
** �� �� Ƶ ��:11.0592M 
******************************************************************/

#include <reg52.h>
#include <lcm_1602.h>

sbit LCM_E=P3^4;//����ӿ�
sbit LCM_RW=P3^5;
sbit LCM_RS=P3^6;
sbit LCM_V0 = P3^7;
#define LCM_Data P2//���ݽӿ�

void LCM_WritEDAta(uchar WDLCM);
void LCM_WriteCommand(uchar WCLCM,BuysC);
uchar LCM_ReadData(void);
uchar LCM_ReadStatus(void);
//void LCM_Init(void);
//void DisplayOneChar(uchar X, uchar Y, uchar DData);
//void DisplayListChar(uchar X, uchar Y, uchar code *DData);
void Delay5Ms(void);
void Delay400Ms(void);

uchar code uctech[] = {"1602A"};
uchar code net[] = {"zhengzekai_test"};

/****************************1602Aд���ݺ���**************************/
/*����ԭ��:void LCM_WriteData(uchar WDLCM)
/*��������:1602Aд����
/*�������:Ҫд�������
/*�������:��
/**********************************************************************/
void LCM_WriteData(uchar WDLCM)
{
	LCM_ReadStatus();//���æ
	LCM_Data = WDLCM;
	LCM_RS = 1;
	LCM_RW = 0;
	LCM_E = 0;//�������ٶ�̫�߿���������С����ʱ
	LCM_E = 0;//��ʱ
	LCM_E = 1;
}

/****************************1602Aдָ���**************************/
/*����ԭ��:void LCM_WriteCommand(uchar WCLCM,BuysC)
/*��������:1602Aдָ��
/*�������:Ҫд���ָ��
/*�������:��
/**********************************************************************/
void LCM_WriteCommand(uchar WCLCM,BuysC)//BuysCΪ0ʱ����æ���
{
	if(BuysC) LCM_ReadStatus();//������Ҫ���æ
	LCM_Data = WCLCM;
	LCM_RS = 0;
	LCM_RW = 0;
	LCM_E = 0;
	LCM_E = 0;
	LCM_E = 1;
}

/****************************1602A�����ݺ���**************************/
/*����ԭ��:uchar LCM_ReadData(void)
/*��������:1602A������
/*�������:��
/*�������:1602A���ص�����
/**********************************************************************/
uchar LCM_ReadData(void)
{
	LCM_RS = 1;
	LCM_RW = 1;
	LCM_E = 0;
	LCM_E = 0;
	LCM_E = 1;
	return(LCM_Data);
}

/****************************1602A��״̬����**************************/
/*����ԭ��:uchar LCM_ReadData(void)
/*��������:1602A��״̬
/*�������:��
/*�������:1602A���ص�״̬
/**********************************************************************/
uchar LCM_ReadStatus(void)
{
	LCM_Data = 0xFF;
	LCM_RS = 0;
	LCM_RW = 1;
	LCM_E = 0;
	LCM_E = 0;
	LCM_E = 1;
	while (LCM_Data & 0x80);//���æ�ź�
	return(LCM_Data);
}

/****************************1602A��ʼ������**************************/
/*����ԭ��:void LCM_Init(void)
/*��������:1602A��ʼ��
/*�������:��
/*�������:��
/**********************************************************************/
void LCM_Init(void)
{
	LCM_V0 = 0;	//���ڶԱȶȣ���Ȼ����ʾ
	LCM_Data = 0;
	LCM_WriteCommand(0x38,0);//������ʾģʽ���ã������æ�ź�
	Delay5Ms();
	LCM_WriteCommand(0x38,0);
	Delay5Ms();
	LCM_WriteCommand(0x38,0);
	Delay5Ms();
	LCM_WriteCommand(0x38,1);//��ʾģʽ����,��ʼҪ��ÿ�μ��æ�ź�
	LCM_WriteCommand(0x08,1);//�ر���ʾ
	LCM_WriteCommand(0x01,1);//��ʾ����
	LCM_WriteCommand(0x06,1);// ��ʾ����ƶ�����
	LCM_WriteCommand(0x0C,1);// ��ʾ�����������
}

/******************1602A��ָ��λ����ʾһ���ַ�����**********************/
/*����ԭ��:void DisplayOneChar(uchar X, uchar Y, uchar DData)
/*��������:1602A��ָ��λ����ʾһ���ַ�
/*�������:X���� Y���� Ҫ��ʾ���ַ�
/*�������:��
/**********************************************************************/

void DisplayOneChar(uchar X, uchar Y, uchar DData)
{
	Y &= 0x1;
	X &= 0xF;//����X���ܴ���15��Y���ܴ���1
	if (Y) X |= 0x40;//��Ҫ��ʾ�ڶ���ʱ��ַ��+0x40;
	X |= 0x80;// ���ָ����
	LCM_WriteCommand(X, 0);//���ﲻ���æ�źţ����͵�ַ��
	LCM_WriteData(DData);
}

/*******************1602A��ָ��λ����ʾһ���ַ�����*********************/
/*����ԭ��:void DisplayListChar(uchar X, uchar Y, uchar code *DData)
/*��������:1602A��ָ��λ����ʾһ���ַ�
/*�������:X���� Y���� Ҫ��ʾ�ַ������׵�ַ
/*�������:��
/**********************************************************************/
void DisplayListChar(uchar X, uchar Y, uchar code *DData)
{
	uchar ListLength;
	ListLength = 0;
	Y &= 0x1;
	X &= 0xF;//����X���ܴ���15��Y���ܴ���1
	while (DData[ListLength]>0x20)//�������ִ�β���˳�
	{
		if (X <= 0xF)//X����ӦС��0xF
		{
			DisplayOneChar(X, Y, DData[ListLength]);//��ʾ�����ַ�
			ListLength++;
			X++;
		}
	}
}

/********************5ms��ʱ����************************/
/*����ԭ��:void Delay5Ms(void)
/*��������:5ms��ʱ
/*�������:��
/*�������:��
/**********************************************************************/
void Delay5Ms(void)
{
	unsigned int TempCyc = 5552;
	while(TempCyc--);
}

/********************400ms��ʱ��ʱ����************************/
/*����ԭ��:void Delay400Ms(void)
/*��������:400ms��ʱ��ʱ
/*�������:��
/*�������:��
/**********************************************************************/
void Delay400Ms(void)
{
	uchar TempCycA = 5;
	unsigned int TempCycB;
	while(TempCycA--)
	{
		TempCycB=7269;
		while(TempCycB--);
	};
}

/***************************main**********************************/
void main(void)
{
	Delay400Ms();//�����ȴ�����LCM���빤��״̬	
	LCM_Init();//LCM��ʼ��	
	Delay5Ms();//��ʱƬ��(�ɲ�Ҫ)	
	DisplayListChar(0, 0, net);	
	DisplayListChar(6, 1, uctech);
	DisplayOneChar(0,1,'9');
	LCM_ReadData();//�����þ�������	
	LCM_Data=255;	
	while(1);
}