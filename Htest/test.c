
#include <reg52.h>
#include "intrins.h"
#define uint unsigned int
#define uchar unsigned char

 //���尴��

  sbit RS=P2^7;  //  RS ���� 0���������ݣ�1���������
  sbit RW=P2^6;  //  RW ���� 0����LCDд��ָ��������� 1����LCD��ȡ����
  sbit E=P2^5;   //ʹ���ź�  1����ȡ��Ϣ  1->0(�½���):ִ��ָ��
#define LCD1602_DATA			P0
#define LCD1602_DATAPINS 	P0

 //��ʱ����
    void delay100us()	     //100us ��ʱ����
     {					   
       unsigned char a,b;
       for(b=1;b>0;b--)
       for(a=47;a>0;a--);
     }
		
		 
 //��1602д��ָ��
  void write1602(uchar com) 
    {
	    RS=0;    //����ָ��
	    RW=0;    //��LCDд��ָ��
	    E=0;     //ִ��ָ��
	    P0=com; 
			
	    delay100us();
	    E=1;    //��һ��������
			delay100us();
	    E=0;
    }
		
 //��1602д������
  void writedat(uchar dat)  
    {
	    RS=1;   //�������
	    RW=0;   //��LCD��ȡ��Ϣ
	    E=0;    //ִ��ָ��
	    P0=dat;
			
	    delay100us();
	    E=1;    //��һ��������
			delay100us();
	    E=0;
    }	
		
 //1602��ʼ��
   void init1602()	   
     {
	     write1602(0x38);    //������ʾģʽ
	     write1602(0x0c);    //����ʾ������ʾ��� ��겻��˸
	     write1602(0x06);    //дһ���ַ��󣬵�ַָ���Զ���һ
	     write1602(0x01);    //��ʾ��0������ָ����0
			 write1602(0x80);    //��������ָ�����
     }

/*************��ʱ����**************/
void delay(unsigned char i)//��ʱ
{
  unsigned char j,k;
  for(j=i;j>0;j--)
    for(k=125;k>0;k--);
}
#define X -200
unsigned char count;      //0.5ms������ʶ
sbit pwm =P3^0 ;          //PWM�ź����
sbit jia =P3^7;           //�Ƕ����Ӱ���
sbit jian =P3^6;           //�Ƕȼ��ٰ���
unsigned char jd;         //�Ƕȱ�ʶ


void Time0_Init()          //��ʱ����ʼ��
{
        TMOD = 0x01;           //��ʱ��0�����ڷ�ʽ1 16λ  
        IE   = 0x82;
        TH0  = X>>8;
        TL0  = X;                   //11.0592MZ����0.5ms  
        TR0=1;                 //��ʱ����ʼ
}

void Time0_Int() interrupt 1 //�жϳ���
{

        TH0  = X>>8;             //���¸�ֵ
        TL0  = X;
     if(count<5)
    {
       pwm=0;
    }
    else{
    if(count<jd+5)              //�ж�0.2ms�����Ƿ�С�ڽǶȱ�ʶ
      pwm=1;                  //ȷʵС�ڣ�PWM����ߵ�ƽ
    else
      pwm=0;  
    }                //����������͵�ƽ

    count=(count+1);          //0.5ms������1
    if(count>=100)
    {
       count=0;
    }
  //����ʼ�ձ���Ϊ100 ����������Ϊ20ms
}



void keyscan()              //����ɨ��
{
   if(jia==0)               //�Ƕ����Ӱ����Ƿ���
  {
    delay(10);              //������ʱ������
    if(jia==0)              //ȷʵ����
     {
      jd++;           //�Ƕȱ�ʶ��1
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
        
      count=0;              //�������� ��20ms���ڴ��¿�ʼ
      if(jd==6)
        jd=5;               //�Ѿ���90�ȣ��򱣳�
      while(jia==0);        //�ȴ������ſ�
     }
  }

  if(jian==0)                //�Ƕȼ�С�����Ƿ���
  {
    delay(10);
    if(jian==0)
     {
      jd--;     //�Ƕȱ�ʶ��1
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
        jd=1;   //�Ѿ���-90�ȣ��򱣳�
      while(jian==0);
     }
  }
}

void main()
{
 init1602();	//1602��ʼ��
jd=1;
count=0;
Time0_Init();           
while(1)
{

  keyscan();               //����ɨ��
}

}
