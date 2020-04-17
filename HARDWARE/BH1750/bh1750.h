#ifndef _BH1750_H_
#define _BH1750_H_
#include  "bh1750.h"
#include "delay.h"
#include "usart.h"

/*
*   ����   VCC-->3.3V      GND-->GND
*    ADDR----->GND
*  SCL ----->pb6
*    sda ----->pb5
*/


//IO��������

#define BH1750_SDA_IN()  {GPIOE->CRL&=0XFFFFFF0F;GPIOE->CRL|=(u32)8<<4;}
#define BH1750_SDA_OUT() {GPIOE->CRL&=0XFFFFFF0F;GPIOE->CRL|=(u32)3<<4;}

//IO��������
#define BH1750_IIC_SCL    PEout(0) //SCL
#define BH1750_IIC_SDA    PEout(1) //SDA     
#define BH1750_READ_SDA   PEin(1)  //����SDA 

#define   BH1750_SlaveAddress   0x46 //����������IIC�����еĴӵ�ַ,����ALT  ADDRESS��ַ���Ų�ͬ�޸�
//ALT  ADDRESS���Žӵ�ʱ��ַΪ0x46���ӵ�Դʱ��ַΪ0xB8

void bh1750_iic_init(void);
void bh1750_iic_start(void);
void bh1750_iic_stop(void);
void bh1750_send_ack(char ack);
char bh1750_recv_ack(void);
void bh1750_send_byte(char dat);
char bh1750_recv_byte(unsigned char ack);

void bh1750_iic_init(void);
void Single_Write_bh1750(unsigned char Reg_Address);
unsigned short Single_Read_bh1750(void);
void bh1750_Init(void);
u16 bh1750_ReadOneTime(void);
u16  bh1750_ReadContinuous1(void);



#endif

