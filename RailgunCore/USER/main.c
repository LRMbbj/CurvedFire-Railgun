#include "delay.h"
#include "pwm.h"
#include "ur.h"
#include "hmi.h"
#include "railgun.h"
#include "openmv.h"

char str[5];
s16 rAng,pAng;

u8 IsEq( u8 *pBUF, char *s ) //判断是否相等，使用通配符'*'
{
	u8 i;
	for(i=0;i<DATA_SIZE;i++)
		if( *(pBUF+i) != *(s+i) && *(s+i) != '*' ) return 0;
	if(*(s+DATA_SIZE) != '\0') return 0;
	return 1;
}

char *int2str(u16 value)//整数转字符串
{

	u16 val = value;
	u16 i = 1000,j = 0;
	for (j = 0; j < 5; j++)
	{
		str[j] = '\0';
	}
	j = 0;
	while (j < 4 && i > 0)
	{
		str[j] = val / i + 48;
		val %= i;
		j++;
		i /= 10;
	}
	
	return (char*)str;

}


void BaseAndTest()
{
	u16 dis = 0;
	ClearBUF();//复位HMI缓冲
	while( !IsEq( USART3_RX_BUF, "DT2" ) );
	ClearBUF();
	rAng = ReadInt() % 10000; // 零度中位
	pAng = ReadInt() % 10000;
	
	HMISetValue("t2.txt",int2str(rAng));
	HMISetValue("t3.txt",int2str(pAng));
	
	PWM_SetAngle(SG_Rotate, rAng );
	PWM_SetAngle(SG_Pitch, pAng );
	
	Railgun_Fire();//发射！
	
	PWM_SetAngle(SG_Rotate, 0 );
	PWM_SetAngle(SG_Pitch, 0 );//复位
	
	delay_ms(500);
	
	Railgun_Fire();
	
	while( !IsEq( USART3_RX_BUF, "OD0" )  )
	{
		dis = 0;
		dis = UR_Detect();
		if( dis != 0 ) HMISetValue("t5.txt",int2str( dis ));
		delay_ms(100);
	}
	
	return;
	
}

void ExtentA()
{
	u16 dis;
	s8 tt =0;
	ClearBUF();//复位HMI缓冲
	
//	rAng = -222;
//	PWM_SetAngle(SG_Rotate, rAng );//复位电磁炮到R=-30 P=0
	
	OPENMV_State(ENABLE);//发送开始指令给OpenMV
	
//	for(rAng = -222;rAng <= 221;rAng+=6)//主循环1（扫过整个目标区域）迭代偏转角
//	{
//			PWM_SetAngle(SG_Rotate, rAng );
//			delay_ms(40);//延迟一定时间
//	}//此处退出循环
//	for(rAng = 221;rAng >= -222;rAng-=6)//主循环1（扫过整个目标区域）迭代偏转角
//	{
//			PWM_SetAngle(SG_Rotate, rAng );
//			delay_ms(40);//延迟一定时间
//	}//此处退出循环
	
	rAng = 0;
	PWM_SetAngle(SG_Rotate, rAng );
	delay_ms(500);
	while(GetBias()>3||GetBias()<-3)
	{
		tt=GetBias();
		if(tt<-120) continue;
		rAng += GetBias()/10%20;
		PWM_SetAngle(SG_Rotate, rAng );
		delay_ms(50);
		tt=GetBias();
	}
	HMISetValue("t3.txt",int2str(rAng));
	OPENMV_State(DISABLE);//发送结束指令给OpenMV
	
	//PWM_SetAngle(SG_Rotate, rAng );//动作到目标位置
	dis = UR_Detect();//打开测距并测量距离
	
	pAng = 100;//!!!缺失 换算发射仰角
	PWM_SetAngle(SG_Pitch, pAng );
	
	Railgun_Fire();//发射
	//PWM_SetAngle(SG_Rotate, 0 );
	PWM_SetAngle(SG_Pitch, 0 );//复位
	
	OPENMV_State(ENABLE);//发送开始指令给OpenMV
	while( !IsEq( USART3_RX_BUF, "OD0" )  );
	OPENMV_State(DISABLE);//发送结束始指令给OpenMV
	
	return;
	
}

void ExtentB()
{
	s16 target;
	ClearBUF();
	
	rAng = -222;
	pAng = 0;
	PWM_SetAngle(SG_Rotate, rAng );
	PWM_SetAngle(SG_Pitch, pAng ); //复位电磁炮到R=-30 P=0
	OPENMV_State(ENABLE); //发送开始指令给OpenMV
	
	//循环1 -30->30->-30
	for(rAng = -222;rAng <= 221;rAng+=3)//主循环1（扫过整个目标区域）迭代偏转角
	{
			PWM_SetAngle(SG_Rotate, rAng );
			delay_ms(30);//延迟一定时间
	}//此处退出循环
	for(rAng = 221;rAng >= -222;rAng-=3)//主循环1（扫过整个目标区域）迭代偏转角
	{
			PWM_SetAngle(SG_Rotate, rAng );
			delay_ms(20);//延迟一定时间
	}//此处退出循环

	target = GetBias();//测量目标角度函数
	OPENMV_State(DISABLE);//发送结束指令给OpenMV	
	
	for(rAng = -222;rAng < target;rAng+=3)//主循环1（扫过整个目标区域）迭代偏转角
	{
			PWM_SetAngle(SG_Rotate, rAng );
			delay_ms(20);//延迟一定时间
	}//此处退出循环
	
	PWM_SetAngle(SG_Rotate, target );
	
	GPIO_SetBits(GPIOF,GPIO_Pin_10);//手动发射
	
	for(rAng = target;rAng <= 221;rAng+=3)//主循环1（扫过整个目标区域）迭代偏转角
	{
			PWM_SetAngle(SG_Rotate, rAng );
			delay_ms(20);//延迟一定时间
	}//此处退出循环

	for(rAng = 221;rAng >= -222;rAng-=3)//主循环（扫过整个目标区域）迭代偏转角
	{
			PWM_SetAngle(SG_Rotate, rAng );//动作到目标位置
			delay_ms(20);//延迟一定时间
	}//循环3 目标位->30->0
	
	
	rAng = 0;
	pAng = 0;
	PWM_SetAngle(SG_Rotate, rAng );
	PWM_SetAngle(SG_Pitch, pAng ); //双零复位
	
	
	GPIO_ResetBits(GPIOF,GPIO_Pin_10);//复位发射
	return;
	
}

int main(void)
{
	//Init
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组 2
	
	delay_init(168);
	Railgun_Init();
	UR_Init();
	PWM_Init();
	HMI_Init();
	OPENMV_Init();
	
	delay_ms(1000);
	
	rAng = 0;
	pAng = 0;
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
