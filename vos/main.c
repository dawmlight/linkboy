/*
 * Copyright (c) 2021 linkboy
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>

#include <unistd.h>

#include "ohos_init.h"
#include "cmsis_os2.h"

#include "hi_io.h"
#include "hi_gpio.h"
#include "hi_uart.h"
#include "hi_watchdog.h"

#include "remo_typedef.h"
#include "proto.h"
#include "remo_device.h"
#include "remo_SoftDelay.h"


//------------------------------------------------------------------------------------------------------------------

#include "hi_wifi_api.h"

//------------------------------------------------------------------------------------------------------------------

#include "wifi_device.h"
#include "wifi_event.h"
#include "wifi_hotspot.h"
#include "wifi_error_code.h"
#include "netifapi.h"

//#define AP_SSID "linkboy-hmos"
//#define AP_PSK  "12345678"

#define ONE_SECOND 1
#define DEF_TIMEOUT 15

static void OnHotspotStaJoinHandler(StationInfo *info);
static void OnHotspotStateChangedHandler(int state);
static void OnHotspotStaLeaveHandler(StationInfo *info);


static struct netif *g_lwip_netif = NULL;
static int g_apEnableSuccess = 0;
WifiEvent g_wifiEventHandler1 = {0};
WifiErrorCode error;
static volatile int g_hotspotStarted = 0;

static bool WifiAPTask(uint8 *p_SSID, uint8 *p_PASS)
{
    int32 eee = hi_wifi_init( 1, 1 );
    printf("hi_wifi_init, error = %d.\r\n", eee);

    //注册wifi事件的回调函数
    g_wifiEventHandler1.OnHotspotStaJoin = OnHotspotStaJoinHandler;
    g_wifiEventHandler1.OnHotspotStaLeave = OnHotspotStaLeaveHandler;
    g_wifiEventHandler1.OnHotspotStateChanged = OnHotspotStateChangedHandler;
    error = RegisterWifiEvent(&g_wifiEventHandler1);
    if (error != WIFI_SUCCESS) {
        printf("RegisterWifiEvent failed, error = %d.\r\n",error);
        return -1;
    }
    //printf("RegisterWifiEvent succeed!\r\n");

    //设置指定的热点配置
    HotspotConfig config = {0};

    strcpy(config.ssid, p_SSID);
    strcpy(config.preSharedKey, p_PASS);
    config.securityType = WIFI_SEC_TYPE_PSK;
    config.band = HOTSPOT_BAND_TYPE_2G;
    config.channelNum = 7;

    error = SetHotspotConfig(&config);
    if (error != WIFI_SUCCESS) {
        printf("SetHotspotConfig failed, error = %d.\r\n", error);
        return -1;
    }
    //printf("SetHotspotConfig succeed!\r\n");

    //启动wifi热点模式
    error = EnableHotspot(); 
    if (error != WIFI_SUCCESS) {
        printf("EnableHotspot failed, error = %d.\r\n", error);
        return -1;
    }
    //printf("Enable Hotspot succeed!\r\n");

    //检查热点模式是否使能
    if (IsHotspotActive() == WIFI_HOTSPOT_NOT_ACTIVE) {
        printf("Wifi Hotspot is not actived.\r\n");
        return -1;
    }

    while (!g_hotspotStarted) {
        osDelay(10);
    }

    //printf("Wifi Hotspot is actived!\r\n");

    //启动dhcp
    g_lwip_netif = netifapi_netif_find("ap0");
    if( g_lwip_netif ) {
    
        ip4_addr_t bp_gw;
        ip4_addr_t bp_ipaddr;
        ip4_addr_t bp_netmask;

        IP4_ADDR(&bp_gw, 192, 168, 1, 1);           /* input your gateway for example: 192.168.1.1 */
        IP4_ADDR(&bp_ipaddr, 192, 168, 1, 1);       /* input your IP for example: 192.168.1.1 */
        IP4_ADDR(&bp_netmask, 255, 255, 255, 0);    /* input your netmask for example: 255.255.255.0 */

        err_t ret = netifapi_netif_set_addr(g_lwip_netif, &bp_ipaddr, &bp_netmask, &bp_gw);
        if(ret != ERR_OK) {
            printf("netifapi_netif_set_addr failed, error = %d.\r\n", ret);
            return -1;
        }
        //printf("netifapi_netif_set_addr succeed!\r\n");

        ret = netifapi_dhcps_start(g_lwip_netif, 0, 0);
        if(ret != ERR_OK) { 
            printf("netifapi_dhcp_start failed, error = %d.\r\n", ret);
            return -1;
        }
        printf("netifapi_dhcps_start succeed!\r\n");
    }
    else {
        printf("netifapi_netif_find ERROR\r\n");
    }
    /*
    //****************以下为UDP服务器代码***************
	//在sock_fd 进行监听
    int sock_fd;
    //服务端地址信息
	struct sockaddr_in server_sock;

    //创建socket
	if ((sock_fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("socket is error.\r\n");
		return -1;
	}

	bzero(&server_sock, sizeof(server_sock));
	server_sock.sin_family = AF_INET;
	server_sock.sin_addr.s_addr = htonl(INADDR_ANY);
	server_sock.sin_port = htons(8888);

	//调用bind函数绑定socket和地址
	if (bind(sock_fd, (struct sockaddr *)&server_sock, sizeof(struct sockaddr)) == -1) {
		perror("bind is error.\r\n");
		return -1;
	}

    int ret;
    char recvBuf[512] = {0};
    //客户端地址信息
    struct sockaddr_in client_addr;
    int size_client_addr= sizeof(struct sockaddr_in);
    */
    /*
    while (1)
    {
        printf("Waiting to receive data...\r\n");
        memset(recvBuf, 0, sizeof(recvBuf));
        ret = recvfrom(sock_fd, recvBuf, sizeof(recvBuf), 0, (struct sockaddr*)&client_addr,(socklen_t*)&size_client_addr);
        if(ret < 0)
        {
            printf("UDP server receive failed!\r\n");
            return -1;
        }
        printf("receive %d bytes of data from ipaddr = %s, port = %d.\r\n", ret, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
        printf("data is %s\r\n",recvBuf);
        ret = sendto(sock_fd, recvBuf, strlen(recvBuf), 0, (struct sockaddr *)&client_addr, sizeof(client_addr));
        if (ret < 0)
        {
            printf("UDP server send failed!\r\n");
            return -1;
        }
    }
    */
}

