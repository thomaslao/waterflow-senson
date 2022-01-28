#include <reg52.h>				       //����ͷ�ļ�
#include <lcd.h>
#include <intrins.h>


#define uint unsigned int		               //�곣������
#define uchar unsigned char		               //�곣������
#define CircleLength		0.204				//С��תһ�ֵĳ���Ϊ.204m


sbit left_motor1 = P1^3;
sbit left_motor2 = P1^4;
sbit right_motor1 = P1^5;
sbit right_motor2 = P1^6;





sbit blackline_detection1 =P3^6;					       //P3_2����Ϊ�ڴ����λ
uint blackn=0;	
uint waitn=0;	
uint blackline=0;			       //���弸�����α���
uint nts=0;	   
//uint aaa=1;	
uint bbb=1;	
uint ccc=1;	//�����ߴ���
uint ddd=1;
uint time_count=0;
char   ReadyToGo1=4;						//����ʱ
char   ReadyToGo=10;
bit    start_flag=1;
bit    Running;
char   p=0;
float  SpeedCount=0;			      				//���ټ�������
float  Speed = 0.0;							
float  Distance = 0.0;                 			
float  PassTime=0.00;                	//С�����ߵ�ʱ��
char   Time0INTCount=0;								//T1�ж�ʱ���ʱ
uchar forward=1;/***�ߵ͵�ƽ��־***/
uchar PWM_flag=1;
uchar startline_flag=1;
uchar flag;/***�ߵ͵�ƽ��־***/
bit direction;/***�����־***/
static uchar j=0;
static uchar constant=5;//���Ըı�ռ�ձ�
void motor_control();				       //����һ��������ƺ���
void PWM_control();
void Delay1();			       //����һ����ʱ����
void Delay2();
void interrupt_init();
void ComputeSpeedANDDistance();
void LCD_Display();
void DelayUs(unsigned int time);
void Clock0_Init(void);
void Clock1_Init(void);
void reverse(void);
void Delay(unsigned int time);						//��ʱ������
void pwm_control();
void heixianjiance();
/*����������������������������������������������������������������������������*/
void main()    
{	
  interrupt_init();
	LCDInit();
		DisplayString(0x0,0,"  Welcome...  ");
	DisplayString(0x0,1,"Autorunning Car");
	Delay(300);
	WriteCmd(LCD_CLS);
	nts=0;
	TR1=0x01;      							  //����T1
	TH1=0x4C;									  //��ʱ��ֵ-50ms�ж�һ��
	TL1=0x00;
	WriteCmd(LCD_CLS);
			DisplayString(0,0,"Time:");
	DisplayString(0x0E,0,"s");	
				DisplayString(0,1,"Distance:");

			DisplayString(0x0E,1,"m");
while(1)
{if(time_count == 0){ComputeSpeedANDDistance();}
LCD_Display();
//heixianjiance();
//if((bbb-ccc) == 1)
//{nts++;
//ccc = bbb;}
//else
//{ccc = bbb;}
DisplaySingleChar(0x0,1,nts+0x30);

//if(nts == 2 && ddd == 1)
//{left_motor2=0;left_motor1=0;right_motor2=0;right_motor1=0;Delay(1500);ddd = 0;}
motor_control();

}


}

//void heixianjiance()
//{
//	if(blackline_detection1==0 && blackline_detection2==0)
//	{aaa = 0;
//	bbb = aaa;}
//	else
//	{aaa = 1;
//	bbb = aaa;}

