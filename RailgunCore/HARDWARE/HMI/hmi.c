#include "hmi.h"

u8 DATA_SIZE = 0;	
u8 STATE = 0;

void HMI_Init(void)
{
	
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;

	RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART3, ENABLE );
	RCC_AHB1PeriphClockCmd(	RCC_AHB1Periph_GPIOB, ENABLE);
	
	GPIO_PinAFConfig( GPIOB, GPIO_PinSource10, GPIO_AF_USART3 ); //PA2复用为 USART2
	GPIO_PinAFConfig( GPIOB, GPIO_PinSource11, GPIO_AF_USART3 );//PA3复用为 USART2
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; //GPIOA2与 GPIOA3 
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; //速度 50MHz 
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init( GPIOB, &GPIO_InitStruct ); //初始化 PA9 PA10
	
	USART_InitStruct.USART_BaudRate = 9600; //波特率
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //硬件控制
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //收发使能
	USART_InitStruct.USART_Parity = USART_Parity_No; //奇偶校验
	USART_InitStruct.USART_StopBits = USART_StopBits_1; //停止位
	USART_InitStruct.USART_WordLength = USART_WordLength_8b; //字长
	
	USART_Init( USART3, &USART_InitStruct ); //初始化串口
	
	USART_Cmd( USART3, ENABLE );
	STATE = 0;
	
	
	#if EN_USART3_RX	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启相关中断

	//Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、

	#endif
	
}

void USART3_puts(char *str)
{
	u8 j=0;
	u16 i=0;
	while (*(str+j))
	{
		USART_SendData(USART3,*(str+j));
		j++;
		for(i=0;i<10000;i++);
	}
}

void HMISendOrder(char *obj,char *val)
{
	
	USART3_puts(obj);
	USART3_puts("=\"");
	USART3_puts(val);
	USART3_puts("\"\xff\xff\xff");
	
}

void HMISetValue(char *obj,char *val)
{
	
	USART3_puts(obj);
	USART3_puts(" ");
	USART3_puts(val);
	USART3_puts("\xff\xff\xff");
	
}


u32 ReadInt(void)
{
	
	while( DATA_SIZE == 4 && STATE == 0 );
	return USART_RX_BUF[0] + 
				256 * USART_RX_BUF[1] + 
				65536 * USART_RX_BUF[2] + 
				16777216 * USART_RX_BUF[3];
	
}

void ClearBUF()
{
	while( DATA_SIZE > 0 )
	{
		DATA_SIZE--;
		USART_RX_BUF[ DATA_SIZE ] = 0;
	}
	
}

void USART3_IRQHandler(void)                	//串口3中断服务程序
{
	u8 Res;
	
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		Res =USART_ReceiveData(USART3);//(USART1->DR);	//读取接收到的数据
		
		if( (Res == 0x70 || Res == 0x71) && STATE == 0) //是否开始接收
		{
			STATE = Res - 0x70 + 1; //str->1,int->2
			DATA_SIZE = 0;
		}
		else if(STATE == 1) //是否正在接收str
		{
			if( DATA_SIZE > 1 && USART_RX_BUF[ DATA_SIZE - 1 ] == 0xff && USART_RX_BUF[ DATA_SIZE - 2 ] == 0xff && Res == 0xff) //是否收到结束位
			{
				DATA_SIZE -= 2;
				STATE = 0; 
			}//接收完成
			else //未收到结束位
			{
				USART_RX_BUF[ DATA_SIZE ] = Res;
				DATA_SIZE++;
			}
		}
		else if(STATE == 2) //是否正在接收int
		{
			if( DATA_SIZE == 6 && USART_RX_BUF[ DATA_SIZE - 1 ] == 0xff && USART_RX_BUF[ DATA_SIZE - 2 ] == 0xff && Res == 0xff) //是否收到结束位
			{
				DATA_SIZE -= 2;
				STATE = 0; 
			}//接收完成
			else //未收到结束位
			{
				USART_RX_BUF[ DATA_SIZE ] = Res;
				DATA_SIZE++;
			}
		}
		else //接受无效
		{
			STATE = 0;
		}
  }  											 
}
