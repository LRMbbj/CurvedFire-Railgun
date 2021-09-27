#ifndef __OPENMV__
#define __OPENMV__


#include "delay.h"

typedef enum {FALSE = 0, TRUE = 1} bool;
s8 GetBias(void);
void OPENMV_Init(void); //初始化OpenMV
void OPENMV_State(u8 state); //使能设置


#endif
