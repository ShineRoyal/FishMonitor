#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"

#include "key.h"

#include "w25qxx.h"

#include "usart3.h"
#include "common.h"

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
    LED_Init();                 //��ʼ����LED���ӵ�Ӳ���ӿ�
    KEY_Init();                 //��ʼ������
    W25QXX_Init();              //��ʼ��W25Q128
    usart3_init(115200);        //��ʼ������3
	
    printf("init ok\r\n");
    while (atk_8266_send_cmd("AT", "OK", 20)) //���WIFIģ���Ƿ�����
    {
        atk_8266_quit_trans();//�˳�͸��
        atk_8266_send_cmd("AT+CIPMODE=0", "OK", 200); //�ر�͸��ģʽ
        delay_ms(800);
				printf("no 8266\r\n");

    }
    while (atk_8266_send_cmd("ATE0", "OK", 20)); //�رջ���
    atk_8266_wifiap_test(); //WIFI AP����
}


















