#include "hmi.h"

void HMI_Init(void)
{
	
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;

	RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART3, ENABLE );
	RCC_AHB1PeriphClockCmd(	RCC_AHB1Periph_GPIOB, ENABLE);
	
	GPIO_PinAFConfig( GPIOB, GPIO_PinSource10, GPIO_AF_USART3 ); //PA2����Ϊ USART2
	GPIO_PinAFConfig( GPIOB, GPIO_PinSource11, GPIO_AF_USART3 );//PA3����Ϊ USART2
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; //GPIOA2�� GPIOA3 
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; //�ٶ� 50MHz 
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init( GPIOB, &GPIO_InitStruct ); //��ʼ�� PA9 PA10
	
	USART_InitStruct.USART_BaudRate = 9600; //������
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //Ӳ������
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //�շ�ʹ��
	USART_InitStruct.USART_Parity = USART_Parity_No; //��żУ��
	USART_InitStruct.USART_StopBits = USART_StopBits_1; //ֹͣλ
	USART_InitStruct.USART_WordLength = USART_WordLength_8b; //�ֳ�
	
	USART_Init( USART3, &USART_InitStruct ); //��ʼ������
	
	USART_Cmd( USART3, ENABLE );
	
	
	#if EN_USART3_RX	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//��������ж�

	//Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����

	#endif
	
}

u8 GetProgState(void)
{

	return PROG_STATE;
	
}

u8 STATE = 0;


void USART3_IRQHandler(void)                	//����3�жϷ������
{
	u8 Res;
	
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		Res =USART_ReceiveData(USART3);//(USART1->DR);	//��ȡ���յ�������
		
		if( (Res == 0x70 || Res == 0x71) && STATE == 0) //�Ƿ�ʼ����
		{
			STATE = 1;
			DATA_SIZE = 0;
		}
		else if(STATE == 1) //�Ƿ����ڽ���
		{
			if( DATA_SIZE > 1 && USART_RX_BUF[ DATA_SIZE - 1 ] == 0xff && USART_RX_BUF[ DATA_SIZE - 2 ] == 0xff && Res == 0xff) //�Ƿ��յ�����λ
			{
				DATA_SIZE -= 2;
				STATE = 0; 
			}//�������
			else //δ�յ�����λ
			{
				USART_RX_BUF[ DATA_SIZE ] = Res;
				DATA_SIZE++;
			}
		}
		else //������Ч
		{
			STATE = 0;
		}
  }  											 
}
