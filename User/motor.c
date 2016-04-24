//驱动机器人电机以及滚刷和边刷
//IO:PA0 PA1 PA2 PA3 分别对应TIM2 的ch1 ch2 ch3 ch4  ch1和ch2控制电机 ch3和ch4控制刷子
//   PA4 PA5 PA6 PA7 控制机器人电机正反转
#include "motor.h"
void Motor_InitConfig(void)
{
	//结构体定义
	GPIO_InitTypeDef GPIO_InitStruction;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStruction;  //初始化TIM
	TIM_OCInitTypeDef TIM_OCInitTypeStruction;   //TIM输出模式配置
	//GPIO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);  //开启GPIOA的时钟和管脚的复用功能
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);  	//开启TIM2的时钟
	GPIO_InitStruction.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;  //控制pwm波
	GPIO_InitStruction.GPIO_Mode=GPIO_Mode_AF_PP;                            //复用推挽输出
	GPIO_InitStruction.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruction);

	GPIO_InitStruction.GPIO_Pin=GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;   //控制电机正反转动
	GPIO_InitStruction.GPIO_Mode=GPIO_Mode_Out_PP;                            //推挽输出
	GPIO_InitStruction.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruction);

	//TIM2
	TIM_TimeBaseStruction.TIM_Period=1000-1; //定时器从0计数到999，为一个定时周期
	TIM_TimeBaseStruction.TIM_Prescaler=0;   //设置预分频：不分频  72MHz
	TIM_TimeBaseStruction.TIM_ClockDivision=TIM_CKD_DIV1;  //设置时钟分频：不分频
	TIM_TimeBaseStruction.TIM_CounterMode=TIM_CounterMode_Up;  //向上计数模式
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStruction);  //配置TIM_CCR预装载使能
	//TIM2 CH1
	TIM_OCInitTypeStruction.TIM_OCMode=TIM_OCMode_PWM1;  //配置pwm的模式：模式1——当向前计数时为有效电平
	TIM_OCInitTypeStruction.TIM_OutputState=TIM_OutputState_Enable; //输出使能
	TIM_OCInitTypeStruction.TIM_OCPolarity=TIM_OCPolarity_High;  //有效为高电平
	TIM_OC1Init(TIM2,&TIM_OCInitTypeStruction);  //TIM2通道1初始化
	TIM_OC1PreloadConfig(TIM2,TIM_OCPreload_Enable);  //配置TIM_CCR预装载使能
	//TIM2 CH2
	TIM_OCInitTypeStruction.TIM_OCMode=TIM_OCMode_PWM1;  //配置pwm的模式：模式1——当向前计数时为有效电平
	TIM_OCInitTypeStruction.TIM_OutputState=TIM_OutputState_Enable; //输出使能
	TIM_OCInitTypeStruction.TIM_OCPolarity=TIM_OCPolarity_High;  //有效为高电平
	TIM_OC2Init(TIM2,&TIM_OCInitTypeStruction);  //TIM2通道1初始化
	TIM_OC2PreloadConfig(TIM2,TIM_OCPreload_Enable);  //配置TIM_CCR预装载使能
	//TIM2 CH3
	TIM_OCInitTypeStruction.TIM_OCMode=TIM_OCMode_PWM1;  //配置pwm的模式：模式1——当向前计数时为有效电平
	TIM_OCInitTypeStruction.TIM_OutputState=TIM_OutputState_Enable; //输出使能
	TIM_OCInitTypeStruction.TIM_OCPolarity=TIM_OCPolarity_High;  //有效为高电平
	TIM_OC3Init(TIM2,&TIM_OCInitTypeStruction);  //TIM2通道1初始化
	TIM_OC3PreloadConfig(TIM2,TIM_OCPreload_Enable);  //配置TIM_CCR预装载使能
	//TIM2 CH4
	TIM_OCInitTypeStruction.TIM_OCMode=TIM_OCMode_PWM1;  //配置pwm的模式：模式1——当向前计数时为有效电平
	TIM_OCInitTypeStruction.TIM_OutputState=TIM_OutputState_Enable; //输出使能
	TIM_OCInitTypeStruction.TIM_OCPolarity=TIM_OCPolarity_High;  //有效为高电平
	TIM_OC4Init(TIM2,&TIM_OCInitTypeStruction);  //TIM2通道1初始化
	TIM_OC4PreloadConfig(TIM2,TIM_OCPreload_Enable);  //配置TIM_CCR预装载使能

	TIM_Cmd(TIM2,ENABLE);  //使能定时器2
}
void move(int a,int b)  //控制电机
{
	if (a>=0&&b>=0) //电机正转
		{
			GPIO_SetBits(GPIOA,GPIO_Pin_4);  //INT1 1
			GPIO_ResetBits(GPIOA,GPIO_Pin_5);//INT2 0
			GPIO_SetBits(GPIOA,GPIO_Pin_6);  //INT3 1
			GPIO_ResetBits(GPIOA,GPIO_Pin_7);//INT4 0
		}
	else if(a>=0&&b<0)//右边反转
		{
			b=-b;
			GPIO_SetBits(GPIOA,GPIO_Pin_4);//INT1 1
			GPIO_ResetBits(GPIOA,GPIO_Pin_5);//INT2 0
			GPIO_SetBits(GPIOA,GPIO_Pin_7);  //INT4 1
			GPIO_ResetBits(GPIOA,GPIO_Pin_6);//INT3 0
		}
	else if(a<0&&b>=0)//左边反转
		{
			a=-a;
			GPIO_SetBits(GPIOA,GPIO_Pin_5);//INT2 1
			GPIO_ResetBits(GPIOA,GPIO_Pin_4);//INT1 0
			GPIO_SetBits(GPIOA,GPIO_Pin_6);  //INT3 1
			GPIO_ResetBits(GPIOA,GPIO_Pin_7);//INT4 0
		}
	else if(a<0&&b<0)//后退
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
void speed(int a ,int b)   //控制滚刷和边刷
{
	TIM_SetCompare3(TIM2,a);   //PD14
	TIM_SetCompare4(TIM2,b);   //PD15
}
