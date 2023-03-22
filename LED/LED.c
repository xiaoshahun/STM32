#include "LED.h"
#include "stm32f10x_gpio.h"
#include "delay.h"

#if 1
void LED_init()
{
	GPIO_InitTypeDef GPIO_InitStrute;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOB,ENABLE); //使能时钟
	GPIO_InitStrute.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStrute.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
	GPIO_InitStrute.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB,&GPIO_InitStrute);//初始化GPIO
	GPIO_SetBits(GPIOB,GPIO_Pin_5);//控制BSRR, 输出高
	
	GPIO_Init(GPIOE,&GPIO_InitStrute);//初始化GPIO
	GPIO_SetBits(GPIOE,GPIO_Pin_5);//控制BSRR, 输出高
}
#else
void LED_init()
{
GPIO_InitTypeDef GPIO_InitStructure;
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE); //使能 PB,PE 端口时钟
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
//LED0-->PB.5 推挽输出
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//推挽输出
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; //LED1-->PE.5 推挽输出
GPIO_Init(GPIOE, &GPIO_InitStructure);
GPIO_SetBits(GPIOE,GPIO_Pin_5);
}
#endif
void Light()
{
		GPIO_SetBits(GPIOE,GPIO_Pin_5);//控制BSRR, 输出高
		delay_ms(1000);
		GPIO_ResetBits(GPIOE,GPIO_Pin_5);//控制BRR， 输出低
		delay_ms(1000);
	
}

