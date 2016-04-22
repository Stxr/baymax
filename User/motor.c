//���������˵���Լ���ˢ�ͱ�ˢ
//IO:PA0 PA1 PA2 PA3 �ֱ��ӦTIM2 ��ch1 ch2 ch3 ch4  ch1��ch2���Ƶ�� ch3��ch4����ˢ��
//   PA4 PA5 PA6 PA7 ���ƻ����˵������ת
#include "motor.h"
void Motor_InitConfig(void)
{
	//�ṹ�嶨��
	GPIO_InitTypeDef GPIO_InitStruction;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStruction;  //��ʼ��TIM
	TIM_OCInitTypeDef TIM_OCInitTypeStruction;   //TIM���ģʽ����
	//GPIO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);  //����GPIOA��ʱ�Ӻ͹ܽŵĸ��ù���
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);  	//����TIM2��ʱ��
	GPIO_InitStruction.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;  //����pwm��
	GPIO_InitStruction.GPIO_Mode=GPIO_Mode_AF_PP;                            //�����������
	GPIO_InitStruction.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruction);

	GPIO_InitStruction.GPIO_Pin=GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;   //���Ƶ������ת��
	GPIO_InitStruction.GPIO_Mode=GPIO_Mode_Out_PP;                            //�������
	GPIO_InitStruction.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruction);

	//TIM2
	TIM_TimeBaseStruction.TIM_Period=1000-1; //��ʱ����0������999��Ϊһ����ʱ����
	TIM_TimeBaseStruction.TIM_Prescaler=0;   //����Ԥ��Ƶ������Ƶ  72MHz
	TIM_TimeBaseStruction.TIM_ClockDivision=TIM_CKD_DIV1;  //����ʱ�ӷ�Ƶ������Ƶ
	TIM_TimeBaseStruction.TIM_CounterMode=TIM_CounterMode_Up;  //���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStruction);  //����TIM_CCRԤװ��ʹ��
	//TIM2 CH1
	TIM_OCInitTypeStruction.TIM_OCMode=TIM_OCMode_PWM1;  //����pwm��ģʽ��ģʽ1��������ǰ����ʱΪ��Ч��ƽ
	TIM_OCInitTypeStruction.TIM_OutputState=TIM_OutputState_Enable; //���ʹ��
	TIM_OCInitTypeStruction.TIM_OCPolarity=TIM_OCPolarity_High;  //��ЧΪ�ߵ�ƽ
	TIM_OC1Init(TIM2,&TIM_OCInitTypeStruction);  //TIM2ͨ��1��ʼ��
	TIM_OC1PreloadConfig(TIM2,TIM_OCPreload_Enable);  //����TIM_CCRԤװ��ʹ��
	//TIM2 CH2
	TIM_OCInitTypeStruction.TIM_OCMode=TIM_OCMode_PWM1;  //����pwm��ģʽ��ģʽ1��������ǰ����ʱΪ��Ч��ƽ
	TIM_OCInitTypeStruction.TIM_OutputState=TIM_OutputState_Enable; //���ʹ��
	TIM_OCInitTypeStruction.TIM_OCPolarity=TIM_OCPolarity_High;  //��ЧΪ�ߵ�ƽ
	TIM_OC2Init(TIM2,&TIM_OCInitTypeStruction);  //TIM2ͨ��1��ʼ��
	TIM_OC2PreloadConfig(TIM2,TIM_OCPreload_Enable);  //����TIM_CCRԤװ��ʹ��
	//TIM2 CH3
	TIM_OCInitTypeStruction.TIM_OCMode=TIM_OCMode_PWM1;  //����pwm��ģʽ��ģʽ1��������ǰ����ʱΪ��Ч��ƽ
	TIM_OCInitTypeStruction.TIM_OutputState=TIM_OutputState_Enable; //���ʹ��
	TIM_OCInitTypeStruction.TIM_OCPolarity=TIM_OCPolarity_High;  //��ЧΪ�ߵ�ƽ
	TIM_OC3Init(TIM2,&TIM_OCInitTypeStruction);  //TIM2ͨ��1��ʼ��
	TIM_OC3PreloadConfig(TIM2,TIM_OCPreload_Enable);  //����TIM_CCRԤװ��ʹ��
	//TIM2 CH4
	TIM_OCInitTypeStruction.TIM_OCMode=TIM_OCMode_PWM1;  //����pwm��ģʽ��ģʽ1��������ǰ����ʱΪ��Ч��ƽ
	TIM_OCInitTypeStruction.TIM_OutputState=TIM_OutputState_Enable; //���ʹ��
	TIM_OCInitTypeStruction.TIM_OCPolarity=TIM_OCPolarity_High;  //��ЧΪ�ߵ�ƽ
	TIM_OC4Init(TIM2,&TIM_OCInitTypeStruction);  //TIM2ͨ��1��ʼ��
	TIM_OC4PreloadConfig(TIM2,TIM_OCPreload_Enable);  //����TIM_CCRԤװ��ʹ��

	TIM_Cmd(TIM2,ENABLE);  //ʹ�ܶ�ʱ��2
}
void move(int a,int b)  //���Ƶ��
{
	if (a>=0&&b>=0) //�����ת
		{
			GPIO_SetBits(GPIOA,GPIO_Pin_4);  //INT1 1
			GPIO_ResetBits(GPIOA,GPIO_Pin_5);//INT2 0
			GPIO_SetBits(GPIOA,GPIO_Pin_6);  //INT3 1
			GPIO_ResetBits(GPIOA,GPIO_Pin_7);//INT4 0
		}
	else if(a>=0&&b<0)//�ұ߷�ת
		{
			b=-b;
			GPIO_SetBits(GPIOA,GPIO_Pin_4);//INT1 1
			GPIO_ResetBits(GPIOA,GPIO_Pin_5);//INT2 0
			GPIO_SetBits(GPIOA,GPIO_Pin_7);  //INT4 1
			GPIO_ResetBits(GPIOA,GPIO_Pin_6);//INT3 0
		}
	else if(a<0&&b>=0)//��߷�ת
		{
			a=-a;
			GPIO_SetBits(GPIOA,GPIO_Pin_5);//INT2 1
			GPIO_ResetBits(GPIOA,GPIO_Pin_4);//INT1 0
			GPIO_SetBits(GPIOA,GPIO_Pin_6);  //INT3 1
			GPIO_ResetBits(GPIOA,GPIO_Pin_7);//INT4 0
		}
	else if(a<0&&b<0)//����
		{
			a=-a;
			b=-b;
			GPIO_SetBits(GPIOA,GPIO_Pin_5);//INT2 1
			GPIO_ResetBits(GPIOA,GPIO_Pin_4);//INT1 0
			GPIO_SetBits(GPIOA,GPIO_Pin_7);  //INT4 1
			GPIO_ResetBits(GPIOA,GPIO_Pin_6);//INT3 0
		}
	TIM_SetCompare1(TIM2,a);   //PD14
	TIM_SetCompare2(TIM2,b);   //PD15

}
void speed(int a ,int b)   //���ƹ�ˢ�ͱ�ˢ
{
	TIM_SetCompare3(TIM2,a);   //PD14
	TIM_SetCompare4(TIM2,b);   //PD15
}
