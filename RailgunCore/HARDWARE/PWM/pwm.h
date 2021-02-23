#ifndef __PWM__
#define __PWM__


#include "sys.h"

#define SG_Rotate ((u8)0x00)
#define SG_Pitch  ((u8)0x01)
#define IS_SG_CODE(CODE) (((CODE) == SG_Rotate) || \
                                       ((STATE) == SG_Pitch))

void PWM_Init(void); //初始化PWM和舵机
void PWM_SetAngle(u8 index,s16 angle); //设置舵机角度


#endif
