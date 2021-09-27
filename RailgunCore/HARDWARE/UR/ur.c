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
	
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启相关中断
	
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;//串口2中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	
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
	if(USART_GetITStatus(USART2,USART_IT_RXNE) != RESET) //中断产生 
 	{
		USART_ClearITPendingBit(USART2,USART_IT_RXNE); //清除中断标志
		if(FLAG == 0)
		{
			USART2_RX_BUF_H = USART_ReceiveData(USART2);     //接收串口2数据到buff缓冲区
			FLAG = 1;
		}
		else if(FLAG == 1)
		{
			USART2_RX_BUF_L = USART_ReceiveData(USART2);
			FLAG = 2;
		}
	}
}
