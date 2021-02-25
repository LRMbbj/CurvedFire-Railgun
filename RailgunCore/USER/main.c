#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "pwm.h"
#include "ur.h"
#include "hmi.h"
#include "railgun.h"
#include "openmv.h"


s16 rAng,pAng;

u8 IsEq( u8 *pBUF, char *s ) //�ж��Ƿ���ȣ�ʹ��ͨ���'*'
{
	u8 i;
	for(i=0;i<DATA_SIZE;i++)
		if( *(pBUF+i) != *(s+i) && *(s+i) != '*' ) return 0;
	if(*(s+DATA_SIZE) != '\0') return 0;
	return 1;
}

char *int2str(int value, int radix)//����ת�ַ���
{
		char		*string = "";
    int     i, d;
    int     flag = 0;
    char    *ptr = string;
 
    /* This implementation only works for decimal numbers. */
    if (radix != 10)
    {
        *ptr = 0;
        return string;
    }
 
    if (!value)
    {
        *ptr++ = 0x30;
        *ptr = 0;
        return string;
    }
 
    /* if this is a negative value insert the minus sign. */
    if (value < 0)
    {
        *ptr++ = '-';
 
        /* Make the value positive. */
        value *= -1;
    }
 
    for (i = 10000; i > 0; i /= 10)
    {
        d = value / i;
 
        if (d || flag)
        {
            *ptr++ = (char)(d + 0x30);
            value -= (d * i);
            flag = 1;
        }
    }
 
    /* Null terminate the string. */
    *ptr = 0;
 
    return string;
 
}


void BaseAndTest()
{
	ClearBUF();//��λHMI����
	while( !IsEq( USART3_RX_BUF, "DT2" ) );
	rAng = ReadInt() % 10000; // �����λ
	pAng = ReadInt() % 10000;
	
	HMISetValue("t2.txt",int2str(rAng,10));
	HMISetValue("t3.txt",int2str(pAng,10));
	
	PWM_SetAngle(SG_Rotate, rAng );
	PWM_SetAngle(SG_Pitch, pAng );
	
	Railgun_Fire();//���䣡
	
	PWM_SetAngle(SG_Rotate, 0 );
	PWM_SetAngle(SG_Pitch, 0 );//��λ
	
	delay_ms(500);
	
	Railgun_Fire();
	
	while( !IsEq( USART3_RX_BUF, "OD0" )  )
	{
		HMISetValue("t5.txt",int2str( UR_Detect() ,10));
		delay_ms(100);
	}
	
	return;
	
}

void ExtentA()
{
	u16 dis;
	ClearBUF();//��λHMI����
	
	rAng = -167;
	PWM_SetAngle(SG_Rotate, rAng );//��λ����ڵ�R=-30 P=0
	OPENMV_State(ENABLE);//���Ϳ�ʼָ���OpenMV
	for(rAng = -167;rAng <= 167;rAng++)//��ѭ����ɨ������Ŀ�����򣩵���ƫת��
	{
			PWM_SetAngle(SG_Rotate, rAng );
			delay_ms(9);//�ӳ�һ��ʱ��
	}//�˴��˳�ѭ��
	rAng = 0;// !!!ȱʧ ����Ŀ��ǶȺ���
	
	PWM_SetAngle(SG_Rotate, rAng );//������Ŀ��λ��
	dis = UR_Detect();//�򿪲�ಢ��������
	
	pAng = dis;//!!!ȱʧ ���㷢������
	PWM_SetAngle(SG_Pitch, pAng );
	
	Railgun_Fire();//����
	PWM_SetAngle(SG_Rotate, 0 );
	PWM_SetAngle(SG_Pitch, 0 );//��λ
	
	return;
	
}

void ExtentB()
{
	s16 target;
	ClearBUF();
	
	rAng = -167;
	pAng = 0;
	PWM_SetAngle(SG_Rotate, rAng );
	PWM_SetAngle(SG_Pitch, pAng ); //��λ����ڵ�R=-30 P=0
	OPENMV_State(ENABLE); //���Ϳ�ʼָ���OpenMV
	
	//ѭ��1 -30->30->-30
	for(rAng = -167;rAng <= 167;rAng++)//��ѭ����ɨ������Ŀ�����򣩵���ƫת��
	{
			PWM_SetAngle(SG_Rotate, rAng );//������Ŀ��λ��
			delay_ms(9);//�ӳ�һ��ʱ��
	}
	for(rAng = 167;rAng >= -167;rAng--)//��ѭ����ɨ������Ŀ�����򣩵���ƫת��
	{
			PWM_SetAngle(SG_Rotate, rAng );//������Ŀ��λ��
			delay_ms(9);//�ӳ�һ��ʱ��
	}
	
	target = 0;//!!ȱ�� �ж�Ŀ��λ
	
	
	for(rAng = -167;rAng <= target;rAng++)//��ѭ����ɨ������Ŀ�����򣩵���ƫת��
	{
			PWM_SetAngle(SG_Rotate, rAng );//������Ŀ��λ��
			delay_ms(9);//�ӳ�һ��ʱ��
	}//ѭ��2 -30->Ŀ��λ
	
	GPIO_SetBits(GPIOF,GPIO_Pin_10);//�ֶ�����
	
	for(rAng = target;rAng <= 167;rAng++)//��ѭ����ɨ������Ŀ�����򣩵���ƫת��
	{
			PWM_SetAngle(SG_Rotate, rAng );//������Ŀ��λ��
			delay_ms(9);//�ӳ�һ��ʱ��
	}
	for(rAng = 167;rAng >= 0;rAng--)//��ѭ����ɨ������Ŀ�����򣩵���ƫת��
	{
			PWM_SetAngle(SG_Rotate, rAng );//������Ŀ��λ��
			delay_ms(9);//�ӳ�һ��ʱ��
	}//ѭ��3 Ŀ��λ->30->0
	
	
	rAng = 0;
	pAng = 0;
	PWM_SetAngle(SG_Rotate, rAng );
	PWM_SetAngle(SG_Pitch, pAng ); //˫�㸴λ
	
	
	GPIO_ResetBits(GPIOF,GPIO_Pin_10);//��λ����
	return;
	
}

int main(void)
{
	//Init
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ����� 2
	
	delay_init(168);
	uart_init(19200);
	Railgun_Init();
	UR_Init();
	PWM_Init();
	HMI_Init();
	
	rAng = 0;
	pAng = 0;
	PWM_SetAngle(SG_Rotate, 0 );
	PWM_SetAngle(SG_Pitch, 0 );//��λ
	
	rAng = 0;
	while(1)
	{
		PWM_SetAngle(SG_Rotate, rAng );//������Ŀ��λ��
		delay_ms(9);//�ӳ�һ��ʱ��
		rAng++;
		rAng %= 167;
		//�˴��˳�ѭ��
	}
	
	while(1)
	{
		if(USART3_RX_BUF[0] == 'O' && USART3_RX_BUF[1] == 'D') //ODx��ת����ͬģʽ
			switch(USART3_RX_BUF[2])
			{	
				case '1':BaseAndTest();break;
				case '2':ExtentA();break;
				case '3':ExtentB();break;
			}
		delay_ms(10);
	}
	
	//return 0;
	
}
