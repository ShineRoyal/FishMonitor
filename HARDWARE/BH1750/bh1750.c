#include  "bh1750.h"
#include "delay.h"
#include "usart.h"



void bh1750_iic_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);    //ʹ��GPIOBʱ��

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
    GPIO_SetBits(GPIOE, GPIO_Pin_0 | GPIO_Pin_1); //PB6,PB7 �����

}

/****��BH1750����д����****/
void Single_Write_bh1750(unsigned char Reg_Address)
{
    bh1750_iic_start();
    bh1750_send_byte(0x46);  //����������ַ0100 0110   ���һλ0����ʾд
    bh1750_send_byte(Reg_Address);
    bh1750_iic_stop();
}

/*
��HB1750�ж�һ������
�����������ݳ���1.2���ǹ���ǿ��
*/
unsigned short Single_Read_bh1750(void)
{
    unsigned char BUF[2];

    bh1750_iic_start();
    bh1750_send_byte(0x47);
    bh1750_recv_ack();
    BUF[0] = bh1750_recv_byte(1);
    BUF[1] = bh1750_recv_byte(0);
    bh1750_iic_stop();
    return ((BUF[0] << 8) + BUF[1]);
}


/****BH1750��ʼ��****/
void bh1750_Init(void)
{
    bh1750_iic_init();
    Single_Write_bh1750(0x01);		//�ϵ�
    Single_Write_bh1750(0x10);		//�����߷ֱ���ģʽ��1lx��120ms
}



/****��BH1750���ζ�Lux****/
/****
ģʽѡ��H-Resolution Mode,�ֱ���1lux
���ζ�Opecode��0010 0000 ����0x20
������ַ��0100011��ADDR��GND
****/
u16 bh1750_ReadOneTime(void)
{
	u8 BUF[2];
	bh1750_iic_start();
	bh1750_send_byte(0x46);  //����������ַ0100 0110   ���һλ0����ʾд
	bh1750_recv_ack();
	bh1750_send_byte(0x20);   //����One time H-Resolution Mode��Opecode 0010 0000���ֱ���Ϊ1lux
	bh1750_recv_ack();
	bh1750_iic_stop();
	delay_ms(200);
	bh1750_iic_start();
	bh1750_send_byte(0x47);  //
	bh1750_recv_ack();
	BUF[0]=bh1750_recv_byte(1);
	//IIC_Ack();        //����Ӧ��
	//temp1=temp<<8;
	BUF[1]=bh1750_recv_byte(0);
	//IIC_NAck();        //������Ӧ��	
	bh1750_iic_stop();
	
	return BUF[0]*256+BUF[1];
// 	temp2=BUF[0];
// 	temp2=(temp2<<8)+BUF[1];
// 	lux=(float)temp2/1.2;    //lux��float��
	
	//return lux;
	//delay_ms(200);
}

/****��BH1750������Lux****/
/****
ģʽѡ��H-Resolution Mode,�ֱ���1lux
������Opecode��0001 0000
������ַ��0100011��ADDR��GND
****/
u16  bh1750_ReadContinuous1(void)
{
	u8 BUF[2];
//	u16 temp=0,temp1=0;
	bh1750_iic_start();
  bh1750_send_byte(0x46);  //����������ַ0100 0110   ���һλ0����ʾд
	bh1750_recv_ack();
	bh1750_send_byte(0x10);  //����Continuous H-Resolution Mode��Opecode 0001 0000���ֱ���Ϊ1lux
	bh1750_recv_ack();
	bh1750_iic_stop();
	//delay_ms(200);
	bh1750_iic_start();
  bh1750_send_byte(0x47);  
	bh1750_recv_ack();
	
	
 	BUF[0]=bh1750_recv_byte(1);
	BUF[1]=bh1750_recv_byte(0);	
	
	
	//temp=bh1750_recv_byte(1);
	//temp1=bh1750_recv_byte(0);


	bh1750_iic_stop();
	return BUF[0]*256+BUF[1];
	//temp2=temp1+(temp<<8);
	//lux=(float)temp2/1.2;    //lux��float��


}


/**************************************
��ʼ�ź�
**************************************/
void bh1750_iic_start(void)
{
    BH1750_SDA_OUT()       //����SDA��Ϊ���
    //�ڿ�ʼ���ݴ���ǰ������SDA��SCL������
    BH1750_IIC_SDA = 1;  //������ʼ�����������źţ��ͷ�����
    delay_us(2);
    BH1750_IIC_SCL = 1;

    delay_us(5);   //Tsu;STA����ʼ�����Ľ���ʱ�����4.7us��

    BH1750_IIC_SDA = 0;  //SDA�ɸ߱�Ϊ�ͱ�ʾ��ʼ�ź�
    delay_us(4);   //��ʼ�����ı���ʱ�����4us
    BH1750_IIC_SCL = 0;  //ǯסIIC���ߣ�׼�����ͻ��߽�������
    delay_us(2);
}

