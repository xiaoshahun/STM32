#include "iic.h"

#include "delay.h"
//��ʼ��IIC
void IIC_Init(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;
//	I2C_InitTypeDef I2C_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	//ʹ��GPIOBʱ��
//	RCC_APB1PeriphClockCmd(	RCC_APB1Periph_I2C1, ENABLE );	//ʹ��I2Cʱ��  
	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_6|GPIO_Pin_7); 	//PB6,PB7 �����
	
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
//    while (I2C_GetFlagStatus(I2C1,  I2C_FLAG_BUSY));  //���I2C�����Ƿ�æ	
//    I2C_GenerateSTART(I2C1,  ENABLE); //��I2C1 
//    //ErrorStatus status = ERROR,   ERROR�Ǹ�ö�����ͣ�ֵΪ0        
//    while( !I2C_CheckEvent(I2C1,  I2C_EVENT_MASTER_MODE_SELECT)); //EV5,��ģʽ	
//	 I2C_Send7bitAddress(I2C1,0x5f, I2C_Direction_Transmitter); //����STM32��IIC�豸�Լ��ĵ�ַ��ÿ�����ӵ�IIC�����ϵ��豸����һ���Լ��ĵ�ַ����Ϊ����Ҳ�����⡣
//	 while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));   //EV6
//	 I2C_SendData(I2C1,  addr);   //�Ĵ�����ַ
//     while(!I2C_CheckEvent(I2C1,  I2C_EVENT_MASTER_BYTE_TRANSMITTING));  //EV8,�ȴ��������� ���
//	I2C_SendData(I2C1,  data);   //��������
//	 while(!I2C_CheckEvent(I2C1,  I2C_EVENT_MASTER_BYTE_TRANSMITTING)); //�ж��Ƿ�����ɣ�  EV8,�ȴ������������
//    I2C_GenerateSTOP( I2C1,  ENABLE); //�ر�I2C����
//}

//���ģ��iic
//����IIC��ʼ�ź�
void IIC_Start(void)
{
	SDA_OUT();     //sda�����
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	delay_us(4);
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
//����IICֹͣ�ź�
void IIC_Stop(void)
{
	SDA_OUT();//sda�����
	IIC_SCL=0;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	IIC_SCL=1; 
	IIC_SDA=1;//����I2C���߽����ź�
	delay_us(4);							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA����Ϊ����  
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
	IIC_SCL=0;//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
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
//������ACKӦ��		    
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
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA_OUT(); 	    
    IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
        IIC_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
		delay_us(2);   //��TEA5767��������ʱ���Ǳ����
		IIC_SCL=1;
		delay_us(2); 
		IIC_SCL=0;	
		delay_us(2);
    }	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA����Ϊ����
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
        IIC_NAck();//����nACK
    else
        IIC_Ack(); //����ACK   
    return receive;
}



























