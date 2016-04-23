//电子智能管家
/*
  函数功能表：
  LED5(a)：测试程序，a为真，点亮led5
	void GPIO_LED_InitConfig(void); //led初始化
	void Motor_InitConfig(void); //初始化函数
  void speed(int a ,int b);//控制刷子
  void move(int a,int b);//控制电机
	void ADC_InitConfig(void); //ADC初始化
	u16 Get_Adc_Average(ADC_Channel_10,u32 times); //求ADC转换后的平均值
	void USART_Config(); //串口初始化
	void EXTI_InitConfig(void);//中断初始化
	void delay_init(void);  //延时函数初始化
	void delay_ms(u16 nms);
	void delay_us(u32 nus);
	int match(char *a,char *b); //匹配函数两个值相同返回1，否则返回0
	void clear(u16 a[]);//清除数组里的值
	int getspeed(void);//测速
*/
/*
		IO口分配：:
		motor：    PA0 PA1 PA2 PA3 分别对应TIM2 的ch1 ch2 ch3 ch4  ch1和ch2控制电机 ch3和ch4控制刷子  ——pwm波
               PA4 PA5 PA6 PA7 控制机器人电机正反转
		adc：      PC0 ,PC1, PC2, PC3分别对应ADC1_IN10 ADC1_IN11 ADC1_IN12 ADC1_IN13
							 PC4 控制电量警告，ADC_IN14
		usart1：   PA9:tx  PA10:rx
		usartt3:   PD8:tx PD9:rx
		exti:      PE0 PE1 PE2 PE3 PE4
*/
/*
	特殊说明：
	红外：没有检测到输出高电平，检测到输出低电平，采样次数为50的时候，阀值大概为2000
	// printf("13:%f\n",(float)Get_Adc_Average(ADC_Channel_13,50));//右   与墙平行：2423  没有检测到物体：2689
	// printf("10:%f\n",(float)Get_Adc_Average(ADC_Channel_10,50));//右上 检测到墙：2232  没有检测到物体：2712
	// printf("11:%f\n",(float)Get_Adc_Average(ADC_Channel_11,50));//左上 检测到墙：2081  没有检测到物体：2831
	// printf("12:%f\n",(float)Get_Adc_Average(ADC_Channel_12,50));//左   与墙平行：2851  没有检测到物体：3297
	前轮测速模块：指示灯亮，低电平。灭，高电平。move(100,100)的时候，大概0.3s转变一次电平 	前轮周长：6cm
*/
#include "stm32f10x.h" //stm32官方头文件
#include "delay.h"  //延时函数
#include "led_test.h" //led灯
#include "motor.h"  //电机
#include "adc.h"   //AD
#include "usart_test.h" //串口
#include "exti.h" //中断
#include "tool.h" //一些工具
void wall(void);
u16 usart3_buffer[64],usart3_rx=0,usart3_sta=0,i;//串口变量
int flag_motor;//flag_motor是为了防止中断1和2重复执行
u16 count0=0,count1=0,count=0;
/**************************************主函数****************************************/
int main(void)
{
	delay_init();  //延时函数初始化
	GPIO_LED_InitConfig();
	Motor_InitConfig();  //电机初始化
	ADC_InitConfig(); //ADC初始化
	USART_Config(); //串口初始化
	EXTI_InitConfig();//中断初始化
	LED5(0);
	LED6(1);
	speed(0,0);
	move(0,0);
	delay_ms(100);
	while(1)
	{
		// printf("13:%f\n",(float)Get_Adc_Average(ADC_Channel_13,50));//右   与墙平行：2423  没有检测到物体：2689
		// printf("10:%f\n",(float)Get_Adc_Average(ADC_Channel_10,50));//右上 检测到墙：2232  没有检测到物体：2712
		// printf("11:%f\n",(float)Get_Adc_Average(ADC_Channel_11,50));//左上 检测到墙：2081  没有检测到物体：2831
		// printf("12:%f\n",(float)Get_Adc_Average(ADC_Channel_12,50));//左   与墙平行：2851  没有检测到物体：3297
		printf("12:%d\n",getspeed());//左   与墙平行：2851  没有检测到物体：3297
	//	USART_SendData(USART3,getspeed()+30);
		 delay_ms(500);
		LED5(0);
		LED6(1);
		if(!usart3_sta)//如果没有接收到串口信号
		{
			wall();
		}
		/***************************************无线控制部分****************************************/
		else
		{
			if(usart3_rx&0x8000)  //接收到一次数据
			{
				char a[LEN];
				for(i=0; i<LEN; i++)
				{
					a[i]=usart3_buffer[i]; //把串口的数据放入数组里面
					USART_SendData(USART3,a[i]);
					while(USART_GetFlagStatus(USART3,USART_FLAG_TXE) == RESET);//为了能够在串口上显示出来
				}
				if(match(a,"up"))
				{
					LED6(1);
					move(200,200);
				}
				else if(match(a,"down"))
				{
					LED6(0);
					move(-800,-800);
				}
				else if(match(a,"left"))
				{
					move(-400,400);
				}
				else if(match(a,"right"))
				{
					move(400,-400);
				}
				else if(match(a,"stop"))
				{
					move(0,0);
					speed(0,0);
				}
				else if(match(a,"over"))
				{
					usart3_sta=0;
				}
				else usart3_sta=0;
				usart3_rx=0;
				clear(usart3_buffer);//清除缓存
			}
		}
	}
}


