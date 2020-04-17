#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"

#include "key.h"
#include "usart3.h"
#include "common.h"
#include "w25qxx.h"

#include "ds18b20.h"
#include "adc.h"
#include "bh1750.h"

/************************************************
 ALIENTEK STM32F103������ ��չʵ��5
 ATK-RM04 WIFIģ�����ʵ��
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/


int main(void)
{


    delay_init();            //��ʱ������ʼ��
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
    uart_init(115200);      //���ڳ�ʼ��Ϊ115200

    while (DS18B20_Init())  //DS18B20��ʼ��
    {
        printf("DS18B20 Error\r\n");
        delay_ms(500);
    }
//    while (1)
//    {
//        short temperature;
//        temperature = DS18B20_Get_Temp();
//        printf("%d.%dC\r\n", temperature / 10, temperature % 10);
//        delay_ms(500);
//    }

    Adc_Init();             //ADC��ʼ��
//    while (1)
//    {
//              u16 adcx;
//        adcx = Get_Adc_Average(ADC_Channel_1, 10);//ֻ��ûһ���������900���͵�������1900������
//        double high = 0;
//        if (adcx < 900)    high = 0.0;
//        else if (adcx > 1900) high = 4.0;
//        else high = (adcx - 900) * 4.0 / 1000.0;    //��������4cm
//        printf("%d\t%f\r\n", adcx, high);
//        delay_ms(500);
//    }

    bh1750_iic_init();
//    while (1)
//    {
//        int lux = bh1750_ReadContinuous1();
//        printf("lux:%f\r\n", lux / 1.2f);
//        delay_ms(500);
//    }

    usart3_init(115200);        //��ʼ������3
		atk_8266_send_cmd("AT+RST", "ready", 2000);
    while (atk_8266_send_cmd("AT", "OK", 20)) //���WIFIģ���Ƿ�����
    {
        atk_8266_quit_trans();//�˳�͸��
        atk_8266_send_cmd("AT+CIPMODE=0", "OK", 200); //�ر�͸��ģʽ
        delay_ms(800);
        printf("no 8266\r\n");

    }
    while (atk_8266_send_cmd("ATE0", "OK", 20)); //�رջ���
    printf("init ok\r\n");
    atk_8266_wifiap_test(); //WIFI AP����
}


















