//�������ܹܼ�
/*
  �������ܱ�
  LED5(a)�����Գ���aΪ�棬����led5
	void GPIO_LED_InitConfig(void); //led��ʼ��
	void Motor_InitConfig(void); //��ʼ������
  void speed(int a ,int b);//����ˢ��
  void move(int a,int b);//���Ƶ��
	void ADC_InitConfig(void); //ADC��ʼ��
	u16 Get_Adc_Average(ADC_Channel_10,u32 times); //��ADCת�����ƽ��ֵ
	void USART_Config(); //���ڳ�ʼ��
	void EXTI_InitConfig(void);//�жϳ�ʼ��
	void delay_init(void);  //��ʱ������ʼ��
	void delay_ms(u16 nms);
	void delay_us(u32 nus);
	int match(char *a,char *b); //ƥ�亯������ֵ��ͬ����1�����򷵻�0
	void clear(u16 a[]);//����������ֵ
	int getspeed(void);//����
*/
/*
		IO�ڷ��䣺:
		motor��    PA0 PA1 PA2 PA3 �ֱ��ӦTIM2 ��ch1 ch2 ch3 ch4  ch1��ch2���Ƶ�� ch3��ch4����ˢ��  ����pwm��
               PA4 PA5 PA6 PA7 ���ƻ����˵������ת
		adc��      PC0 ,PC1, PC2, PC3�ֱ��ӦADC1_IN10 ADC1_IN11 ADC1_IN12 ADC1_IN13
							 PC4 ���Ƶ������棬ADC_IN14
		usart1��   PA9:tx  PA10:rx
		usartt3:   PD8:tx PD9:rx
		exti:      PE0 PE1 PE2 PE3 PE4
*/
/*
	����˵����
	���⣺û�м�⵽����ߵ�ƽ����⵽����͵�ƽ����������Ϊ50��ʱ�򣬷�ֵ���Ϊ2000
	// printf("13:%f\n",(float)Get_Adc_Average(ADC_Channel_13,50));//��   ��ǽƽ�У�2423  û�м�⵽���壺2689
	// printf("10:%f\n",(float)Get_Adc_Average(ADC_Channel_10,50));//���� ��⵽ǽ��2232  û�м�⵽���壺2712
	// printf("11:%f\n",(float)Get_Adc_Average(ADC_Channel_11,50));//���� ��⵽ǽ��2081  û�м�⵽���壺2831
	// printf("12:%f\n",(float)Get_Adc_Average(ADC_Channel_12,50));//��   ��ǽƽ�У�2851  û�м�⵽���壺3297
	ǰ�ֲ���ģ�飺ָʾ�������͵�ƽ���𣬸ߵ�ƽ��move(100,100)��ʱ�򣬴��0.3sת��һ�ε�ƽ 	ǰ���ܳ���6cm
*/
#include "stm32f10x.h" //stm32�ٷ�ͷ�ļ�
#include "delay.h"  //��ʱ����
#include "led_test.h" //led��
#include "motor.h"  //���
#include "adc.h"   //AD
#include "usart_test.h" //����
#include "exti.h" //�ж�
#include "tool.h" //һЩ����
void wall(void);
u16 usart3_buffer[64],usart3_rx=0,usart3_sta=0,i;//���ڱ���
int flag_motor;//flag_motor��Ϊ�˷�ֹ�ж�1��2�ظ�ִ��
u16 count0=0,count1=0,count=0;
/**************************************������****************************************/
int main(void)
{
	delay_init();  //��ʱ������ʼ��
	GPIO_LED_InitConfig();
	Motor_InitConfig();  //�����ʼ��
	ADC_InitConfig(); //ADC��ʼ��
	USART_Config(); //���ڳ�ʼ��
	EXTI_InitConfig();//�жϳ�ʼ��
	LED5(0);
	LED6(1);
	speed(0,0);
	move(0,0);
	delay_ms(100);
	while(1)
	{
		// printf("13:%f\n",(float)Get_Adc_Average(ADC_Channel_13,50));//��   ��ǽƽ�У�2423  û�м�⵽���壺2689
		// printf("10:%f\n",(float)Get_Adc_Average(ADC_Channel_10,50));//���� ��⵽ǽ��2232  û�м�⵽���壺2712
		// printf("11:%f\n",(float)Get_Adc_Average(ADC_Channel_11,50));//���� ��⵽ǽ��2081  û�м�⵽���壺2831
		// printf("12:%f\n",(float)Get_Adc_Average(ADC_Channel_12,50));//��   ��ǽƽ�У�2851  û�м�⵽���壺3297
		printf("12:%d\n",getspeed());//��   ��ǽƽ�У�2851  û�м�⵽���壺3297
	//	USART_SendData(USART3,getspeed()+30);
		 delay_ms(500);
		LED5(0);
		LED6(1);
		if(!usart3_sta)//���û�н��յ������ź�
		{
			wall();
		}
		/***************************************���߿��Ʋ���****************************************/
		else
		{
			if(usart3_rx&0x8000)  //���յ�һ������
			{
				char a[LEN];
				for(i=0; i<LEN; i++)
				{
					a[i]=usart3_buffer[i]; //�Ѵ��ڵ����ݷ�����������
					USART_SendData(USART3,a[i]);
					while(USART_GetFlagStatus(USART3,USART_FLAG_TXE) == RESET);//Ϊ���ܹ��ڴ�������ʾ����
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
				clear(usart3_buffer);//�������
			}
		}
	}
}


/***************************************�ж�*******************************************/
void EXTI0_IRQHandler(void) //���
{
	if(EXTI_GetITStatus(EXTI_Line1) != RESET)
	{
		LED5(1);
		while(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_1)!=RESET)
			move(500,-200);
	}
	while(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_0)!=RESET);//����⵽�͵�ƽ��ʱ�򣬲��뿪�ж�
	EXTI_ClearITPendingBit(EXTI_Line0);  //���EXTI0��·����λ
}
void EXTI1_IRQHandler(void) //���Ͻ��ж�
{
	if(EXTI_GetITStatus(EXTI_Line1) != RESET)
	{
		if(flag_motor==1)
		{
			LED5(1);
			move(0,0);
			delay_ms(100);
			while(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_1)!=RESET||GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2)!=RESET)//����⵽�͵�ƽ��ʱ��
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
	EXTI_ClearITPendingBit(EXTI_Line1);  //���EXTI1��·����λ
}
void EXTI2_IRQHandler(void) //���Ͻ��ж�
{
	if(EXTI_GetITStatus(EXTI_Line2) != RESET)
	{
		if(flag_motor==1)
		{
			LED5(1);
			move(0,0);
			delay_ms(100);
			while(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2)!=RESET||GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_1)!=RESET)//����⵽�͵�ƽ��ʱ�򣬲��뿪�ж�
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
	EXTI_ClearITPendingBit(EXTI_Line2);  //���EXTI2��·����λ
}
void EXTI3_IRQHandler(void)//�ұ�
{
	if(EXTI_GetITStatus(EXTI_Line3) != RESET)
	{
		LED5(1);
		while(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)!=RESET)
			move(-200,500);

	}
	while(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)!=RESET);//����⵽�͵�ƽ��ʱ�򣬲��뿪�ж�
	EXTI_ClearITPendingBit(EXTI_Line3);  //���EXTI3��·����λ
}
void EXTI4_IRQHandler(void)   //��������жϣ��ж�Ϊ������ȼ�
{
	if(EXTI_GetITStatus(EXTI_Line4) != RESET)
	{
		if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)==RESET)   //�͵�ƽ�����������̧������ʱ��΢�����عرգ�Ϊ�͵�ƽ��
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
			EXTI_ClearITPendingBit(EXTI_Line4);  //���EXTI4��·����λ
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
//��ʱ��4�жϷ������
void TIM4_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)//�Ǹ����ж�
	{
		usart3_rx|=1<<15;	//��ǽ������
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //���TIMx�����жϱ�־
		TIM4_Set(0);			//�ر�TIM4
	}
}
void USART1_IRQHandler(void)       //���ڽ����жϣ��������յ������ݷ��ͳ�
{

	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)//�ж� �Ƿ� �����ж�
	{
		uint16_t temp_trx[2],i;
		LED5(1);
		for(i=0; i<2; i++)
			temp_trx[i]=USART_ReceiveData(USART1);
		USART_SendData(USART1,temp_trx[1]);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);//�ж� ���ͱ�־
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
	}
}

