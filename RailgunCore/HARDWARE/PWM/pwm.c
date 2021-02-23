#include "pwm.h"


void PWM_Init(void)
{
	
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIMInitStruct;
	TIM_OCInitTypeDef TIMOCInitStruct;
	
	
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM3, ENABLE);
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOC, ENABLE); //��ʼ����ʱ��
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_High_Speed;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP; //����GPIO��ʼ���ṹ��
	
	GPIO_Init( GPIOF, &GPIO_InitStruct); //��ʼ��GPIO
	
	GPIO_PinAFConfig( GPIOF, GPIO_PinSource6, GPIO_AF_TIM3);
	GPIO_PinAFConfig( GPIOF, GPIO_PinSource7, GPIO_AF_TIM3); //����IO����
	
	
	TIMInitStruct.TIM_Period = 9999;
	TIMInitStruct.TIM_Prescaler = 167; //����10000Ϊ20ms(���Ե�������)
	TIMInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIMInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit( TIM3, &TIMInitStruct );
	
	TIMOCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIMOCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIMOCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
	TIMOCInitStruct.TIM_Pulse = 750; // 250=>0.5ms=>0�� 1250=>2.5ms=>180��
	TIM_OC1Init( TIM3, &TIMOCInitStruct );
	TIM_OC2Init( TIM3, &TIMOCInitStruct );
	
	TIM_OC1PreloadConfig( TIM3, TIM_OCPreload_Enable );
	TIM_OC2PreloadConfig( TIM3, TIM_OCPreload_Enable );
	
	TIM_ARRPreloadConfig( TIM3, ENABLE );
	
	TIM_Cmd( TIM3, ENABLE );
	
}

void PWM_SetAngle(u8 index,s16 angle) //0=>0�� 1000=>180��
{
	
	assert_param(IS_SG_CODE(index));
	
	if( index == SG_Rotate )
	{
		
		TIM_SetCompare1( TIM3, (u16)(angle + 750) );
		
	}
	else if( index == SG_Pitch )
	{
		
		TIM_SetCompare2( TIM3,(u16)(angle + 250) );
		
	}
	
	
}















