#include "tim.h"
void TIM3_Init()
{
    NVIC_InitTypeDef NVIC_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
    TIM_TimeBaseStructure.TIM_Period=2000-1; //计数到2000停止
    TIM_TimeBaseStructure.TIM_Prescaler=3600-1;// 72MHz/3600=2KHz
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;;
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);

    TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE );//开启定时器3的中断

    TIM3_Set(0);
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//抢占优先级1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
}
void TIM3_Set(u8 sta)
{
  if(sta)
    {
      TIM_SetCounter(TIM3,0);//计数器清空
      TIM_Cmd(TIM3, ENABLE);  //使能TIMx
    }
  else
    TIM_Cmd(TIM3, DISABLE);//关闭定时器3
}
