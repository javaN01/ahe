
#include <reg52.h>
#include "intrins.h"
#define uint unsigned int
#define uchar unsigned char

 //定义按键

  sbit RS=P2^7;  //  RS 输入 0：输入数据；1：输出数据
  sbit RW=P2^6;  //  RW 输入 0：向LCD写入指令或者数据 1：从LCD读取数据
  sbit E=P2^5;   //使能信号  1：读取信息  1->0(下降沿):执行指令
#define LCD1602_DATA			P0
#define LCD1602_DATAPINS 	P0

 //延时函数
    void delay100us()	     //100us 延时函数
     {					   
       unsigned char a,b;
       for(b=1;b>0;b--)
       for(a=47;a>0;a--);
     }
		
		 
 //向1602写入指令
  void write1602(uchar com) 
    {
	    RS=0;    //输入指令
	    RW=0;    //向LCD写入指令
	    E=0;     //执行指令
	    P0=com; 
			
	    delay100us();
	    E=1;    //给一个高脉冲
			delay100us();
	    E=0;
    }
		
 //向1602写入数据
  void writedat(uchar dat)  
    {
	    RS=1;   //输出数据
	    RW=0;   //从LCD读取信息
	    E=0;    //执行指令
	    P0=dat;
			
	    delay100us();
	    E=1;    //给一个高脉冲
			delay100us();
	    E=0;
    }	
		
 //1602初始化
   void init1602()	   
     {
	     write1602(0x38);    //设置显示模式
	     write1602(0x0c);    //开显示，不显示光标 光标不闪烁
	     write1602(0x06);    //写一个字符后，地址指针自动加一
	     write1602(0x01);    //显示清0，数据指针清0
			 write1602(0x80);    //设置数据指针起点
     }

/*************延时函数**************/
void delay(unsigned char i)//延时
{
  unsigned char j,k;
  for(j=i;j>0;j--)
    for(k=125;k>0;k--);
}
#define X -200
unsigned char count;      //0.5ms次数标识
sbit pwm =P3^0 ;          //PWM信号输出
sbit jia =P3^7;           //角度增加按键
sbit jian =P3^6;           //角度减少按键
unsigned char jd;         //角度标识


void Time0_Init()          //定时器初始化
{
        TMOD = 0x01;           //定时器0工作在方式1 16位  
        IE   = 0x82;
        TH0  = X>>8;
        TL0  = X;                   //11.0592MZ晶振，0.5ms  
        TR0=1;                 //定时器开始
}

void Time0_Int() interrupt 1 //中断程序
{

        TH0  = X>>8;             //重新赋值
        TL0  = X;
     if(count<5)
    {
       pwm=0;
    }
    else{
    if(count<jd+5)              //判断0.2ms次数是否小于角度标识
      pwm=1;                  //确实小于，PWM输出高电平
    else
      pwm=0;  
    }                //大于则输出低电平

    count=(count+1);          //0.5ms次数加1
    if(count>=100)
    {
       count=0;
    }
  //次数始终保持为100 即保持周期为20ms
}



void keyscan()              //按键扫描
{
   if(jia==0)               //角度增加按键是否按下
  {
    delay(10);              //按下延时，消抖
    if(jia==0)              //确实按下
     {
      jd++;           //角度标识加1
       if(jd==1)
       {
         writedat('-');
         writedat('9');
         writedat('0');
         write1602(0x80);
       }
         if(jd==2)
       {
          writedat('-');
         writedat('4');
         writedat('5');
         write1602(0x80);
             
       }
        if(jd==3)
       {
          writedat('0');
         writedat('0');
         writedat('0');
         write1602(0x80);
       }
        if(jd==4)
       {
           writedat('+');
         writedat('4');
         writedat('5');
          write1602(0x80);
       }
        if(jd==5)
       {
           writedat('+');
         writedat('9');
         writedat('0');
           write1602(0x80);
       }
        
      count=0;              //按键按下 则20ms周期从新开始
      if(jd==6)
        jd=5;               //已经是90度，则保持
      while(jia==0);        //等待按键放开
     }
  }

  if(jian==0)                //角度减小按键是否按下
  {
    delay(10);
    if(jian==0)
     {
      jd--;     //角度标识减1
       if(jd==1)
       {
         writedat('-');
         writedat('9');
         writedat('0');
         write1602(0x80);
       }
         if(jd==2)
       {
          writedat('-');
         writedat('4');
         writedat('5');
         write1602(0x80);
             
       }
        if(jd==3)
       {
          writedat('0');
         writedat('0');
         writedat('0');
         write1602(0x80);
       }
        if(jd==4)
       {
           writedat('+');
         writedat('4');
         writedat('5');
          write1602(0x80);
       }
        if(jd==5)
       {
           writedat('+');
         writedat('9');
         writedat('0');
           write1602(0x80);
       }
     
      count=0;
      if(jd==0)
        jd=1;   //已经是-90度，则保持
      while(jian==0);
     }
  }
}

void main()
{
 init1602();	//1602初始化
jd=1;
count=0;
Time0_Init();           
while(1)
{

  keyscan();               //按键扫描
}

}
