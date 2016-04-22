#include "tim.h"
u16 count0=0,count1=0;
void Time1_Init()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);

    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;//TIM3_ETR：PD2
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOD,&GPIO_InitStructure);

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
    TIM_TimeBaseStructure.TIM_Period=0xffff;
    TIM_TimeBaseStructure.TIM_Prescaler=0;
    TIM_TimeBaseStructure.TIM_ClockDivision=0;
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);

    TIM_ETRClockMode2Config(TIM3,TIM_ExtTRGPSC_OFF,TIM_ExtTRGPolarity_NonInverted,4);//外部计数模式:不分频，上升沿有效，滤波器0
    TIM_SetCounter(TIM3,0);
    TIM_Cmd(TIM3,ENABLE);
}
u16 getspeed()
{
    u16 data;
    count0=count1;
    count1=TIM_GetCounter(TIM3);
    delay_ms(100);
    // if(count1<count0)
    //     {
    //         data=0xffff-count0+count1;
    //     }
    // else
    //     data=count1-count0;
    return count1;
}
