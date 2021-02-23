#include "openmv.h"

void OPENMV_Init(void)
{
	
	GPIO_InitTypeDef GPIO_InitStruct;

	RCC_AHB1PeriphClockCmd(	RCC_AHB1Periph_GPIOA, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7; //GPIOA2与 GPIOA3 
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;//复用功能
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; //速度 50MHz 
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init( GPIOF, &GPIO_InitStruct ); //初始化 PA7
	
	GPIO_ResetBits (GPIOF, GPIO_Pin_7);//使能为否
	
}	//初始化OpenMV
void OPENMV_State(u8 state)
{
	
	if(state == DISABLE) GPIO_ResetBits (GPIOF, GPIO_Pin_7);//使能为否
	if(state == ENABLE) GPIO_SetBits (GPIOF, GPIO_Pin_7);//使能为是
	
}	//使能设置