//}
void heixianjiance()
{
	if(blackline_detection1==0 )
	{
	bbb = 0;}
	else
	{
	bbb = 1;}

}

	/*
	LCDInit();										//��ʼ��LCD
	DisplayString(0x0,0,"  Welcome...  ");
	DisplayString(0x0,1,"Autorunning Car");
	Delay(300);
	WriteCmd(LCD_CLS);

	while(1)						   //������ѭ��
	{				
	 if(start_flag==1)
    	{
		  
	      DisplayString(0x0,0,"Left Times To Go");
	        while (ReadyToGo1--)
          {		
		    DisplaySingleChar(0x7,1,ReadyToGo1+0x30);
		    DisplaySingleChar(0x09,1,'s');	
			Delay(150);	
			Delay(150);	
	      }
    	WriteCmd(LCD_CLS);
    	DisplayString(0x05,0,"Go!!!");  
    	Delay(100);
    	start_flag=0;
	
	*/		   
		 
		// pwm_control();						   //���������ƺ���
		// motor_control();
		// pwm_control();
		// if(p==1)
		 //  {LCD_Display();}
//	}

/*��������������������������������������ơ�������������������������������������*/
//�������ܣ�    ���Ƶ����ת
//������ڲ�������
//��������ֵ��  �� 
//void motor_control()        
//{if(nts==0 || nts ==1)
//{if(flag == 1)
//{left_motor2=0;left_motor1=1;right_motor2=0;right_motor1=1;}
//if(flag == 2)
//{left_motor2=0;left_motor1=0;right_motor2=0;right_motor1=0;}}
//else if(nts == 2 || nts ==3)
//	{if(flag == 1)
//{left_motor2=1;left_motor1=0;right_motor2=1;right_motor1=0;}
//if(flag == 2)
//{left_motor2=0;left_motor1=0;right_motor2=0;right_motor1=0;}
//	}
//	else
//	{left_motor2=0;left_motor1=0;right_motor2=0;right_motor1=0;}

//}
//void motor_control()        

//{if(flag == 1)
//{if(nts==0 || nts ==1){
//left_motor2=0;left_motor1=1;right_motor2=0;right_motor1=1;
//}
//else if(nts == 2 || nts ==3){left_motor2=1;left_motor1=0;right_motor2=1;right_motor1=0;}
//else
//	{left_motor2=0;left_motor1=0;right_motor2=0;right_motor1=0;}}
//if(flag == 2)
//{left_motor2=0;left_motor1=0;right_motor2=0;right_motor1=0;}

//}
void motor_control()        

{
	if(nts==0 || nts ==1){
left_motor2=0;left_motor1=1;right_motor2=0;right_motor1=1;
}
else if(nts == 2 ){
	if (blackn == 0)
	{blackn = 1;
	}
	if (waitn<200)
	{left_motor2=0;left_motor1=0;right_motor2=0;right_motor1=0;}
	else {left_motor2=1;left_motor1=0;right_motor2=1;right_motor1=0;}

    }

else if(nts == 3 ){left_motor2=1;left_motor1=0;right_motor2=1;right_motor1=0;}
else
	{left_motor2=0;left_motor1=0;right_motor2=0;right_motor1=0;TR1=0x00;}

}
/*
void pwm_control()
{
 	switch(blackline)
		    { 
			   case 0: constant=0;break;
			   case 1: constant=0;break;
			   case 2: constant=0;break;
			   case 3: constant=5; break;//���Ըı�ռ�ձ�
			   case 4: constant=0;break;	 
			   case 5: constant=0;break;
			   case 6:{	  
			            forward=0;//ת���־λ
						startline_flag=1;
						left_motor1=0;left_motor2=0;right_motor1=0;right_motor2=0;			      
					   	DisplayString(0x0,0,"Left Times To Go");
						while (ReadyToGo--)
   						{
							DisplaySingleChar(0x7,1,ReadyToGo+0x30);
							DisplaySingleChar(0x09,1,'s');
							Delay(150);
							Delay(150);	
						}blackline++;}break;
			 
			   case 7:	constant=0;break;
			   case 8:	constant=0;break;
			   case 9:  constant=5; break;//���Ըı�ռ�ձ�
			   case 10:	constant=0; break;//���Ըı�ռ�ձ�
			   case 11: constant=0;break;
			   case 12: {
			              p=1;
			              left_motor1=0;left_motor2=0;right_motor1=0;right_motor2=0;
						}break;
			  // case 13:	 {p=1;}break;
			   default :{p=1;}	break;
		    }
         if(p==1)
            {
            	ComputeSpeedANDDistance();
	            left_motor1=0;left_motor2=0;right_motor1=0;	right_motor2=0;	//�г̽�����С��ֹͣ																		  	
           	}	
}  
*/

