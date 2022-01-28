#ifndef __CAR_CONTROL_H__
#define __CAR_CONTROL_H__
#include<stc12c5a60s2.h>
sbit M_CON1 = P1^3;
sbit M_CON2 = P1^5;
sbit M_CON3 = P1^4;
sbit M_CON4 = P1^6;
#if  1
#define  CAR_BACKWARD  M_CON1=1;M_CON2=0;M_CON3=1;M_CON4=0
#define  CAR_FORWARD   M_CON1=0;M_CON2=1;M_CON3=1;M_CON4=0
#define  CAR_LEFT     M_CON1=0;M_CON2=1;M_CON3=0;M_CON4=1
#define  CAR_RIGHT     M_CON1=1;M_CON2=0;M_CON3=1;M_CON4=0
#define  CAR_STOP      M_CON1=0;M_CON2=0;M_CON3=0;M_CON4=0
#endif 
#if  0
#define  CAR_BACKWARD M_CON1=1;M_CON2=0;M_CON3=1;M_CON4=0
#define  CAR_FORWARD   M_CON1=0;M_CON2=1;M_CON3=0;M_CON4=1
#define  CAR_RIGHT        M_CON1=0;M_CON2=1;M_CON3=1;M_CON4=0
#define  CAR_LEFT    M_CON1=1;M_CON2=0;M_CON3=0;M_CON4=1
#define  CAR_STOP      M_CON1=0;M_CON2=0;M_CON3=0;M_CON4=0
#endif
//void Car_Speed_Control(unsigned char speed_value,unsigned char IO_1,unsigned char IO_2);
extern void Car_Speed_Control(unsigned char L_Value,unsigned char R_Value,unsigned char IO_1,unsigned char IO_2);
void Car_set_speed(unsigned char IO_1,unsigned char  pwm0_value, unsigned char IO_2,unsigned char pwm1_value); 
#endif