static void OnHotspotStaJoinHandler(StationInfo *info)
{
    if (info == NULL) {
        printf("HotspotStaJoin:info is null.\r\n");
    } 
    else {
        printf("New Sta Join\n");
    }
}

static void OnHotspotStaLeaveHandler(StationInfo *info)
{
    if (info == NULL) {
        printf("HotspotStaLeave:info is null.\r\n");
    } 
    else {
        static char macAddress[32] = {0};
        unsigned char* mac = info->macAddress;
        snprintf(macAddress, sizeof(macAddress), "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
        printf("HotspotStaLeave: macAddress=%s, reason=%d.\r\n", macAddress, info->disconnectedReason);
    }
}

static void OnHotspotStateChangedHandler(int state)
{
    printf("HotspotStateChanged:state is %d.\r\n", state);
    if (state == WIFI_HOTSPOT_ACTIVE) {
        g_hotspotStarted = 1;
        printf("wifi hotspot active.\r\n");
    } else {
        g_hotspotStarted = 0;
        printf("wifi hotspot noactive.\r\n");
    }
}

//============================================================================================
int32 StrLength( uint8 *p )
{
	int32 n = 0;
	while( *p != 0 ) {
		p++;
		n++;
	}
	return n;
}
int32 StrCopy( uint8 *ps, uint8 *pd )
{
	int n = 0;
	while( *ps != 0 ) {
		*pd = *ps;
		ps++;
		pd++;
		n++;
	}
	*pd = 0;
	return n;
}

bool wifiSTA_isConnected;
bool wifiSTA_enable;

void wifiSTA_Init(void)
{
	wifiSTA_isConnected = false;
	wifiSTA_enable = false;
}

void wifi_init_sta(uint8 *p_SSID, uint8 *p_PASS)
{
}

bool wifiAP_isConnected;

void wifiAP_Init(void)
{
	wifiAP_isConnected = false;
}

void wifi_init_softap(uint8 *p_SSID, uint8 *p_PASS)
{
    bool b = WifiAPTask( p_SSID, p_PASS );
}
//-----------------------------------------------------------------
bool TCP_isConnected;
int32 TCP_RLength;

void Main_TCP_Init( uint32 buf, int32 n )
{
}
void Main_StartTCP( int32 port )
{
}
void TCP_Server_SendString(uint8 *p )
{
}

bool TCP_Client_isConnected;
int32 TCP_Client_RLength;

void Main_TCP_Client_Init( uint32 buf, int32 n )
{
}
void Main_TCP_Client_Connect( uint32 buf, int32 port )
{
}
void TCP_Client_SendString(uint8 *p )
{
}
//------------------------------------------------------------------------------------------------------------------

#define LED_TASK_STACK_SIZE 1024 * 8
#define LED_TASK_PRIO 25

#define UART_BUFF_SIZE 1000
uint8_t uart_buff[UART_BUFF_SIZE];

int32 wdt_sleep_tick = 0;
static void Main_Deal()
{
        //注意升级hos时候一定要修改 uart.c里边的读函数, len==0直接返回, 或者后续研究下超时读取
        //hi_uart_is_buf_empty()有个bug, 就是第一次始终为非空, 但是read函数得到len==0, 导致等待!!!
        bool isEmpty = true; //尽量设置初始值为 true (参阅源码, 正规应该判断返回值是否为 SUCCESS)

        if( remo_USART_isOpen( 0 ) ) {
            isEmpty = true;
            uint32_t ret = hi_uart_is_buf_empty( HI_UART_IDX_0, &isEmpty );
            if( ret == HI_ERR_SUCCESS && !isEmpty ) {
                int n = hi_uart_read( HI_UART_IDX_0, uart_buff, UART_BUFF_SIZE );
                if( n != 0 ) {
                    for( uint8 i = 0; i < n; ++i ) {
                        Serial_Data = uart_buff[i];
                        Deal( Serial_Data );
	                    UART_List_Receive0();
                    }
                }
            }
        }
        if( remo_USART_isOpen( 1 ) ) {
            isEmpty = true;
            uint32_t ret = hi_uart_is_buf_empty( HI_UART_IDX_1, &isEmpty );
            if( ret == HI_ERR_SUCCESS && !isEmpty ) {
                int n = hi_uart_read( HI_UART_IDX_1, uart_buff, UART_BUFF_SIZE );
                if( n != 0 ) {
                    for( uint8 i = 0; i < n; ++i ) {
                        Serial_Data = uart_buff[i];
                        //Deal( Serial_Data );
	                    UART_List_Receive1();
                    }
                }
            }
        }
        if( remo_USART_isOpen( 2 ) ) {
            isEmpty = true;
            uint32_t ret = hi_uart_is_buf_empty( HI_UART_IDX_2, &isEmpty );
            if( ret == HI_ERR_SUCCESS && !isEmpty ) {
                int n = hi_uart_read( HI_UART_IDX_2, uart_buff, UART_BUFF_SIZE );
                if( n != 0 ) {
                    for( uint8 i = 0; i < n; ++i ) {
                        Serial_Data = uart_buff[i];
                        //Deal( Serial_Data );
	                    UART_List_Receive2();
                    }
                }
            }
        }
        
        //这里需要持续调用 hi_watchdog_feed或者usleep 否则30秒之后系统wdt重启, 20为大约0.6秒执行一次
        wdt_sleep_tick++;
        if( wdt_sleep_tick > 20 ) {
            wdt_sleep_tick = 0;
            hi_watchdog_feed();
        }
}

static void LedExampleEntry(void)
{
/*
    osThreadAttr_t attr;
    attr.name = "LedTask";
    attr.attr_bits = 0U;
    attr.cb_mem = NULL;
    attr.cb_size = 0U;
    attr.stack_mem = NULL;
    attr.stack_size = LED_TASK_STACK_SIZE;
    attr.priority = LED_TASK_PRIO;

    if (osThreadNew((osThreadFunc_t)LedTask, NULL, &attr) == NULL) {
        printf("[LedExample] Falied to create LedTask!\n");
    }
*/

    //不知道干嘛用的
    //(void)arg;

    //bool b = WifiAPTask();

    wifiSTA_Init();
    wifiAP_Init();

    setup0();

    //等待一秒钟 带有命令扫描
    for(int32 nCount = 0; nCount < 100000; nCount++) {
		SoftDelay_10us( 1 );
    	Main_Deal();
    }
	//如果在延时期间收到了程序下载事件, 则等待下载完再继续执行
	while (!Running) {
        Main_Deal();
    }
    while (1) {
        
        //IoTGpioSetOutputVal(LED_TEST_GPIO, 1);
        //SoftDelay_ms( 500 );
        //IoTGpioSetOutputVal(LED_TEST_GPIO, 0);
        //SoftDelay_ms( 500 );

        for( int32 i = 0; i < 1000; ++i ) {
			loop();
		}
        Main_Deal();
    }
}

SYS_RUN(LedExampleEntry);
