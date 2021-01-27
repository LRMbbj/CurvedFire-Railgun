#include "ur.h"

void UR_Init(void)
{
	
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;

	RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART2, ENABLE );
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	
	GPIO_PinAFConfig( GPIOA, GPIO_PinSource2, GPIO_AF_USART2 ); //PA2复用为 USART2
	GPIO_PinAFConfig( GPIOA, GPIO_PinSource3, GPIO_AF_USART2 );//PA3复用为 USART2
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; //GPIOA2与 GPIOA3 
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; //速度 50MHz 
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init( GPIOA, &GPIO_InitStruct ); //初始化 PA9 PA10
	
	USART_InitStruct.USART_BaudRate = 9600; //波特率
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //硬件控制
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //收发使能
	USART_InitStruct.USART_Parity = USART_Parity_No; //奇偶校验
	USART_InitStruct.USART_StopBits = USART_StopBits_1; //停止位
	USART_InitStruct.USART_WordLength = USART_WordLength_8b; //字长
	
	USART_Init( USART2, &USART_InitStruct ); //初始化串口
	
	USART_Cmd( USART2, ENABLE );
	
}

u16 UR_Detect(void)
{
	
	u16 data = 0;

	USART_SendData( USART2, 0x55 );
	
	while( USART_GetITStatus( USART2, USART_IT_RXNE ) != RESET );
	
	data = USART_ReceiveData( USART2 ) << 8; //读取高八位
	
	while( USART_GetITStatus( USART2, USART_IT_RXNE ) != RESET );
	
	data += USART_ReceiveData( USART2 ); //读取低八位
	
	return data;

}
