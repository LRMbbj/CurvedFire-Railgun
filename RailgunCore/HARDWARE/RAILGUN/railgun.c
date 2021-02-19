#include "railgun.h"

void Railgun_Init()
{
	
	GPIO_InitTypeDef GPIO_InitStruct;

	RCC_AHB1PeriphClockCmd(	RCC_AHB1Periph_GPIOF, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10; //GPIOA2�� GPIOA3 
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;//���ù���
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; //�ٶ� 50MHz 
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init( GPIOF, &GPIO_InitStruct ); //��ʼ�� PA9 PA10
	
	GPIO_ResetBits (GPIOF, GPIO_Pin_10);//���
	
	delay_init(168);
}
void Railgun_Fire()
{
	
	GPIO_SetBits(GPIOF,GPIO_Pin_10);//��������
	delay_ms(500);
	GPIO_ResetBits (GPIOF, GPIO_Pin_10);//�رշ��䣬���
	delay_ms(500);
	//���䲽����Ҫ1s
}	
