#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"

#include "key.h"

#include "w25qxx.h"
#include "ds18b20.h"
#include "usart3.h"
#include "common.h"

/************************************************
 ALIENTEK STM32F103开发板 扩展实验5
 ATK-RM04 WIFI模块测试实验
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司
 作者：正点原子 @ALIENTEK
************************************************/


int main(void)
{
    short temperature;

    delay_init();            //延时函数初始化
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
    uart_init(115200);      //串口初始化为115200
    LED_Init();                 //初始化与LED连接的硬件接口
    KEY_Init();                 //初始化按键
    W25QXX_Init();              //初始化W25Q128
    usart3_init(115200);        //初始化串口3
    while (DS18B20_Init())  //DS18B20初始化
    {
        printf("DS18B20 Error\r\n");
        delay_ms(500);
    }
    temperature = DS18B20_Get_Temp();
    while (1)
    {
        temperature = DS18B20_Get_Temp();
        printf("%d.%dC\r\n", temperature / 10, temperature % 10);
        delay_ms(500);
    }
    printf("init ok\r\n");
    while (atk_8266_send_cmd("AT", "OK", 20)) //检查WIFI模块是否在线
    {
        atk_8266_quit_trans();//退出透传
        atk_8266_send_cmd("AT+CIPMODE=0", "OK", 200); //关闭透传模式
        delay_ms(800);
        printf("no 8266\r\n");

    }
    while (atk_8266_send_cmd("ATE0", "OK", 20)); //关闭回显
    atk_8266_wifiap_test(); //WIFI AP测试
}


















