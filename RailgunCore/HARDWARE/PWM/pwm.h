#ifndef __PWM__
#define __PWM__


#include "sys.h"

#define SG_Rotate ((u8)0x00)
#define SG_Pitch  ((u8)0x01)
#define IS_SG_CODE(CODE) (((CODE) == SG_Rotate) || \
                                       ((STATE) == SG_Pitch))

void PWM_Init(void); //��ʼ��PWM�Ͷ��
void PWM_SetAngle(u8 index,s16 angle); //���ö���Ƕ�


#endif
