#include <reg52.h>				       //����ͷ�ļ�
#include <lcd.h>
#include <intrins.h>


#define uint unsigned int		               //�곣������
#define uchar unsigned char		             //�곣������



   




float  SpeedCount=0;			      				//���ټ�������
float  Speed = 0.0;				      				//˲ʱ�ٶ�						
float  flow = 0.0;			      			//����                 			
uint time_count;


void interrupt_init();			       //�����жϳ�ʼ������
void ComputeSpeedANDDistance();    //�����ٶȾ�����㺯��

void DelayUs(unsigned int time);   //��ʱ�Ӻ���
void Delay(unsigned int time);		 //��ʱ������
void DisplayFlow(void);
/*����������������������������������������������������������������������������*/
void main()    
{	

  interrupt_init();                       //�жϳ�ʼ��
	LCDInit();                              //��ʾ����ʼ��
	DisplayString(0x0,0,"1sensor.taobao");
	DisplayString(0x0,1,"          .com");
	Delay(300);
	WriteCmd(LCD_CLS);                      //���� 


	WriteCmd(LCD_CLS);
	DisplayString(0x0,0,"Flow:");  

	DisplayString(0x0C,0,"L");  	
	
  while(1)
  {
		if(time_count == 0)
		{
			ComputeSpeedANDDistance();
			DisplayFlow();			
		}

	}
}





/*���������������������������������ٶȡ��������㺯����������������������������������������*/
void ComputeSpeedANDDistance(void)
{
	Speed=SpeedCount/5.5/60;					//����1������
	flow+=Speed;								          //�������						
	SpeedCount=0;
}
//void ComputeSpeedANDDistance(void)
//{
//	flow+=SpeedCount;								          //�������						
//	SpeedCount=0;
//}
/*-------------------------------------  ��ʾ���� ----------------------------------------*/
//�������ܣ�    ��ʾ·��
//������ڲ�������
//��������ֵ��  ��
void DisplayFlow(void)
{
	uchar a1,a2,a3,a4,a5,a6;
	a1 = (int)(flow*100)/100000+'0';
	a2 = (int)(flow*100)%100000/10000+'0';	
	a3 = (int)(flow*100)%10000/1000+'0';
	a4 = (int)(flow*100)%1000/100+'0';
	a5 = (int)(flow*100)%100/10+'0';
	a6 = (int)(flow*100)%10+'0';	

//	a1 = (int)(flow)/100000+'0';
//	a2 = (int)(flow)%100000/10000+'0';	
//	a3 = (int)(flow)%10000/1000+'0';
//	a4 = (int)(flow)%1000/100+'0';
//	a5 = (int)(flow)%100/10+'0';
//	a6 = (int)(flow)%10+'0';	
	
	DisplaySingleChar(0x05,0,a1)	;
	DisplaySingleChar(0x06,0,a2)	;	
	DisplaySingleChar(0x07,0,a3)	;	
	DisplaySingleChar(0x08,0,a4)	;	
	DisplaySingleChar(0x09,0,'.')	;		
	DisplaySingleChar(0x0a,0,a5)	;	
	DisplaySingleChar(0x0b,0,a6)	;
	
}

/*����������������������������������ʱ������������������������������������������*/
//�������ܣ�    ��ʱ
//������ڲ������ַ���x��������ʱʱ��ĳ���
//��������ֵ��  ��
void Delay(unsigned int time)
{
    unsigned int timeCounter = 0;
	for (timeCounter = time;timeCounter > 0 ;timeCounter --)
		DelayUs(255);
}

void DelayUs(unsigned int time)
{
	unsigned int timeCounter = 0;
	for (timeCounter = 0;timeCounter < time;timeCounter ++)
		_nop_();
}

/*���������������������������������жϳ�ʼ����������������������������������������*/
 void interrupt_init()
{
	EA=1;									  //�����ж�
	EX0=1;								  //���ⲿ�ж�0
	IT0=1;									//INTO���ش���
	PX0=1;          				//INTO���ȼ�Ϊ�߼�
	EX1=1;									//��INT1�ж�
	IT1=1;									//INT1���ش���
	PX1=1;          				//INT1���ȼ�Ϊ�߼�
	
	TMOD=0x11;							//T0/T1��ʱ��ʽ1
	ET0=0x00;      					//��T0�ж�
	ET1=0x01;     					//��T1�ж�

  TR0=0x00;
	TR1=0x01;      							            //����T1
	TH1=0x4C;									              //��ʱ��ֵ-50ms�ж�һ��
	TL1=0x00;	
}


/*���������������������������������ⲿ�ж�1��������������������������������������*/
//�������ܣ�    �ⲿ�ж�1����¼����ת����Ȧ��
//������ڲ�������
//��������ֵ��  ��
void ext1() interrupt 2			
{
	 SpeedCount++;
}

/*����������������������������������ʱ���ж�1��������������������������������������*/
//�������ܣ�    ��ʱ���ж�1��������¼ʱ��
//������ڲ�������
//��������ֵ��  ��
void Time1() interrupt 3      
{
	TR1=1;
  TH1 = 0x04C;
  TL1 = 0x00;
	time_count++;
	if (time_count == 20)
	{
	 time_count = 0;
	} 
}


