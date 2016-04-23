#include "tim.h"
void TIM3_Init()
{
    NVIC_InitTypeDef NVIC_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
    TIM_TimeBaseStructure.TIM_Period=2000-1; //������2000ֹͣ
    TIM_TimeBaseStructure.TIM_Prescaler=3600-1;// 72MHz/3600=2KHz
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;;
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);

    TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE );//������ʱ��3���ж�

    TIM3_Set(0);
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//��ռ���ȼ�1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//�����ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
}
void TIM3_Set(u8 sta)
{
  if(sta)
    {
      TIM_SetCounter(TIM3,0);//���������
      TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx
    }
  else
    TIM_Cmd(TIM3, DISABLE);//�رն�ʱ��3
}
