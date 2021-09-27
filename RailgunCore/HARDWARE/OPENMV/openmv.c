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

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//ʹ��USART1ʱ��
	RCC_AHB1PeriphClockCmd(	RCC_AHB1Periph_GPIOA, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7; //GPIOA7
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;//���ù���
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; //�ٶ� 50MHz 
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init( GPIOF, &GPIO_InitStruct ); //��ʼ�� PA7
	
	GPIO_ResetBits (GPIOF, GPIO_Pin_7);//ʹ��Ϊ��
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11; //GPIOA7
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;//���ù���
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; //�ٶ� 50MHz 
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init( GPIOF, &GPIO_InitStruct ); //��ʼ�� PA7
	
	GPIO_ResetBits (GPIOF, GPIO_Pin_11);//ʹ��Ϊ��
	
		//USART1�˿�����
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10����ΪUSART1
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //GPIOA9��GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ��PA9��PA10
	
	USART_InitStruct.USART_BaudRate = 19200; //������
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //Ӳ������
	USART_InitStruct.USART_Mode = USART_Mode_Rx; //��ʹ��
	USART_InitStruct.USART_Parity = USART_Parity_No; //��żУ��
	USART_InitStruct.USART_StopBits = USART_StopBits_1; //ֹͣλ
	USART_InitStruct.USART_WordLength = USART_WordLength_8b; //�ֳ�
	
	USART_Init( USART1, &USART_InitStruct ); //��ʼ������
	
	USART_Cmd( USART1, ENABLE );
	
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//��������ж�
	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//����2�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
}	//��ʼ��OpenMV
void OPENMV_State(u8 state)
{
	
	if(state == DISABLE) 
	{
		GPIO_ResetBits (GPIOF, GPIO_Pin_7);//ʹ��Ϊ��
		isON = FALSE;
	}
	if(state == ENABLE)
	{
		GPIO_SetBits (GPIOF, GPIO_Pin_7);//ʹ��Ϊ��
		isON = TRUE;
		DATA_NUM = 0;
		POS_DATA = -122;
	}
	
}	//ʹ������

s8 GetBias()
{
	return POS_DATA;
}

void USART1_IRQHandler(void)                	//����1�жϷ������
{
	u8 Res;
	
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		Res =USART_ReceiveData(USART1);//(USART1->DR);	//��ȡ���յ�������
		
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
