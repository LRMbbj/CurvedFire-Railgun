#include "delay.h"
#include "pwm.h"
#include "ur.h"
#include "hmi.h"
#include "railgun.h"
#include "openmv.h"

char str[5];
s16 rAng,pAng;

u8 IsEq( u8 *pBUF, char *s ) //�ж��Ƿ���ȣ�ʹ��ͨ���'*'
{
	u8 i;
	for(i=0;i<DATA_SIZE;i++)
		if( *(pBUF+i) != *(s+i) && *(s+i) != '*' ) return 0;
	if(*(s+DATA_SIZE) != '\0') return 0;
	return 1;
}

char *int2str(u16 value)//����ת�ַ���
{

	u16 val = value;
	u16 i = 1000,j = 0;
	for (j = 0; j < 5; j++)
	{
		str[j] = '\0';
	}
	j = 0;
	while (j < 4 && i > 0)
	{
		str[j] = val / i + 48;
		val %= i;
		j++;
		i /= 10;
	}
	
	return (char*)str;

}


void BaseAndTest()
{
	u16 dis = 0;
	ClearBUF();//��λHMI����
	while( !IsEq( USART3_RX_BUF, "DT2" ) );
	ClearBUF();
	rAng = ReadInt() % 10000; // �����λ
	pAng = ReadInt() % 10000;
	
	HMISetValue("t2.txt",int2str(rAng));
	HMISetValue("t3.txt",int2str(pAng));
	
	PWM_SetAngle(SG_Rotate, rAng );
	PWM_SetAngle(SG_Pitch, pAng );
	
	Railgun_Fire();//���䣡
	
	PWM_SetAngle(SG_Rotate, 0 );
	PWM_SetAngle(SG_Pitch, 0 );//��λ
	
	delay_ms(500);
	
	Railgun_Fire();
	
	while( !IsEq( USART3_RX_BUF, "OD0" )  )
	{
		dis = 0;
		dis = UR_Detect();
		if( dis != 0 ) HMISetValue("t5.txt",int2str( dis ));
		delay_ms(100);
	}
	
	return;
	
}

void ExtentA()
{
	u16 dis;
	s8 tt =0;
	ClearBUF();//��λHMI����
	
//	rAng = -222;
//	PWM_SetAngle(SG_Rotate, rAng );//��λ����ڵ�R=-30 P=0
	
	OPENMV_State(ENABLE);//���Ϳ�ʼָ���OpenMV
	
//	for(rAng = -222;rAng <= 221;rAng+=6)//��ѭ��1��ɨ������Ŀ�����򣩵���ƫת��
//	{
//			PWM_SetAngle(SG_Rotate, rAng );
//			delay_ms(40);//�ӳ�һ��ʱ��
//	}//�˴��˳�ѭ��
//	for(rAng = 221;rAng >= -222;rAng-=6)//��ѭ��1��ɨ������Ŀ�����򣩵���ƫת��
//	{
//			PWM_SetAngle(SG_Rotate, rAng );
//			delay_ms(40);//�ӳ�һ��ʱ��
//	}//�˴��˳�ѭ��
	
	rAng = 0;
	PWM_SetAngle(SG_Rotate, rAng );
	delay_ms(500);
	while(GetBias()>3||GetBias()<-3)
	{
		tt=GetBias();
		if(tt<-120) continue;
		rAng += GetBias()/10%20;
		PWM_SetAngle(SG_Rotate, rAng );
		delay_ms(50);
		tt=GetBias();
	}
	HMISetValue("t3.txt",int2str(rAng));
	OPENMV_State(DISABLE);//���ͽ���ָ���OpenMV
	
	//PWM_SetAngle(SG_Rotate, rAng );//������Ŀ��λ��
	dis = UR_Detect();//�򿪲�ಢ��������
	
	pAng = 100;//!!!ȱʧ ���㷢������
	PWM_SetAngle(SG_Pitch, pAng );
	
	Railgun_Fire();//����
	//PWM_SetAngle(SG_Rotate, 0 );
	PWM_SetAngle(SG_Pitch, 0 );//��λ
	
	OPENMV_State(ENABLE);//���Ϳ�ʼָ���OpenMV
	while( !IsEq( USART3_RX_BUF, "OD0" )  );
	OPENMV_State(DISABLE);//���ͽ���ʼָ���OpenMV
	
	return;
	
}

void ExtentB()
{
	s16 target;
	ClearBUF();
	
	rAng = -222;
	pAng = 0;
	PWM_SetAngle(SG_Rotate, rAng );
	PWM_SetAngle(SG_Pitch, pAng ); //��λ����ڵ�R=-30 P=0
	OPENMV_State(ENABLE); //���Ϳ�ʼָ���OpenMV
	
	//ѭ��1 -30->30->-30
	for(rAng = -222;rAng <= 221;rAng+=3)//��ѭ��1��ɨ������Ŀ�����򣩵���ƫת��
	{
			PWM_SetAngle(SG_Rotate, rAng );
			delay_ms(30);//�ӳ�һ��ʱ��
	}//�˴��˳�ѭ��
	for(rAng = 221;rAng >= -222;rAng-=3)//��ѭ��1��ɨ������Ŀ�����򣩵���ƫת��
	{
			PWM_SetAngle(SG_Rotate, rAng );
			delay_ms(20);//�ӳ�һ��ʱ��
	}//�˴��˳�ѭ��

	target = GetBias();//����Ŀ��ǶȺ���
	OPENMV_State(DISABLE);//���ͽ���ָ���OpenMV	
	
	for(rAng = -222;rAng < target;rAng+=3)//��ѭ��1��ɨ������Ŀ�����򣩵���ƫת��
	{
			PWM_SetAngle(SG_Rotate, rAng );
			delay_ms(20);//�ӳ�һ��ʱ��
	}//�˴��˳�ѭ��
	
	PWM_SetAngle(SG_Rotate, target );
	
	GPIO_SetBits(GPIOF,GPIO_Pin_10);//�ֶ�����
	
	for(rAng = target;rAng <= 221;rAng+=3)//��ѭ��1��ɨ������Ŀ�����򣩵���ƫת��
	{
			PWM_SetAngle(SG_Rotate, rAng );
			delay_ms(20);//�ӳ�һ��ʱ��
	}//�˴��˳�ѭ��

	for(rAng = 221;rAng >= -222;rAng-=3)//��ѭ����ɨ������Ŀ�����򣩵���ƫת��
	{
			PWM_SetAngle(SG_Rotate, rAng );//������Ŀ��λ��
			delay_ms(20);//�ӳ�һ��ʱ��
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
	Railgun_Init();
	UR_Init();
	PWM_Init();
	HMI_Init();
	OPENMV_Init();
	
	delay_ms(1000);
	
	rAng = 0;
	pAng = 0;
	PWM_SetAngle(SG_Rotate, 0 );
	PWM_SetAngle(SG_Pitch, 0 );//��λ
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
