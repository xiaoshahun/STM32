#include "iic.h"

#include "delay.h"
//初始化IIC
void IIC_Init(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;
//	I2C_InitTypeDef I2C_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	//使能GPIOB时钟
//	RCC_APB1PeriphClockCmd(	RCC_APB1Periph_I2C1, ENABLE );	//使能I2C时钟  
	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_6|GPIO_Pin_7); 	//PB6,PB7 输出高
	
//	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
//	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
//	I2C_InitStructure.I2C_ClockSpeed = 400000;
//	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_16_9;
//	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
//	I2C_InitStructure.I2C_OwnAddress1 = 0x5f;
//	I2C_Init(I2C1, &I2C_InitStructure);
//	I2C_Cmd(I2C1, ENABLE); 
}


//void I2C_WriteByte(uint8_t addr,uint8_t data)
//{
//     //FlagStatus bitstatus = RESET
//    while (I2C_GetFlagStatus(I2C1,  I2C_FLAG_BUSY));  //检查I2C总线是否繁忙	
//    I2C_GenerateSTART(I2C1,  ENABLE); //打开I2C1 
//    //ErrorStatus status = ERROR,   ERROR是个枚举类型，值为0        
//    while( !I2C_CheckEvent(I2C1,  I2C_EVENT_MASTER_MODE_SELECT)); //EV5,主模式	
//	 I2C_Send7bitAddress(I2C1,0x5f, I2C_Direction_Transmitter); //配置STM32的IIC设备自己的地址，每个连接到IIC总线上的设备都有一个自己的地址，作为主机也不例外。
//	 while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));   //EV6
//	 I2C_SendData(I2C1,  addr);   //寄存器地址
//     while(!I2C_CheckEvent(I2C1,  I2C_EVENT_MASTER_BYTE_TRANSMITTING));  //EV8,等待发送数据 完成
//	I2C_SendData(I2C1,  data);   //发送数据
//	 while(!I2C_CheckEvent(I2C1,  I2C_EVENT_MASTER_BYTE_TRANSMITTING)); //判断是否发送完成，  EV8,等待发送数据完成
//    I2C_GenerateSTOP( I2C1,  ENABLE); //关闭I2C总线
//}

//软件模拟iic
//产生IIC起始信号
void IIC_Start(void)
{
	SDA_OUT();     //sda线输出
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	delay_us(4);
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCL=0;//钳住I2C总线，准备发送或接收数据 
}	  
//产生IIC停止信号
void IIC_Stop(void)
{
	SDA_OUT();//sda线输出
	IIC_SCL=0;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	IIC_SCL=1; 
	IIC_SDA=1;//发送I2C总线结束信号
	delay_us(4);							   	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA设置为输入  
	IIC_SDA=1;delay_us(1);	   
	IIC_SCL=1;delay_us(1);	 
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL=0;//时钟输出0 	   
	return 0;  
} 
//产生ACK应答
void IIC_Ack(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=0;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}
//不产生ACK应答		    
void IIC_NAck(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=1;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}					 				     
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA_OUT(); 	    
    IIC_SCL=0;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        IIC_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
		delay_us(2);   //对TEA5767这三个延时都是必须的
		IIC_SCL=1;
		delay_us(2); 
		IIC_SCL=0;	
		delay_us(2);
    }	 
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        IIC_SCL=0; 
        delay_us(2);
		IIC_SCL=1;
        receive<<=1;
        if(READ_SDA)receive++;   
		delay_us(1); 
    }					 
    if (!ack)
        IIC_NAck();//发送nACK
    else
        IIC_Ack(); //发送ACK   
    return receive;
}



























