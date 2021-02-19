#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "pwm.h"
#include "ur.h"
#include "hmi.h"
#include "railgun.h"

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
	u8 rAng,pAng;
	while( !IsEq( USART_RX_BUF, "DT2" ) );
	rAng = ReadInt() % 10000;
	pAng = ReadInt() % 10000;
	
	HMISetValue("t2.txt",int2str(rAng,10));
	HMISetValue("t3.txt",int2str(pAng,10));
	
	PWM_SetAngle(SG_Rotate, rAng+90 );
	PWM_SetAngle(SG_Pitch, pAng );
	
	Railgun_Fire();//���䣡
	
	PWM_SetAngle(SG_Rotate, 90 );
	PWM_SetAngle(SG_Pitch, 0 );//��λ
	
	delay_ms(500);
	
	Railgun_Fire();
	
	while( !IsEq( USART_RX_BUF, "OD0" )  )
	{
		HMISetValue("t5.txt",int2str( UR_Detect() ,10));
		delay_ms(100);
	}
	
	return;
	
}

void ExtentA()
{
	
	USART_RX_BUF[0] = 0;//��λHMI����
	
	//��λ����ڵ�R=-30 P=0
	//���Ϳ�ʼָ���OpenMV
	//��ѭ��������ʱ������
			//����ƫת��
			//�ӳ�һ��ʱ��
	//�˴��˳�ѭ��
	//�򿪲�ಢ��������
	//���������뷢�亯��
	
	return;
	
}

void ExtentB()
{
	
	USART_RX_BUF[0] = 0;
	
	//��λ����ڵ�R=-30 P=0
	//���Ϳ�ʼָ���OpenMV
	//ѭ��1������ʱ������
			//����ƫת��
			//�ӳ�һ��ʱ��
	//�˴��˳�ѭ��1
	//�򿪲�ಢ��������
	//ѭ��2������ʱ������
			//����ƫת��
			//�ӳ�һ��ʱ��
	//�˴��˳�ѭ��2
	//�������뷢�亯��(��̬)
	//ѭ��3�����յ�����ָ��������
			//����ƫת��
			//�ӳ�һ��ʱ��
	//˫�㸴λ
	
	return;
	
}

int main(void)
{
	//Init
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ����� 2
	
	delay_init(168);
	uart_init(115200);
	Railgun_Init();
	UR_Init();
	PWM_Init();
	HMI_Init();
	
	while(1)
	{
		if(USART_RX_BUF[0] == 'O' && USART_RX_BUF[1] == 'D') //ODx��ת����ͬģʽ
			switch(USART_RX_BUF[2])
			{	
				case '1':BaseAndTest();break;
				case '2':ExtentA();break;
				case '3':ExtentB();break;
			}
		delay_ms(10);
	}
	
	//return 0;
	
}
