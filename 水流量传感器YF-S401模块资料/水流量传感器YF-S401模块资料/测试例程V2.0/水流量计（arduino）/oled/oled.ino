#include "U8glib.h"
#include <MsTimer2.h>
 int i =0; 
 float speeds,flow; 
U8GLIB_SSD1306_128X64 u8g(10, 9, 12, 11, 13); //创建对象,VCC 3.3V |GND GND|D0 D10|D1 D9|RES D13|DC D11|CS D12
  
void setup(void) {
 attachInterrupt(0, COUNT, RISING);   //中断引脚 D2
 MsTimer2::set(1000, dongzuo);
 MsTimer2::start();
}
  
void loop(void) {
//    for(int i=10;i<80;i++){
     u8g.firstPage();  //表示图像循环的开始
     do {
         u8g.setFont(u8g_font_fub20);//设置字体和自号
         u8g.setPrintPos(0, 50); //显示的位置
         u8g.print(flow);//显示变量i的值
        u8g.setFont(u8g_font_fub14);//设置字体和自号
         u8g.setPrintPos(118, 50); //显示的位置
         u8g.print("L");//显示
         }while( u8g.nextPage() );//u8g.nextPage() 表示图像循环的结束
      delay(500);//显示的时间间隔
//    }
}

void COUNT()
{
  i++;
}
void dongzuo()
{
  speeds = i/60/5.5;
  flow += speeds;
  speeds = 0;
  i = 0;
  
  
}

