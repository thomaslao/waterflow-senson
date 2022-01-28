#include <reg52.h>				       //包含头文件
#include <lcd.h>
#include <intrins.h>


#define uint unsigned int		               //宏常量定义
#define uchar unsigned char		             //宏常量定义



   




float  SpeedCount=0;			      				//测速计数脉冲
float  Speed = 0.0;				      				//瞬时速度						
float  flow = 0.0;			      			//流量                 			
uint time_count;


void interrupt_init();			       //声明中断初始化函数
void ComputeSpeedANDDistance();    //声明速度距离计算函数

void DelayUs(unsigned int time);   //延时子函数
void Delay(unsigned int time);		 //延时主程序
void DisplayFlow(void);
/*————————————————主函数———————————————————*/
void main()    
{	

  interrupt_init();                       //中断初始化
	LCDInit();                              //显示屏初始化
	DisplayString(0x0,0,"1sensor.taobao");
	DisplayString(0x0,1,"          .com");
	Delay(300);
	WriteCmd(LCD_CLS);                      //清屏 


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





/*————————————————速度、流量计算函数———————————————————*/
void ComputeSpeedANDDistance(void)
{
	Speed=SpeedCount/5.5/60;					//计算1秒流量
	flow+=Speed;								          //计算距离						
	SpeedCount=0;
}
//void ComputeSpeedANDDistance(void)
//{
//	flow+=SpeedCount;								          //计算距离						
//	SpeedCount=0;
//}
/*-------------------------------------  显示流量 ----------------------------------------*/
//函数功能：    显示路程
//函数入口参数：无
//函数返回值：  无
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

/*————————————————延时函数———————————————————*/
//函数功能：    延时
//函数入口参数：字符型x，储存延时时间的长短
//函数返回值：  无
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
	EX0=1;								  //开外部中断0
	IT0=1;									//INTO边沿触发
	PX0=1;          				//INTO优先级为高级
	EX1=1;									//开INT1中断
	IT1=1;									//INT1边沿触发
	PX1=1;          				//INT1优先级为高级
	
	TMOD=0x11;							//T0/T1定时方式1
	ET0=0x00;      					//关T0中断
	ET1=0x01;     					//开T1中断

  TR0=0x00;
	TR1=0x01;      							            //启动T1
	TH1=0x4C;									              //定时初值-50ms中断一次
	TL1=0x00;	
}


/*————————————————外部中断1———————————————————*/
//函数功能：    外部中断1，记录车轮转过的圈数
//函数入口参数：无
//函数返回值：  无
void ext1() interrupt 2			
{
	 SpeedCount++;
}

/*————————————————定时器中断1———————————————————*/
//函数功能：    定时器中断1，用来记录时间
//函数入口参数：无
//函数返回值：  无
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


