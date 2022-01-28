#include <reg52.h>				       //包含头文件
#include <lcd.h>
#include <intrins.h>


#define uint unsigned int		               //宏常量定义
#define uchar unsigned char		               //宏常量定义
#define CircleLength		0.204				//小车转一轮的长度为.204m


sbit left_motor1 = P1^3;
sbit left_motor2 = P1^4;
sbit right_motor1 = P1^5;
sbit right_motor2 = P1^6;





sbit blackline_detection1 =P3^6;					       //P3_2设置为黑带检测位
uint blackn=0;	
uint waitn=0;	
uint blackline=0;			       //定义几个整形变量
uint nts=0;	   
//uint aaa=1;	
uint bbb=1;	
uint ccc=1;	//过黑线次数
uint ddd=1;
uint time_count=0;
char   ReadyToGo1=4;						//倒计时
char   ReadyToGo=10;
bit    start_flag=1;
bit    Running;
char   p=0;
float  SpeedCount=0;			      				//测速计数脉冲
float  Speed = 0.0;							
float  Distance = 0.0;                 			
float  PassTime=0.00;                	//小车行走的时间
char   Time0INTCount=0;								//T1中断时间计时
uchar forward=1;/***高低电平标志***/
uchar PWM_flag=1;
uchar startline_flag=1;
uchar flag;/***高低电平标志***/
bit direction;/***方向标志***/
static uchar j=0;
static uchar constant=5;//可以改变占空比
void motor_control();				       //声明一个电机控制函数
void PWM_control();
void Delay1();			       //声明一个延时函数
void Delay2();
void interrupt_init();
void ComputeSpeedANDDistance();
void LCD_Display();
void DelayUs(unsigned int time);
void Clock0_Init(void);
void Clock1_Init(void);
void reverse(void);
void Delay(unsigned int time);						//延时主程序
void pwm_control();
void heixianjiance();
/*————————————————主函数———————————————————*/
void main()    
{	
  interrupt_init();
	LCDInit();
		DisplayString(0x0,0,"  Welcome...  ");
	DisplayString(0x0,1,"Autorunning Car");
	Delay(300);
	WriteCmd(LCD_CLS);
	nts=0;
	TR1=0x01;      							  //启动T1
	TH1=0x4C;									  //定时初值-50ms中断一次
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
	LCDInit();										//初始化LCD
	DisplayString(0x0,0,"  Welcome...  ");
	DisplayString(0x0,1,"Autorunning Car");
	Delay(300);
	WriteCmd(LCD_CLS);

	while(1)						   //进入主循环
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
		 
		// pwm_control();						   //进入电机控制函数
		// motor_control();
		// pwm_control();
		// if(p==1)
		 //  {LCD_Display();}
//	}

/*————————————————电机控制———————————————————*/
//函数功能：    控制电机运转
//函数入口参数：无
//函数返回值：  无 
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
			   case 3: constant=5; break;//可以改变占空比
			   case 4: constant=0;break;	 
			   case 5: constant=0;break;
			   case 6:{	  
			            forward=0;//转向标志位
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
			   case 9:  constant=5; break;//可以改变占空比
			   case 10:	constant=0; break;//可以改变占空比
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
	            left_motor1=0;left_motor2=0;right_motor1=0;	right_motor2=0;	//行程结束，小车停止																		  	
           	}	
}  
*/

void LCD_Display()
{
 	
   // ChoosingDisplay=True;								  
//	WriteCmd(LCD_CLS);

	/*首先显示主菜单，然后显示第一个选项*/
//	DisplayString(0x0,0,"Now Choose what ");
//	DisplayString(0x0,1,"you want to see ");
//	Delay(100);
//	WriteCmd(LCD_CLS);

 // WriteCmd(LCD_CLS);

			DisplayTime();
			
            //WriteCmd(LCD_CLS);

			DisplayDistance();

		
}
/*————————————————计算函数———————————————————*/
void ComputeSpeedANDDistance(void)
{
	Speed=SpeedCount*CircleLength/20;					//计算瞬时速度
	Distance+=Speed;								//计算距离						
	SpeedCount=0;
}

/*-------------------------------------  显示时间 ----------------------------------------*/
void DisplayTime(void)
{
	char PassTime1=0x30;
	char PassTime2=0x30;
	char PassTime3=0x30;
	char PassTime4=0x30;
	
	if ((int)PassTime*100<100)                  		//时间未够1s
	{
		PassTime1+=0;      
		PassTime2+=(int)(PassTime*100)/10;
		PassTime3+=(int)(PassTime*100)%10;
	}
	else if ((int)(PassTime*100) > 100  && (int)(PassTime*100) < 1000) //够1s而未够10s
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

/*-------------------------------------  显示路程 ----------------------------------------*/

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
/*————————————————延时函数———————————————————*/
//函数功能：    延时
//函数入口参数：字符型x，储存延时时间的长短
//函数返回值：  无
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
/*————————————————中断初始化———————————————————*/
 void interrupt_init()
{
	EA=1;									  //开总中断
	EX0=1;
	IT0=1;									  //INTO边沿触发
	PX0=1;          							  //INTO优先级为高级
	EX1=1;									  //开INT1中断
	IT1=1;									  //INT1边沿触发
	PX1=1;          							  //INT1优先级为高级
//	Clock0_Init();                 				  //初始化时钟中断
	TMOD=0x11;								  //T0/T1定时方式1
	ET0=0x00;      							  //开T0中断
	ET1=0x01;     							  //开T1中断
	TR1=0x00;
}
void Clock0_Init(void)
{
	TR0=0x01;       							  //启动T0
    TH0 = 0x0D8;
    TL0 = 0x0F0;

}
/*————————————————外部中断0———————————————————*/
//函数功能：    外部中断0，用来检测黑带
//函数入口参数：无
//函数返回值：  无
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
/*————————————————外部中断1———————————————————*/
//函数功能：    外部中断1，记录车轮转过的圈数
//函数入口参数：无
//函数返回值：  无
void ext1() interrupt 2			
{
	 SpeedCount++;
}
/*————————————————定时器中断0———————————————————*/
//函数功能：    定时器中断0，用来记录时间
//函数入口参数：无
//函数返回值：  无
void Time1() interrupt 3      
{
	TR1=0x01;      							  //启动T1
	TH1=0x4C;									  //定时初值-50ms中断一次
	TL1=0x00;
	time_count++;
if (time_count == 20)
	{time_count = 0;}
if (blackn == 1)
{waitn++;
}	
	
	
PassTime+=0.05;

	  }
		

//void Time0() interrupt 1 		         //T0中断
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
void Time0() interrupt 1 		         //T0中断
{  
	TR0=1;
    TH0 = 0x00;
    TL0 = 0x00; 
	j++;
	if(j>135)
	  {TR0=0;left_motor2=1;left_motor1=0;right_motor2=1;right_motor1=0;ET0=0;}
	
}		