void LCD_Display()
{
 	
   // ChoosingDisplay=True;								  
//	WriteCmd(LCD_CLS);

	/*������ʾ���˵���Ȼ����ʾ��һ��ѡ��*/
//	DisplayString(0x0,0,"Now Choose what ");
//	DisplayString(0x0,1,"you want to see ");
//	Delay(100);
//	WriteCmd(LCD_CLS);

 // WriteCmd(LCD_CLS);

			DisplayTime();
			
            //WriteCmd(LCD_CLS);

			DisplayDistance();

		
}
/*�����������������������������������㺯����������������������������������������*/
void ComputeSpeedANDDistance(void)
{
	Speed=SpeedCount*CircleLength/20;					//����˲ʱ�ٶ�
	Distance+=Speed;								//�������						
	SpeedCount=0;
}

/*-------------------------------------  ��ʾʱ�� ----------------------------------------*/
void DisplayTime(void)
{
	char PassTime1=0x30;
	char PassTime2=0x30;
	char PassTime3=0x30;
	char PassTime4=0x30;
	
	if ((int)PassTime*100<100)                  		//ʱ��δ��1s
	{
		PassTime1+=0;      
		PassTime2+=(int)(PassTime*100)/10;
		PassTime3+=(int)(PassTime*100)%10;
	}
	else if ((int)(PassTime*100) > 100  && (int)(PassTime*100) < 1000) //��1s��δ��10s
	{
		PassTime1+=(int)(PassTime*100)/100;
		PassTime2+=(int)(PassTime*100)/10%10;
		PassTime3+=(int)(PassTime*100)%10;
	}
	else
	{
		PassTime1+=(int)(PassTime*100)/1000;
		PassTime2+=(int)(PassTime*100)/100%10;
		PassTime3+=(int)(PassTime*100)/10%10;
		PassTime4+=(int)(PassTime*100)%10;
	}

	if ((int)(PassTime*100) < 1000)
	{	DisplaySingleChar(0x09,0,0x30);
		DisplaySingleChar(0x0A,0,PassTime1);
		DisplaySingleChar(0x0B,0,'.');
		DisplaySingleChar(0x0C,0,PassTime2);
		DisplaySingleChar(0x0D,0,PassTime3);
	}
	else
	{
		DisplaySingleChar(0x09,0,PassTime1);
		DisplaySingleChar(0x0A,0,PassTime2);
		DisplaySingleChar(0x0B,0,'.');
		DisplaySingleChar(0x0C,0,PassTime3);
		DisplaySingleChar(0x0D,0,PassTime4);
	}	
}

/*-------------------------------------  ��ʾ·�� ----------------------------------------*/