/***************************************中断*******************************************/
void EXTI0_IRQHandler(void) //左边
{
	if(EXTI_GetITStatus(EXTI_Line1) != RESET)
	{
		LED5(1);
		while(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_1)!=RESET)
			move(500,-200);
	}
	while(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_0)!=RESET);//当检测到低电平的时候，才离开中断
	EXTI_ClearITPendingBit(EXTI_Line0);  //清除EXTI0线路挂起位
}
void EXTI1_IRQHandler(void) //左上角中断
{
	if(EXTI_GetITStatus(EXTI_Line1) != RESET)
	{
		if(flag_motor==1)
		{
			LED5(1);
			move(0,0);
			delay_ms(100);
			while(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_1)!=RESET||GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2)!=RESET)//当检测到低电平的时候
				move(-400,-400);
			delay_ms(500);
			LED6(0);
			move(0,0);
			delay_ms(100);
			move(400,-300);
			delay_ms(600);
			flag_motor=0;
		}
	}
	EXTI_ClearITPendingBit(EXTI_Line1);  //清除EXTI1线路挂起位
}
void EXTI2_IRQHandler(void) //右上角中断
{
	if(EXTI_GetITStatus(EXTI_Line2) != RESET)
	{
		if(flag_motor==1)
		{
			LED5(1);
			move(0,0);
			delay_ms(100);
			while(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2)!=RESET||GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_1)!=RESET)//当检测到低电平的时候，才离开中断
				move(-400,-400);
			delay_ms(500);
			LED6(0);
			move(0,0);
			delay_ms(100);
			move(-300,400);
			delay_ms(600);
			flag_motor=0;
		}
	}
	EXTI_ClearITPendingBit(EXTI_Line2);  //清除EXTI2线路挂起位
}
void EXTI3_IRQHandler(void)//右边
{
	if(EXTI_GetITStatus(EXTI_Line3) != RESET)
	{
		LED5(1);
		while(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)!=RESET)
			move(-200,500);

	}
	while(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)!=RESET);//当检测到低电平的时候，才离开中断
	EXTI_ClearITPendingBit(EXTI_Line3);  //清除EXTI3线路挂起位
}
void EXTI4_IRQHandler(void)   //电机开关中断，中断为最高优先级
{
	if(EXTI_GetITStatus(EXTI_Line4) != RESET)
	{
		if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)==RESET)   //低电平（当电机轮子抬起来的时候，微动开关关闭，为低电平）
		{
			delay_us(10);
			if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)==RESET)
			{
				LED5(1);
				move(0,0);
				speed(0,0);
			}
		}
		else
		{
			EXTI_ClearITPendingBit(EXTI_Line4);  //清除EXTI4线路挂起位
		}
	}
}
void EXTI9_5_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line5)!=RESET)
	{
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)!=RESET)
		{
			delay_ms(10);
			if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)!=RESET)
			{
				count++;
				delay_ms(10);
			}
		}
	}
	EXTI_ClearITPendingBit(EXTI_Line5);
}
//定时器4中断服务程序
void TIM4_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)//是更新中断
	{
		usart3_rx|=1<<15;	//标记接收完成
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //清除TIMx更新中断标志
		TIM4_Set(0);			//关闭TIM4
	}
}
void USART1_IRQHandler(void)       //串口接收中断，并将接收到得数据发送出
{

	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)//判断 是否 接收中断
	{
		uint16_t temp_trx[2],i;
		LED5(1);
		for(i=0; i<2; i++)
			temp_trx[i]=USART_ReceiveData(USART1);
		USART_SendData(USART1,temp_trx[1]);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);//判断 发送标志
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
	}
}

void USART3_IRQHandler(void)       //串口接收中断，并将接收到得数据发送出
{
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)//判断 是否 接收中断
	{
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);
		usart3_sta=1; //串口3中断标志位
		LED5(1);
		TIM_SetCounter(TIM4,0);//定时器清空（非常重要)
		if(usart3_rx==0) TIM4_Set(1);//开启定时器
		if(usart3_rx<LEN)
		{
			usart3_buffer[usart3_rx++]=USART_ReceiveData(USART3);
		}
		else
		{
			usart3_rx|=1<<15; //强制接收
		}
	}
}
/***********************************防止撞墙程序*************************************/
void wall(void)
{
	flag_motor=1;
	if((float)Get_Adc_Average(ADC_Channel_10,50)<2000)//右上
	{
		LED6(0);
		move(0,0);
		delay_ms(10);
		move(-300,400);//左拐
		delay_ms(300);
	}
	else if((float)Get_Adc_Average(ADC_Channel_11,50)<2000)//左上
	{
		LED6(0);
		move(0,0);
		delay_ms(10);
		move(400,-300);//右拐
		delay_ms(300);
	}
	else if((float)Get_Adc_Average(ADC_Channel_12,50)<2000)//左
	{
		move(400,-300);//右拐
		delay_ms(50);
	}
	else if((float)Get_Adc_Average(ADC_Channel_13,50)<2000)//右
	{
		move(-300,400);//左拐
		delay_ms(50);
	}
	else
	{
		move(100,100);
		speed(800,800);
	}
}
/*********************************测速**************************/
int getspeed(void)
{
	count0=count1;
	count1=count;
	delay_ms(300);
	return count1-count0;
}
/**************************END OF FILE*************************/
//测试一下git