void USART3_IRQHandler(void)       //���ڽ����жϣ��������յ������ݷ��ͳ�
{
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)//�ж� �Ƿ� �����ж�
	{
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);
		usart3_sta=1; //����3�жϱ�־λ
		LED5(1);
		TIM_SetCounter(TIM4,0);//��ʱ����գ��ǳ���Ҫ)
		if(usart3_rx==0) TIM4_Set(1);//������ʱ��
		if(usart3_rx<LEN)
		{
			usart3_buffer[usart3_rx++]=USART_ReceiveData(USART3);
		}
		else
		{
			usart3_rx|=1<<15; //ǿ�ƽ���
		}
	}
}
/***********************************��ֹײǽ����*************************************/
void wall(void)
{
	flag_motor=1;
	if((float)Get_Adc_Average(ADC_Channel_10,50)<2000)//����
	{
		LED6(0);
		move(0,0);
		delay_ms(10);
		move(-300,400);//���
		delay_ms(300);
	}
	else if((float)Get_Adc_Average(ADC_Channel_11,50)<2000)//����
	{
		LED6(0);
		move(0,0);
		delay_ms(10);
		move(400,-300);//�ҹ�
		delay_ms(300);
	}
	else if((float)Get_Adc_Average(ADC_Channel_12,50)<2000)//��
	{
		move(400,-300);//�ҹ�
		delay_ms(50);
	}
	else if((float)Get_Adc_Average(ADC_Channel_13,50)<2000)//��
	{
		move(-300,400);//���
		delay_ms(50);
	}
	else
	{
		move(100,100);
		speed(800,800);
	}
}
/*********************************����**************************/
int getspeed(void)
{
	count0=count1;
	count1=count;
	delay_ms(300);
	return count1-count0;
}
/**************************END OF FILE*************************/
//����һ��git
