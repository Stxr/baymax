//���ܣ�����ADC������DMA�ķ�ʽ��adc��ֵ������
// IO:PC0 ,PC1, PC2, PC3�ֱ��ӦADC1_IN10 ADC1_IN11 ADC1_IN12 ADC1_IN13
#include "adc.h"
#include "delay.h"
#define ADC1_DR_Address    ((u32)0x40012400+0x4c)
void ADC_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_ADC1,ENABLE);  //����GPIOC��ADC1ʱ��

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;  //��Ϊadc�����ʱ�򣬱�����ģ������
	GPIO_Init(GPIOC,&GPIO_InitStructure);      //��Ϊ�����ʱ������������
}
void ADC_Mode_Config(void)
{
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_Mode=ADC_Mode_Independent;  //����adcģʽ
	ADC_InitStructure.ADC_ScanConvMode=DISABLE;  //ɨ��ģʽ�õ�ͨ���ɼ�
	ADC_InitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None; //ת��ʹ�����ת���������ⲿ����ת��
	ADC_InitStructure.ADC_DataAlign=ADC_DataAlign_Right;  //�Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel=5;  //Ҫת����ͨ����Ŀ��5
	ADC_Init(ADC1,&ADC_InitStructure);

	RCC_ADCCLKConfig(RCC_PCLK2_Div6);  //PCLK��6��Ƶ��12Hz ���Ϊ14Hz

	ADC_Cmd(ADC1,ENABLE);  //ʹ��ADC1
	ADC_ResetCalibration(ADC1);//��λУ׼�Ĵ���
	while(ADC_GetResetCalibrationStatus(ADC1));//�ȴ���λ���

	ADC_StartCalibration(ADC1);//ADCУ׼
	while(ADC_GetCalibrationStatus(ADC1)); //�ȴ�У׼���

	//ADC_SoftwareStartConvCmd(ADC1,ENABLE);  //�������ʹ��
}
void ADC_InitConfig(void)
{
	ADC_GPIO_Config();
	ADC_Mode_Config();
}
u16 Get_ADC(u8 ch)
{
	ADC_RegularChannelConfig(ADC1,ch,1,ADC_SampleTime_55Cycles5); //	ADC1 chͨ�� ��������Ϊ55.5
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);  //�������ʹ��

	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������
	return ADC_GetConversionValue(ADC1);	//�������һ��ADC1�������ת�����
}
u16 Get_Adc_Average(u8 ch,u32 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0; t<times; t++)
		{
			temp_val+=Get_ADC(ch);
			delay_ms(1);
		}
	return temp_val/times;
}
