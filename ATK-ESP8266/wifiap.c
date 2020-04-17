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
char package[256];
char body[128];
char cmd[32];

//ATK-ESP8266 WIFI AP����
//���ڲ���TCP/UDP����
//����ֵ:0,����
//    ����,�������
u8 atk_8266_wifiap_test(void)
{
    u16 rlen = 0;

    char mux = 0;
    char new_get = 0;
    atk_8266_send_cmd("AT+CIPMUX=1", "OK", 20); //0�������ӣ�1��������
    atk_8266_send_cmd("AT+CIPSERVER=1,8080", "OK", 20);   //����Serverģʽ���˿ں�Ϊ8086
    printf("begin listen...\r\n");
    USART3_RX_STA = 0;
    while (1)
    {
        delay_ms(5);
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
                //�����ʾ����
                sprintf((char *)body, "hahaha\r\n");
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
        }
    }

    return 0;
}







