#include "common.h"
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//ATK-ESP8266 WIFI模块 WIFI AP驱动代码
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2014/4/3
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved
///////////////////////////////////////////////////////////////////////////////////////////////////////////
const static char Header[] = "HTTP/1.1 200 OK\r\n\Content-Length:";
char package[256];
char body[128];
char cmd[32];

//ATK-ESP8266 WIFI AP测试
//用于测试TCP/UDP连接
//返回值:0,正常
//    其他,错误代码
u8 atk_8266_wifiap_test(void)
{
    u16 rlen = 0;

    char mux = 0;
    char new_get = 0;
    atk_8266_send_cmd("AT+CIPMUX=1", "OK", 20); //0：单连接，1：多连接
    atk_8266_send_cmd("AT+CIPSERVER=1,8080", "OK", 20);   //开启Server模式，端口号为8086
    printf("begin listen...\r\n");
    USART3_RX_STA = 0;
    while (1)
    {
        delay_ms(5);
        if (USART3_RX_STA & 0X8000)     //接收到一次数据了
        {
            rlen = USART3_RX_STA & 0X7FFF; //得到本次接收到的数据长度
            USART3_RX_BUF[rlen] = 0;    //添加结束符
            printf("recv %d bytes:\r\n---------->\r\n%s\r\n<----------\r\n", rlen, USART3_RX_BUF);
            char *muxpos = strstr(USART3_RX_BUF, "+IPD,");
            char *get = strstr(USART3_RX_BUF, "GET / ");

            if (muxpos) //是来自client的请求
            {
                mux = *(muxpos + 5);
                printf("mux:%c\r\n", mux);
            }
            if (get) //是get请求
                new_get = 1;

            if (new_get)
            {
                //组件显示内容
                sprintf((char *)body, "hahaha\r\n");
                int bodylength = strlen(body);
                //组建整体数据包
                sprintf(package, "%s%d\r\n\r\n%s", Header, bodylength, body);
                int packagelength = strlen(package);
                //进入透传
                sprintf(cmd, "AT+CIPSEND=%c,%d\r\n", mux, packagelength);
                printf("cmd:%s", cmd);
                atk_8266_send_cmd(cmd, "OK", 200); //发送指定长度的数据
                //发送数据
                printf("send package:\r\n%s", package);
                atk_8266_send_data(package, "OK", 100); //发送指定长度的数据
                printf("send ok\r\n");
            }
            else printf("not get\r\n");;
            //强制关闭mux通道
            sprintf(cmd, "AT+CIPCLOSE=%c\r\n", mux);
            printf("cmd:%s", cmd);
            atk_8266_send_cmd(cmd, "OK", 200); //发送指定长度的数据
						//清除buffer，等待接收下一个连接
            USART3_RX_STA = 0;
            memset(USART3_RX_BUF, 0, sizeof(USART3_RX_BUF));
        }
    }

    return 0;
}







