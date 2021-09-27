#ifndef __OPENMV__
#define __OPENMV__


#include "delay.h"

typedef enum {FALSE = 0, TRUE = 1} bool;
s8 GetBias(void);
void OPENMV_Init(void); //��ʼ��OpenMV
void OPENMV_State(u8 state); //ʹ������


#endif
