#include "LED.h"
#include "stm32f10x_gpio.h"
#include "delay.h"

#if 1
void LED_init()
{
	GPIO_InitTypeDef GPIO_InitStrute;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOB,ENABLE); //ʹ��ʱ��
	GPIO_InitStrute.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStrute.GPIO_Mode = GPIO_Mode_Out_PP;//�������
	GPIO_InitStrute.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB,&GPIO_InitStrute);//��ʼ��GPIO
	GPIO_SetBits(GPIOB,GPIO_Pin_5);//����BSRR, �����
	
	GPIO_Init(GPIOE,&GPIO_InitStrute);//��ʼ��GPIO
	GPIO_SetBits(GPIOE,GPIO_Pin_5);//����BSRR, �����
}
#else
void LED_init()
{
GPIO_InitTypeDef GPIO_InitStructure;
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE); //ʹ�� PB,PE �˿�ʱ��
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
//LED0-->PB.5 �������
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//�������
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; //LED1-->PE.5 �������
GPIO_Init(GPIOE, &GPIO_InitStructure);
GPIO_SetBits(GPIOE,GPIO_Pin_5);
}
#endif
void Light()
{
		GPIO_SetBits(GPIOE,GPIO_Pin_5);//����BSRR, �����
		delay_ms(1000);
		GPIO_ResetBits(GPIOE,GPIO_Pin_5);//����BRR�� �����
		delay_ms(1000);
	
}

