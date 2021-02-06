#ifndef __HMI__
#define __HMI__


#include "sys.h"


#define USART_REC_LEN  			200  	//定义最大接收字节数 200
#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收
	  	
extern u8 USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u8 DATA_SIZE;         		//接收大小	
extern u8 STATE; //接收状态

void HMI_Init(void); //初始化HMI
u8 GetProgState(void);
u32 ReadInt(void);
void ClearBUF(void);
void HMISendOrder( char*, char*);
void HMISetValue( char*, char*);

#endif