/**************************************
ֹͣ�ź�
**************************************/
void bh1750_iic_stop(void)
{
    BH1750_SDA_OUT()       //����SDA��Ϊ���

    //IIC_SCL=0;    //�˾��ǵ���BH1750ʱ�ӣ�2018.03.03

    BH1750_IIC_SDA = 0; //����ֹͣ�źŵ������ź�
    delay_us(2);
    BH1750_IIC_SCL = 1; //����ֹͣ�źŵ�ʱ���ź�
    delay_us(5);  //ֹͣ�źŵĽ���ʱ�����4us
    BH1750_IIC_SDA = 1; //����ֹͣ�ź�
    delay_us(4);    //;
}

/**************************************
����Ӧ���ź�
��ڲ���:ack (0:ACK 1:NAK)
**************************************/
void bh1750_send_ack(char ack)
{
    BH1750_IIC_SCL = 0;
    BH1750_SDA_OUT();
    BH1750_IIC_SDA = ack; //������Ӧ��
    delay_us(2);
    BH1750_IIC_SCL = 1;
    delay_us(4);        //SCL�ߵ�ƽ���ڴ���4us
    BH1750_IIC_SCL = 0;       //��ʱ���ߣ�ǯסIIC�����Ա��������
    delay_us(2);

}

/**************************************
����Ӧ���ź�
**************************************/
char bh1750_recv_ack(void)
{
    u8 ucErrorTime = 0;
    BH1750_SDA_IN();     //SDA���ó�����
    BH1750_IIC_SDA = 1;
    delay_us(2);
    BH1750_IIC_SCL = 1;
    delay_us(2);
    while (BH1750_READ_SDA)
    {
        ucErrorTime++;
        if (ucErrorTime >= 250)
        {
            bh1750_iic_stop();
            return 1;
        }
    }
    BH1750_IIC_SCL = 0;
    return 0;
}

/**************************************
��IIC���߷���һ���ֽ�����
**************************************/
void bh1750_send_byte(char dat)
{
    u8 i;
    BH1750_SDA_OUT();

    for (i = 0; i < 8; i++)
    {
        if ((dat << i) & 0x80)  BH1750_IIC_SDA = 1; //�жϷ���λ
        else BH1750_IIC_SDA = 0;

        delay_us(2);
        BH1750_IIC_SCL = 1;   //����SCL��֪ͨ��������ʼ��������λ
        delay_us(4);   //��֤SCL�ߵ�ƽ���ڴ���4us
        BH1750_IIC_SCL = 0;  //����SCL������SDA������һλ���ݡ���Ϊֻ����SCL=0ʱ������SDA��״̬�����仯
        delay_us(2);
    }
    //�����ľ��ǵ���BH1750ʱ��ӵģ�2018.03.03
    //�ر�ע��;�÷�����ʱ�������ľ䲻��ȥ����ȥ����õ�����������
    //�÷�����ʱ�������ľ���ע�͵�������Ҳ�������⣬����ԭ����ʱ���������
//   IIC_SDA=0;
//   delay_us(2);
//   IIC_SCL=1;
//   delay_us(2);

    BH1750_IIC_SCL = 0;    //����SCL��Ϊ�´����ݴ�������׼��
    delay_us(2);
}

/**************************************
��IIC���߽���һ���ֽ�����
�������մ��������������ݣ�ackΪ1ʱ������Ack(����Ӧ��);ackΪ0ʱ������NAck(������Ӧ��)
**************************************/
char bh1750_recv_byte(unsigned char ack)
{
    unsigned char i, receive = 0;
    BH1750_SDA_IN();
    for (i = 0; i < 8; i++)
    {
        BH1750_IIC_SCL = 0;  //��SCLΪ�ͣ�׼����������λ
        delay_us(5);  //ʱ�ӵ͵�ƽ���ڴ���4.7us
        BH1750_IIC_SCL = 1; //��SCLΪ�ߣ�ʹSDA��������Ч
        delay_us(2);
        receive = receive << 1;
        if (BH1750_READ_SDA) receive = receive + 1; //��ȡSDA���ѽ��յ�����λ����receve�У����˴�д��IIC_SDA==1����ʧ�ܣ�˼��ԭ��
        delay_us(2);
    }
    if (!ack)
        bh1750_send_ack(1);   //����NAck
    else
        bh1750_send_ack(0);    //����Ack
    //IIC_SCL=0;      //�ڴ˴��к��ô�������(�²⣺Ϊ�´����ݴ�������׼���� //ǯסIIC���ߣ�׼�����ͻ��߽�������)
    //delay_us(2);
    return receive;
}