void DisplayDistance(void)
{
	int Distance1=0x30;
	int Distance2=0x30;
	int Distance3=0x30;
	int Distance4=0x30;

	if ((int)(Distance*100) < 100)
	{
		Distance1+=0;
		Distance2+=(int)(Distance*100)/10;
		Distance3+=(int)(Distance*100)%10;
	}
	else if ((int)(Distance*100) > 100  && (int)(Distance*100) < 1000)
	{
		Distance1+=(int)(Distance*100)/100;
		Distance2+=(int)(Distance*100)/10%10;
		Distance3+=(int)(Distance*100)%10;
	}
	else
	{
		Distance1+=(int)(Distance*100)/1000;
		Distance2+=(int)(Distance*100)/100%10;
		Distance3+=(int)(Distance*100)/10%10;
		Distance4+=(int)(Distance*100)%10;
	}
	

	if ((int)(Distance*100) < 1000)
	{	DisplaySingleChar(0x09,1,0x30);
		DisplaySingleChar(0x0A,1,Distance1);
		DisplaySingleChar(0x0B,1,'.');
		DisplaySingleChar(0x0C,1,Distance2);
		DisplaySingleChar(0x0D,1,Distance3);
	}
	else
	{
		DisplaySingleChar(0x09,1,Distance1);
		DisplaySingleChar(0x0A,1,Distance2);
		DisplaySingleChar(0x0B,1,'.');
		DisplaySingleChar(0x0C,1,Distance3);
		DisplaySingleChar(0x0D,1,Distance4);
	}
}
/*����������������������������������ʱ������������������������������������������*/
//�������ܣ�    ��ʱ
//������ڲ������ַ���x��������ʱʱ��ĳ���
//��������ֵ��  ��
void Delay1()			 
{
    unsigned char a,b,c;
    for(c=3;c>0;c--)
        for(b=44;b>0;b--)
            for(a=173;a>0;a--);
}
void Delay2()
{
    unsigned char a,b,c;
    for(c=173;c>0;c--)
        for(b=162;b>0;b--)
            for(a=10;a>0;a--);
}
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
	EX0=1;
	IT0=1;									  //INTO���ش���
	PX0=1;          							  //INTO���ȼ�Ϊ�߼�
	EX1=1;									  //��INT1�ж�
	IT1=1;									  //INT1���ش���
	PX1=1;          							  //INT1���ȼ�Ϊ�߼�
//	Clock0_Init();                 				  //��ʼ��ʱ���ж�
	TMOD=0x11;								  //T0/T1��ʱ��ʽ1
	ET0=0x00;      							  //��T0�ж�
	ET1=0x01;     							  //��T1�ж�
	TR1=0x00;
}
void Clock0_Init(void)
{
	TR0=0x01;       							  //����T0
    TH0 = 0x0D8;
    TL0 = 0x0F0;

}
/*���������������������������������ⲿ�ж�0��������������������������������������*/
//�������ܣ�    �ⲿ�ж�0���������ڴ�
//������ڲ�������
//��������ֵ��  ��
void ext0() interrupt 0			 
{
	nts++;

} 
	/*if(blackline_detection==0)
    	if(startline_flag==1)
          {
        	Delay2();
			startline_flag=0;
	      }	
		else
		  {
		   Delay1();
		  }
	if(blackline_detection==0)	
	blackline++;
*/
/*���������������������������������ⲿ�ж�1��������������������������������������*/
//�������ܣ�    �ⲿ�ж�1����¼����ת����Ȧ��
//������ڲ�������
//��������ֵ��  ��
void ext1() interrupt 2			
{
	 SpeedCount++;
}
/*����������������������������������ʱ���ж�0��������������������������������������*/
//�������ܣ�    ��ʱ���ж�0��������¼ʱ��
//������ڲ�������
//��������ֵ��  ��
void Time1() interrupt 3      
{
	TR1=0x01;      							  //����T1
	TH1=0x4C;									  //��ʱ��ֵ-50ms�ж�һ��
	TL1=0x00;
	time_count++;
if (time_count == 20)
	{time_count = 0;}
if (blackn == 1)
{waitn++;
}	
	
	
PassTime+=0.05;

	  }
		

//void Time0() interrupt 1 		         //T0�ж�
//{  
//	TR0=1;
//    TH0 = 0x0D8;
//    TL0 = 0x0F0; 
//	j++;
//	if(j<=constant)
//	  {flag=1;}
//	if(j<=10&&j>constant)
//	  {flag=2;}   	
//	if(j==10) 
//		j=0;	
//}			
void Time0() interrupt 1 		         //T0�ж�
{  
	TR0=1;
    TH0 = 0x00;
    TL0 = 0x00; 
	j++;
	if(j>135)
	  {TR0=0;left_motor2=1;left_motor1=0;right_motor2=1;right_motor1=0;ET0=0;}
	
}		