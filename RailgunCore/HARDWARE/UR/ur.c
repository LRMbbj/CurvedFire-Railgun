#include "ur.h"
#include "delay.h"

u8 USART2_RX_BUF_H, USART2_RX_BUF_L, FLAG;

void UR_Init(void)
{
	
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART2, ENABLE );
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	
	GPIO_PinAFConfig( GPIOA, GPIO_PinSource2, GPIO_AF_USART2 ); //PA2����Ϊ USART2
	GPIO_PinAFConfig( GPIOA, GPIO_PinSource3, GPIO_AF_USART2 );//PA3����Ϊ USART2
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; //GPIOA2�� GPIOA3 
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; //�ٶ� 50MHz 
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init( GPIOA, &GPIO_InitStruct ); //��ʼ�� PA9 PA10
	
	USART_InitStruct.USART_BaudRate = 9600; //������
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //Ӳ������
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //�շ�ʹ��
	USART_InitStruct.USART_Parity = USART_Parity_No; //��żУ��
	USART_InitStruct.USART_StopBits = USART_StopBits_1; //ֹͣλ
	USART_InitStruct.USART_WordLength = USART_WordLength_8b; //�ֳ�
	
	USART_Init( USART2, &USART_InitStruct ); //��ʼ������
	
	USART_Cmd( USART2, ENABLE );
	
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//��������ж�
	
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;//����2�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
}

u16 UR_Detect(void)
{
	u16 i = 0;
	FLAG = 0;
	USART2_RX_BUF_H = 0;
	USART2_RX_BUF_L = 0;

	USART_SendData( USART2, 0x55 );
	
	delay_ms(250);

	i = USART2_RX_BUF_H * 256 + USART2_RX_BUF_L;
	if(i>10000) i = 0;
	return i;

}

void USART2_IRQHandler() 
{
	if(USART_GetITStatus(USART2,USART_IT_RXNE) != RESET) //�жϲ��� 
 	{
		USART_ClearITPendingBit(USART2,USART_IT_RXNE); //����жϱ�־
		if(FLAG == 0)
		{
			USART2_RX_BUF_H = USART_ReceiveData(USART2);     //���մ���2���ݵ�buff������
			FLAG = 1;
		}
		else if(FLAG == 1)
		{
			USART2_RX_BUF_L = USART_ReceiveData(USART2);
			FLAG = 2;
		}
	}
}
