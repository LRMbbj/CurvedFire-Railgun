#include "railgun.h"

void Railgun_Init()
{
	
	GPIO_InitTypeDef GPIO_InitStruct;

	RCC_AHB1PeriphClockCmd(	RCC_AHB1Periph_GPIOF, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10; //GPIOA2与 GPIOA3 
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;//复用功能
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; //速度 50MHz 
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init( GPIOF, &GPIO_InitStruct ); //初始化 PA9 PA10
	
	GPIO_ResetBits (GPIOF, GPIO_Pin_10);//充电
	
	delay_init(168);
}
void Railgun_Fire()
{
	
	GPIO_SetBits(GPIOF,GPIO_Pin_10);//开启发射
	delay_ms(500);
	GPIO_ResetBits (GPIOF, GPIO_Pin_10);//关闭发射，充电
	delay_ms(500);
	//发射步骤需要1s
}	
