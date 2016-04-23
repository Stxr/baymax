//���ܣ���Ҫ����������ADCת�������ֵ
//IO:PA9 ����  PA10����
#include "usart_test.h"
void USART1_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	/* config USART1 clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);

	/* USART1 GPIO config */
	/* Configure USART1 Tx (PA.09) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/* Configure USART1 Rx (PA.10) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* USART1 mode config */
	USART_InitStructure.USART_BaudRate = 9600;                  //������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;   //��������λ��
	USART_InitStructure.USART_StopBits = USART_StopBits_1;        //ֹͣλ1
	USART_InitStructure.USART_Parity = USART_Parity_No ;          //����У��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  //������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;  //ʹ�ý��պͷ��͹���
	USART_Init(USART1, &USART_InitStructure);
	USART_Cmd(USART1, ENABLE);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);     //����1 �����ж� ʹ��

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);   //�����ж�����Ϊ��2�飬4λ������ռ���ȼ���4λ������Ӧ���ȼ�

	NVIC_InitStructure.NVIC_IRQChannel=USART1_IRQn;  //�����ж�1
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;  //������ȼ���ֹ��̨
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
void USART3_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	/* config USART3 clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO| RCC_APB2Periph_GPIOD, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	GPIO_PinRemapConfig(GPIO_FullRemap_USART3, ENABLE);

	/* USART3 GPIO config */
	/* Configure USART3 Tx (PD.08) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	/* Configure USART3 Rx (PD.9) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	/* USART3 mode config */
	USART_InitStructure.USART_BaudRate = 9600;                  //������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;   //��������λ��
	USART_InitStructure.USART_StopBits = USART_StopBits_1;        //ֹͣλ1
	USART_InitStructure.USART_Parity = USART_Parity_No ;          //����У��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  //������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;  //ʹ�ý��պͷ��͹���
	USART_Init(USART3, &USART_InitStructure);
	USART_Cmd(USART3, ENABLE);
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);     //����3 �����ж� ʹ��

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);   //�����ж�����Ϊ��2�飬4λ������ռ���ȼ���4λ������Ӧ���ȼ�

	NVIC_InitStructure.NVIC_IRQChannel=USART3_IRQn;  //�����ж�3
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;  //������ȼ���ֹ��̨
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	TIM4_Init(100-1,7200-1);		//10ms�ж�
	usart3_rx=0;		//����
	TIM4_Set(0);			//�رն�ʱ��4
}
void USART_Config(void)
{
	USART1_Config();
	USART3_Config();
}
//����TIM4�Ŀ���
//sta:0���ر�;1,����;
void TIM4_Set(u8 sta)
{
	if(sta)
		{
			TIM_SetCounter(TIM4,0);//���������
			TIM_Cmd(TIM4, ENABLE);  //ʹ��TIMx
		}
	else
		TIM_Cmd(TIM4, DISABLE);//�رն�ʱ��4
}
void TIM4_Init(u16 arr,u16 psc)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //ʱ��ʹ��//TIM4ʱ��ʹ��

	//��ʱ��TIM4��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM4�ж�,��������ж�


	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
}

///�ض���c�⺯��printf��USART1
int fputc(int ch, FILE *f)
{
	/* ����һ���ֽ����ݵ�USART1 */
	USART_SendData(USART3, (uint8_t) ch);

	/* �ȴ�������� */
	while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);

	return (ch);
}

///�ض���c�⺯��scanf��USART1
int fgetc(FILE *f)
{
	/* �ȴ�����1�������� */
	while (USART_GetFlagStatus(USART3, USART_FLAG_RXNE) == RESET);

	return (int)USART_ReceiveData(USART3);
}
/*********************************************END OF FILE**********************/
