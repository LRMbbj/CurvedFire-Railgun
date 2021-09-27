#include "openmv.h"

u8 SIZE = 0, DATA_NUM = 0;
u8 USART1_RX_BUF[6];
s8 POS_DATA;
bool isON = FALSE, STATE_O = FALSE;
extern s16 rAng;
s8 Random[50];

void OPENMV_Init(void)
{
	
	GPIO_InitTypeDef GPIO_InitStruct, GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	USART_InitTypeDef USART_InitStruct;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//使能USART1时钟
	RCC_AHB1PeriphClockCmd(	RCC_AHB1Periph_GPIOA, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7; //GPIOA7
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;//复用功能
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; //速度 50MHz 
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init( GPIOF, &GPIO_InitStruct ); //初始化 PA7
	
	GPIO_ResetBits (GPIOF, GPIO_Pin_7);//使能为否
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11; //GPIOA7
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;//复用功能
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; //速度 50MHz 
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init( GPIOF, &GPIO_InitStruct ); //初始化 PA7
	
	GPIO_ResetBits (GPIOF, GPIO_Pin_11);//使能为否
	
		//USART1端口配置
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10复用为USART1
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //GPIOA9与GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PA9，PA10
	
	USART_InitStruct.USART_BaudRate = 19200; //波特率
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //硬件控制
	USART_InitStruct.USART_Mode = USART_Mode_Rx; //收使能
	USART_InitStruct.USART_Parity = USART_Parity_No; //奇偶校验
	USART_InitStruct.USART_StopBits = USART_StopBits_1; //停止位
	USART_InitStruct.USART_WordLength = USART_WordLength_8b; //字长
	
	USART_Init( USART1, &USART_InitStruct ); //初始化串口
	
	USART_Cmd( USART1, ENABLE );
	
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启相关中断
	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//串口2中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	
}	//初始化OpenMV
void OPENMV_State(u8 state)
{
	
	if(state == DISABLE) 
	{
		GPIO_ResetBits (GPIOF, GPIO_Pin_7);//使能为否
		isON = FALSE;
	}
	if(state == ENABLE)
	{
		GPIO_SetBits (GPIOF, GPIO_Pin_7);//使能为是
		isON = TRUE;
		DATA_NUM = 0;
		POS_DATA = -122;
	}
	
}	//使能设置

s8 GetBias()
{
	return POS_DATA;
}

void USART1_IRQHandler(void)                	//串口1中断服务程序
{
	u8 Res;
	
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		Res =USART_ReceiveData(USART1);//(USART1->DR);	//读取接收到的数据
		
		if( SIZE < 3 )
		{
			USART1_RX_BUF[SIZE] = Res;
			SIZE++;
			STATE_O = TRUE;
		}
		else if( SIZE == 3 &&  Res == '\n' && USART1_RX_BUF[2] == '\r' )
		{
			STATE_O = FALSE;
			SIZE = 0;
			if(isON == TRUE) 
			{
				POS_DATA = USART1_RX_BUF[0] + USART1_RX_BUF[1]*256-5000;
				USART1_RX_BUF[0] = 0;
				USART1_RX_BUF[1] = 0;
				Random[DATA_NUM] = rAng;
				DATA_NUM++;
			}
		}
		else
		{
			SIZE = 0;
			STATE_O = FALSE;
		}
  }  											 
}
