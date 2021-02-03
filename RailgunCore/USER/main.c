#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "pwm.h"
#include "ur.h"
#include "hmi.h"


void BaseAndTest()
{
	
	USART_RX_BUF[0] = 0;
	return;
	
}

void ExtentA()
{
	
	USART_RX_BUF[0] = 0;
	return;
	
}

void ExtentB()
{
	
	USART_RX_BUF[0] = 0;
	return;
	
}

int main(void)
{
	//Init
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组 2
	
	u8 lcd_id[12];
	delay_init(168);
	uart_init(115200);
	PWM_Init();
	HMI_Init();
	
	while(1)
	{
		switch(USART_RX_BUF[0])
		{
			case 1:BaseAndTest();break;
			case 2:ExtentA();break;
			case 3:ExtentB();break;
		}
	}
	
	//return 0;
	
}
