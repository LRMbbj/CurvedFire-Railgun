#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "pwm.h"
#include "ur.h"
#include "hmi.h"
#include "railgun.h"
#include "openmv.h"


extern s16 rAng,pAng;

u8 IsEq( u8 *pBUF, char *s ) //判断是否相等，使用通配符'*'
{
	u8 i;
	for(i=0;i<DATA_SIZE;i++)
		if( *(pBUF+i) != *(s+i) && *(s+i) != '*' ) return 0;
	if(*(s+DATA_SIZE) != '\0') return 0;
	return 1;
}

char *int2str(int value, int radix)//整数转字符串
{
		char		*string = "";
    int     i, d;
    int     flag = 0;
    char    *ptr = string;
 
    /* This implementation only works for decimal numbers. */
    if (radix != 10)
    {
        *ptr = 0;
        return string;
    }
 
    if (!value)
    {
        *ptr++ = 0x30;
        *ptr = 0;
        return string;
    }
 
    /* if this is a negative value insert the minus sign. */
    if (value < 0)
    {
        *ptr++ = '-';
 
        /* Make the value positive. */
        value *= -1;
    }
 
    for (i = 10000; i > 0; i /= 10)
    {
        d = value / i;
 
        if (d || flag)
        {
            *ptr++ = (char)(d + 0x30);
            value -= (d * i);
            flag = 1;
        }
    }
 
    /* Null terminate the string. */
    *ptr = 0;
 
    return string;
 
}


void BaseAndTest()
{
	ClearBUF();//复位HMI缓冲
	while( !IsEq( USART3_RX_BUF, "DT2" ) );
	rAng = ReadInt() % 10000; // 零度中位
	pAng = ReadInt() % 10000;
	
	HMISetValue("t2.txt",int2str(rAng,10));
	HMISetValue("t3.txt",int2str(pAng,10));
	
	PWM_SetAngle(SG_Rotate, rAng );
	PWM_SetAngle(SG_Pitch, pAng );
	
	Railgun_Fire();//发射！
	
	PWM_SetAngle(SG_Rotate, 0 );
	PWM_SetAngle(SG_Pitch, 0 );//复位
	
	delay_ms(500);
	
	Railgun_Fire();
	
	while( !IsEq( USART3_RX_BUF, "OD0" )  )
	{
		HMISetValue("t5.txt",int2str( UR_Detect() ,10));
		delay_ms(100);
	}
	
	return;
	
}

void ExtentA()
{
	u16 dis;
	ClearBUF();//复位HMI缓冲
	
	rAng = -167;
	PWM_SetAngle(SG_Rotate, rAng );//复位电磁炮到R=-30 P=0
	OPENMV_State(ENABLE);//发送开始指令给OpenMV
	for(rAng = -167;rAng <= 167;rAng++)//主循环（扫过整个目标区域）迭代偏转角
	{
			PWM_SetAngle(SG_Rotate, rAng );//动作到目标位置
			delay_ms(9);//延迟一定时间
	}//此处退出循环
	rAng = 0;// !!!缺失 测量目标角度函数
	
	dis = UR_Detect();//打开测距并测量距离
	pAng = dis;//!!!缺失 换算发射仰角
	PWM_SetAngle(SG_Pitch, pAng );
	
	Railgun_Fire();//发射
	PWM_SetAngle(SG_Rotate, 0 );
	PWM_SetAngle(SG_Pitch, 0 );//复位
	
	return;
	
}

void ExtentB()
{
	
	ClearBUF();
	
	//复位电磁炮到R=-30 P=0
	//发送开始指令给OpenMV
	//循环1（对正时结束）
			//迭代偏转角
			//延迟一定时间
	//此处退出循环1
	//打开测距并测量距离
	//循环2（对正时结束）
			//迭代偏转角
			//延迟一定时间
	//此处退出循环2
	//数据输入发射函数(动态)
	//循环3（接收到结束指令后结束）
			//迭代偏转角
			//延迟一定时间
	//双零复位
	
	return;
	
}

int main(void)
{
	//Init
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组 2
	
	delay_init(168);
	uart_init(19200);
	Railgun_Init();
	UR_Init();
	PWM_Init();
	HMI_Init();
	
	PWM_SetAngle(SG_Rotate, 0 );
	PWM_SetAngle(SG_Pitch, 0 );//复位
	
	while(1)
	{
		if(USART3_RX_BUF[0] == 'O' && USART3_RX_BUF[1] == 'D') //ODx跳转到不同模式
			switch(USART3_RX_BUF[2])
			{	
				case '1':BaseAndTest();break;
				case '2':ExtentA();break;
				case '3':ExtentB();break;
			}
		delay_ms(10);
	}
	
	//return 0;
	
}
