#include "common.h"
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//ATK-ESP8266 WIFIģ�� WIFI AP��������
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2014/4/3
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved
///////////////////////////////////////////////////////////////////////////////////////////////////////////
const static char Header[] = "HTTP/1.1 200 OK\r\n\Content-Length:";
char package[512];
char body[256];
char cmd[32];

#include "ds18b20.h"
#include "adc.h"
#include "bh1750.h"

//ATK-ESP8266 WIFI AP����
//���ڲ���TCP/UDP����
//����ֵ:0,����
//    ����,�������
u8 atk_8266_wifiap_test(void)
{
    u16 rlen = 0;
    char mux = 0;
    char new_get = 0;
    int time = 0;

    short temperature; //�洢�¶�����
    double high = 0;//�洢Һλ�߶�����
    double lux;//�洢����ǿ������
    atk_8266_send_cmd("AT+CIPMUX=1", "OK", 20); //0�������ӣ�1��������
    atk_8266_send_cmd("AT+CIPSERVER=1,80", "OK", 20);   //����Serverģʽ���˿ں�Ϊ8086
    printf("begin listen...\r\n");
    USART3_RX_STA = 0;
    while (1)
    {
        if (USART3_RX_STA & 0X8000)     //���յ�һ��������
        {
            rlen = USART3_RX_STA & 0X7FFF; //�õ����ν��յ������ݳ���
            USART3_RX_BUF[rlen] = 0;    //��ӽ�����
            printf("recv %d bytes:\r\n---------->\r\n%s\r\n<----------\r\n", rlen, USART3_RX_BUF);
            char *muxpos = strstr(USART3_RX_BUF, "+IPD,");
            char *get = strstr(USART3_RX_BUF, "GET / ");

            if (muxpos) //������client������
            {
                mux = *(muxpos + 5);
                printf("mux:%c\r\n", mux);
            }
            if (get) //��get����
                new_get = 1;

            if (new_get)
            {
                //�齨��ʾ����
                sprintf((char *)body, "<html>\
									<meta http-equiv=\"refresh\" content=\"1;url=#\">\
								temp:%d.%dC<br/>\
								high:%.2fcm<br/>\
								light:%.2flux\
								</html>", temperature / 10, temperature % 10, high, lux);

                int bodylength = strlen(body);
                //�齨�������ݰ�
                sprintf(package, "%s%d\r\n\r\n%s", Header, bodylength, body);
                int packagelength = strlen(package);
                //����͸��
                sprintf(cmd, "AT+CIPSEND=%c,%d\r\n", mux, packagelength);
                printf("cmd:%s", cmd);
                atk_8266_send_cmd(cmd, "OK", 200); //����ָ�����ȵ�����
                //��������
                printf("send package:\r\n%s", package);
                atk_8266_send_data(package, "OK", 100); //����ָ�����ȵ�����
                printf("send ok\r\n");
            }
            else printf("not get\r\n");;
            //ǿ�ƹر�muxͨ��
            sprintf(cmd, "AT+CIPCLOSE=%c\r\n", mux);
            printf("cmd:%s", cmd);
            atk_8266_send_cmd(cmd, "OK", 200); //����ָ�����ȵ�����
            //���buffer���ȴ�������һ������
            USART3_RX_STA = 0;
            memset(USART3_RX_BUF, 0, sizeof(USART3_RX_BUF));
            continue;
        }
        delay_ms(5);
        time++;
        if (time >= 40)    //200ms����һ������
        {
            time = 0;
            temperature = DS18B20_Get_Temp();

            u16 adcx = Get_Adc_Average(ADC_Channel_1, 10);
            if (adcx < 900)    high = 0.0;
            else if (adcx > 1900) high = 4.0;
            else high = (adcx - 900) * 4.0 / 1000.0;    //��������4cm
            lux = bh1750_ReadContinuous1() / 1.2f;

        }
    }
    return 0;
}







