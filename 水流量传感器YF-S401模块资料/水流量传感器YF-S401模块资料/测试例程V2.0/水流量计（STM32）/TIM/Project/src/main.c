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
** Descriptions:		ͨ�ö�ʱ��TIMʵ��				
**--------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:		
** Version:				
** Descriptions:		
** Rechecked by:			
**********************************************************************************************************/
/****-----���Ķ�ReadMe.txt����ʵ��-----***********/

#include "stm32f10x.h"
#include "delay.h"
#include "timer.h"
#include "OLED_I2C.h"

int count;
char buff[10];
float flow;
float speed;
/**************************************************************************************
 * ��  �� : ��ʼ��PB1Ϊ�ⲿ�ж�1
 * ��  �� : ��
 * ����ֵ : ��
 **************************************************************************************/
void EXTI_PB1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* Enable the EXTI_PB1 Clock */
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB , ENABLE); 
	
	//GPIO_DeInit(GPIOB);     //������GPIOB�Ĵ�������Ϊȱʡֵ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;    //��������   
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource1);   //����PB1�ܽ�Ϊ�ⲿ�ж���·��
	  
	EXTI_InitStructure.EXTI_Line = EXTI_Line1;      //����Ϊ�ⲿ�ж���1
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;        //����Ϊ�ж�����
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;      //������·�½���Ϊ�ж�����
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;	  //ʹ���ж�
  EXTI_Init(&EXTI_InitStructure);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);    //NVIC_Group:��ռ���ȼ�2λ�������ȼ�2λ  
  NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;   //����Ϊ�ⲿ�ж�1�ж�
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�Ϊ1
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;   //�����ȼ�Ϊ2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;      //ʹ���ж�ͨ��
  NVIC_Init(&NVIC_InitStructure); 
}

/**************************************************************************************
 * ��  �� : �ⲿ�жϷ������
 * ��  �� : ��
 * ����ֵ : ��
 **************************************************************************************/
void EXTI1_IRQHandler(void)
{
  count++;   //ָʾ����˸����
	
	EXTI_ClearITPendingBit(EXTI_Line1);   //���EXTI�ж���·����λ
}

/**************************************************************************************
 * ��  �� : MAIN����
 * ��  �� : ��
 * ����ֵ : ��
 **************************************************************************************/
int main(void)
{

	SystemInit();			//����ϵͳʱ��72MHZ

	
	TIM3_Init();                 //��ʼ������TIM
		I2C_Configuration();
	  OLED_Init();
	EXTI_PB1_Init();   //�ⲿ�ж�1��ʼ��

 	
		OLED_CLS();//����
	
		OLED_ShowCN(16,0,0);//��ʾ������
		OLED_ShowCN(32,0,1);//��ʾ��ӭ��
		OLED_ShowCN(48,0,2);//��ʾ��ʹ��
		OLED_ShowCN(64,0,3);//��ʾ���á�

		
		delay_ms(2000);  //��ʱ2S
		
    OLED_CLS();//����	
		OLED_ShowCN(16,0,10);//��ʾ������
		OLED_ShowCN(32,0,11);//��ʾ������
		OLED_ShowCN(48,0,12);//��ʾ���ơ�
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
		
		
		OLED_ShowStr(32,4,buff,2);//����6*8�ַ�		
	}
}
/*********************************END FILE********************************************/
