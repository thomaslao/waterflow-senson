/****************************************Copyright (c)****************************************************
**                                        
**                                 
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			
** Last modified Date:          
** Last Version:		   
** Descriptions:							
**--------------------------------------------------------------------------------------------------------
** Created by:			FiYu
** Created date:		2015-11-28
** Version:			    1.0
** Descriptions:		通用定时器TIM实验				
**--------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:		
** Version:				
** Descriptions:		
** Rechecked by:			
**********************************************************************************************************/
/****-----请阅读ReadMe.txt进行实验-----***********/

#include "stm32f10x.h"
#include "delay.h"
#include "timer.h"
#include "OLED_I2C.h"

int count;
char buff[10];
float flow;
float speed;
/**************************************************************************************
 * 描  述 : 初始化PB1为外部中断1
 * 入  参 : 无
 * 返回值 : 无
 **************************************************************************************/
void EXTI_PB1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* Enable the EXTI_PB1 Clock */
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB , ENABLE); 
	
	//GPIO_DeInit(GPIOB);     //将外设GPIOB寄存器重设为缺省值
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;    //浮空输入   
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource1);   //配置PB1管脚为外部中断线路用
	  
	EXTI_InitStructure.EXTI_Line = EXTI_Line1;      //配置为外部中断线1
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;        //配置为中断请求
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;      //输入线路下降沿为中断请求
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;	  //使能中断
  EXTI_Init(&EXTI_InitStructure);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);    //NVIC_Group:先占优先级2位，从优先级2位  
  NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;   //配置为外部中断1中断
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级为1
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;   //从优先级为2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;      //使能中断通道
  NVIC_Init(&NVIC_InitStructure); 
}

/**************************************************************************************
 * 描  述 : 外部中断服务程序
 * 入  参 : 无
 * 返回值 : 无
 **************************************************************************************/
void EXTI1_IRQHandler(void)
{
  count++;   //指示灯闪烁函数
	
	EXTI_ClearITPendingBit(EXTI_Line1);   //清除EXTI中断线路挂起位
}

/**************************************************************************************
 * 描  述 : MAIN函数
 * 入  参 : 无
 * 返回值 : 无
 **************************************************************************************/
int main(void)
{

	SystemInit();			//设置系统时钟72MHZ

	
	TIM3_Init();                 //初始化配置TIM
		I2C_Configuration();
	  OLED_Init();
	EXTI_PB1_Init();   //外部中断1初始化

 	
		OLED_CLS();//清屏
	
		OLED_ShowCN(16,0,0);//显示“欢”
		OLED_ShowCN(32,0,1);//显示“迎”
		OLED_ShowCN(48,0,2);//显示“使”
		OLED_ShowCN(64,0,3);//显示“用”

		
		delay_ms(2000);  //延时2S
		
    OLED_CLS();//清屏	
		OLED_ShowCN(16,0,10);//显示“流”
		OLED_ShowCN(32,0,11);//显示“量”
		OLED_ShowCN(48,0,12);//显示“计”
		buff[4]='.';
		buff[7]='L';
  while(1)
  {

		buff[0] = (int)(flow*100)/100000+'0';
		buff[1] = (int)(flow*100)%100000/10000+'0';
		buff[2] = (int)(flow*100)%10000/1000+'0';
		buff[3] = (int)(flow*100)%1000/100+'0';
		buff[5] = (int)(flow*100)%100/10+'0';
		buff[6] = (int)(flow*100)%10+'0';
		
		
		OLED_ShowStr(32,4,buff,2);//测试6*8字符		
	}
}
/*********************************END FILE********************************************/
