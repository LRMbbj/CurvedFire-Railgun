#ifndef __HMI__
#define __HMI__


#include "sys.h"

#define USART_REC_LEN  			200  	//�����������ֽ��� 200
#define EN_USART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u8 DATA_SIZE;         		//����״̬���	

u8 PROG_STATE = 0;

void HMI_Init(void); //��ʼ��HMI
u8 GetProgState(void);

#endif