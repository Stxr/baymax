//�þ�ûŪ32�ˣ�����һյ�������֡�
//LED:PE5�������ӷ�
#include "led_test.h"
void GPIO_LED_InitConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStruction;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);  //����GPIOE��ʱ��
	GPIO_InitStruction.GPIO_Mode=GPIO_Mode_Out_PP;  //�������
	GPIO_InitStruction.GPIO_Pin=GPIO_Pin_5|GPIO_Pin_6;   //PE5,PE6
	GPIO_InitStruction.GPIO_Speed=GPIO_Speed_50MHz;  // ����50MHZ
	GPIO_Init(GPIOE,&GPIO_InitStruction);     //�����õ�ֵװ��Ĵ�����
}
