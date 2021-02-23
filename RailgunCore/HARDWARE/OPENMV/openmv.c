#include "openmv.h"

void OPENMV_Init(void)
{
	
	GPIO_InitTypeDef GPIO_InitStruct;

	RCC_AHB1PeriphClockCmd(	RCC_AHB1Periph_GPIOA, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7; //GPIOA2�� GPIOA3 
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;//���ù���
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; //�ٶ� 50MHz 
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init( GPIOF, &GPIO_InitStruct ); //��ʼ�� PA7
	
	GPIO_ResetBits (GPIOF, GPIO_Pin_7);//ʹ��Ϊ��
	
}	//��ʼ��OpenMV
void OPENMV_State(u8 state)
{
	
	if(state == DISABLE) GPIO_ResetBits (GPIOF, GPIO_Pin_7);//ʹ��Ϊ��
	if(state == ENABLE) GPIO_SetBits (GPIOF, GPIO_Pin_7);//ʹ��Ϊ��
	
}	//ʹ������
