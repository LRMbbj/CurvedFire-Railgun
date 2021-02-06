#ifndef __HMI__
#define __HMI__


#include "sys.h"


#define USART_REC_LEN  			200  	//�����������ֽ��� 200
#define EN_USART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
	  	
extern u8 USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u8 DATA_SIZE;         		//���մ�С	
extern u8 STATE; //����״̬

void HMI_Init(void); //��ʼ��HMI
u8 GetProgState(void);
u32 ReadInt(void);
void ClearBUF(void);
void HMISendOrder( char*, char*);
void HMISetValue( char*, char*);

#endif
