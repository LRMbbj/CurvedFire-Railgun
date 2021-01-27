#include "ur.h"

void UR_Init(void)
{
	
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;

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
	
}

u16 UR_Detect(void)
{
	
	u16 data = 0;

	USART_SendData( USART2, 0x55 );
	
	while( USART_GetITStatus( USART2, USART_IT_RXNE ) != RESET );
	
	data = USART_ReceiveData( USART2 ) << 8; //��ȡ�߰�λ
	
	while( USART_GetITStatus( USART2, USART_IT_RXNE ) != RESET );
	
	data += USART_ReceiveData( USART2 ); //��ȡ�Ͱ�λ
	
	return data;

}
