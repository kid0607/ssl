/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *-pipe -fno-strict-aliasing -Wall -Wstrict-prototypes -Wmissing-prototypes -Werror-implicit-function-declaration -Wpointer-arith -std=gnu99 -ffunction-sections -fdata-sections -Wchar-subscripts -Wcomment -Wformat=2 -Wimplicit-int -Wmain -Wparentheses -Wsequence-point -Wreturn-type -Wswitch -Wtrigraphs -Wunused -Wuninitialized -Wunknown-pragmas -Wfloat-equal -Wundef -Wshadow -Wbad-function-cast -Wwrite-strings -Wsign-compare -Waggregate-return  -Wmissing-declarations -Wformat -Wmissing-format-attribute -Wno-deprecated-declarations -Wpacked -Wredundant-decls -Wnested-externs -Wlong-long -Wunreachable-code -Wcast-align --param max-inline-insns-single=500
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
#include <hsf.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <cyassl/openssl/ssl.h>
#include <cyassl/internal.h>
#include <cyassl/cyassl_config.h>
#include "upnptest.h"
#include "user_atcmd.h"

#define UART_BUF_SIZE 600


char upload[UART_BUF_SIZE] = {0};  //仅用于接收
char upload_handle[UART_BUF_SIZE] = {0}; //双buffer，用于实际处理
int buffer_avaliable_flag = 0;
int tcp_uartfd = 0;
int wifi_connected_flag = 0;
extern char room_type[30];
extern char room_name[30];
int   Sync_Flag = 0;
extern CYASSL*         ssl; 

#ifdef __LPT100__
static int module_type= HFM_TYPE_LPT100;
const int hf_gpio_fid_to_pid_map_table[HFM_MAX_FUNC_CODE]=
{
	HF_M_PIN(2),	//HFGPIO_F_JTAG_TCK
	HF_M_PIN(3),	//HFGPIO_F_JTAG_TDO
	HF_M_PIN(4),	//HFGPIO_F_JTAG_TDI
	HF_M_PIN(5),	//HFGPIO_F_JTAG_TMS
	HFM_NOPIN,		//HFGPIO_F_USBDP
	HFM_NOPIN,		//HFGPIO_F_USBDM
	HF_M_PIN(39),	//HFGPIO_F_UART0_TX
	HF_M_PIN(40),	//HFGPIO_F_UART0_RTS
	HF_M_PIN(41),	//HFGPIO_F_UART0_RX
	HF_M_PIN(42),	//HFGPIO_F_UART0_CTS
	
	HF_M_PIN(27),	//HFGPIO_F_SPI_MISO
	HF_M_PIN(28),	//HFGPIO_F_SPI_CLK
	HF_M_PIN(29),	//HFGPIO_F_SPI_CS
	HF_M_PIN(30),	//HFGPIO_F_SPI_MOSI
	
	HFM_NOPIN,	//HFGPIO_F_UART1_TX,
	HFM_NOPIN,	//HFGPIO_F_UART1_RTS,
	HFM_NOPIN,	//HFGPIO_F_UART1_RX,
	HFM_NOPIN,	//HFGPIO_F_UART1_CTS,
	
	HF_M_PIN(11),	//HFGPIO_F_NLINK
	HF_M_PIN(12),	//HFGPIO_F_NREADY
	HF_M_PIN(45),	//HFGPIO_F_NRELOAD
	HFM_NOPIN,	//HFGPIO_F_SLEEP_RQ
	HFM_NOPIN,	//HFGPIO_F_SLEEP_ON
	
	HF_M_PIN(18),	//HFGPIO_F_WPS
	HFM_NOPIN,		//HFGPIO_F_RESERVE1
	HFM_NOPIN,		//HFGPIO_F_RESERVE2
	HFM_NOPIN,		//HFGPIO_F_RESERVE3
	HFM_NOPIN,		//HFGPIO_F_RESERVE4
	HFM_NOPIN,		//HFGPIO_F_RESERVE5
	
	HFM_NOPIN,	//HFGPIO_F_USER_DEFINE
};
#elif defined(__LPB105__)
static int module_type= HFM_TYPE_LPB105;
const int hf_gpio_fid_to_pid_map_table[HFM_MAX_FUNC_CODE]=
{
	HF_M_PIN(2),	//HFGPIO_F_JTAG_TCK
	HF_M_PIN(3),	//HFGPIO_F_JTAG_TDO
	HF_M_PIN(4),	//HFGPIO_F_JTAG_TDI
	HF_M_PIN(5),	//HFGPIO_F_JTAG_TMS
	HFM_NOPIN,		//HFGPIO_F_USBDP
	HFM_NOPIN,		//HFGPIO_F_USBDM
	HF_M_PIN(39),	//HFGPIO_F_UART0_TX
	HFM_NOPIN,	//HFGPIO_F_UART0_RTS
	HF_M_PIN(41),	//HFGPIO_F_UART0_RX
	HFM_NOPIN,	//HFGPIO_F_UART0_CTS
	
	HF_M_PIN(27),	//HFGPIO_F_SPI_MISO
	HF_M_PIN(28),	//HFGPIO_F_SPI_CLK
	HF_M_PIN(29),	//HFGPIO_F_SPI_CS
	HF_M_PIN(30),	//HFGPIO_F_SPI_MOSI
	
	HFM_NOPIN,	//HFGPIO_F_UART1_TX,
	HFM_NOPIN,	//HFGPIO_F_UART1_RTS,
	HFM_NOPIN,	//HFGPIO_F_UART1_RX,
	HFM_NOPIN,	//HFGPIO_F_UART1_CTS,
	
	HF_M_PIN(11),	//HFGPIO_F_NLINK
	HF_M_PIN(44),	//HFGPIO_F_NREADY
	HF_M_PIN(45),	//HFGPIO_F_NRELOAD
	HFM_NOPIN,	//HFGPIO_F_SLEEP_RQ
	HFM_NOPIN,	//HFGPIO_F_SLEEP_ON
	
	HF_M_PIN(18),	//HFGPIO_F_WPS
	HFM_NOPIN,		//HFGPIO_F_RESERVE1
	HFM_NOPIN,		//HFGPIO_F_RESERVE2
	HFM_NOPIN,		//HFGPIO_F_RESERVE3
	HFM_NOPIN,		//HFGPIO_F_RESERVE4
	HFM_NOPIN,		//HFGPIO_F_RESERVE5
	
	HFM_NOPIN,	//HFGPIO_F_USER_DEFINE
};
#elif defined(__LPT200__)
static int module_type= HFM_TYPE_LPT200;
const int hf_gpio_fid_to_pid_map_table[HFM_MAX_FUNC_CODE]=
{
	HF_M_PIN(2),	//HFGPIO_F_JTAG_TCK
	HF_M_PIN(3),	//HFGPIO_F_JTAG_TDO
	HF_M_PIN(4),	//HFGPIO_F_JTAG_TDI
	HF_M_PIN(5),	//HFGPIO_F_JTAG_TMS
	HFM_NOPIN,		//HFGPIO_F_USBDP
	HFM_NOPIN,		//HFGPIO_F_USBDM
	HF_M_PIN(39),	//HFGPIO_F_UART0_TX
	HF_M_PIN(40),	//HFGPIO_F_UART0_RTS
	HF_M_PIN(41),	//HFGPIO_F_UART0_RX
	HF_M_PIN(42),	//HFGPIO_F_UART0_CTS
	
	HF_M_PIN(27),	//HFGPIO_F_SPI_MISO
	HF_M_PIN(28),	//HFGPIO_F_SPI_CLK
	HF_M_PIN(29),	//HFGPIO_F_SPI_CS
	HF_M_PIN(30),	//HFGPIO_F_SPI_MOSI
	
	HFM_NOPIN,	//HFGPIO_F_UART1_TX,
	HFM_NOPIN,	//HFGPIO_F_UART1_RTS,
	HFM_NOPIN,	//HFGPIO_F_UART1_RX,
	HFM_NOPIN,	//HFGPIO_F_UART1_CTS,
	
	HF_M_PIN(43),	//HFGPIO_F_NLINK
	HF_M_PIN(44),	//HFGPIO_F_NREADY
	HF_M_PIN(45),	//HFGPIO_F_NRELOAD
	HFM_NOPIN,	//HFGPIO_F_SLEEP_RQ
	HFM_NOPIN,	//HFGPIO_F_SLEEP_ON
		
	HF_M_PIN(7),		//HFGPIO_F_WPS
	HFM_NOPIN,		//HFGPIO_F_RESERVE1
	HFM_NOPIN,		//HFGPIO_F_RESERVE2
	HFM_NOPIN,		//HFGPIO_F_RESERVE3
	HFM_NOPIN,		//HFGPIO_F_RESERVE4
	HFM_NOPIN,		//HFGPIO_F_RESERVE5
	
	HFM_NOPIN,	//HFGPIO_F_USER_DEFINE
};
#elif defined(__LPB100__)
static int module_type= HFM_TYPE_LPB100;
const int hf_gpio_fid_to_pid_map_table[HFM_MAX_FUNC_CODE]=
{
	HF_M_PIN(2),	//HFGPIO_F_JTAG_TCK
	HFM_NOPIN,	//HFGPIO_F_JTAG_TDO
	HFM_NOPIN,	//HFGPIO_F_JTAG_TDI
	HF_M_PIN(5),	//HFGPIO_F_JTAG_TMS
	HFM_NOPIN,		//HFGPIO_F_USBDP
	HFM_NOPIN,		//HFGPIO_F_USBDM
	HF_M_PIN(39),	//HFGPIO_F_UART0_TX
	HF_M_PIN(40),	//HFGPIO_F_UART0_RTS
	HF_M_PIN(41),	//HFGPIO_F_UART0_RX
	HF_M_PIN(42),	//HFGPIO_F_UART0_CTS
	
	HF_M_PIN(27),	//HFGPIO_F_SPI_MISO
	HF_M_PIN(28),	//HFGPIO_F_SPI_CLK
	HF_M_PIN(29),	//HFGPIO_F_SPI_CS
	HF_M_PIN(30),	//HFGPIO_F_SPI_MOSI
	
	HF_M_PIN(23),	//HFGPIO_F_UART1_TX,
	HFM_NOPIN,	//HFGPIO_F_UART1_RTS,
	HF_M_PIN(8),	//HFGPIO_F_UART1_RX,
	HFM_NOPIN,	//HFGPIO_F_UART1_CTS,
	
	HF_M_PIN(43),	//HFGPIO_F_NLINK
	HF_M_PIN(44),	//HFGPIO_F_NREADY
	HF_M_PIN(45),	//HFGPIO_F_NRELOAD
	HF_M_PIN(7),	//HFGPIO_F_SLEEP_RQ
	HFM_NOPIN,	//HFGPIO_F_SLEEP_ON
		
	HF_M_PIN(15),		//HFGPIO_F_WPS
	HFM_NOPIN,		//HFGPIO_F_RESERVE1
	HFM_NOPIN,		//HFGPIO_F_RESERVE2
	HFM_NOPIN,		//HFGPIO_F_RESERVE3
	HFM_NOPIN,		//HFGPIO_F_RESERVE4
	HFM_NOPIN,		//HFGPIO_F_RESERVE5
	
	HFM_NOPIN,	//HFGPIO_F_USER_DEFINE
};
#elif defined(__LPB100U__)
static int module_type= HFM_TYPE_LPB100;
const int hf_gpio_fid_to_pid_map_table[HFM_MAX_FUNC_CODE]=
{
	HF_M_PIN(2),	//HFGPIO_F_JTAG_TCK
	HFM_NOPIN,	//HFGPIO_F_JTAG_TDO
	HFM_NOPIN,	//HFGPIO_F_JTAG_TDI
	HF_M_PIN(5),	//HFGPIO_F_JTAG_TMS
	HFM_NOPIN,		//HFGPIO_F_USBDP
	HFM_NOPIN,		//HFGPIO_F_USBDM
	HF_M_PIN(39),	//HFGPIO_F_UART0_TX
	HFM_NOPIN,//HF_M_PIN(40),	//HFGPIO_F_UART0_RTS
	HF_M_PIN(41),	//HFGPIO_F_UART0_RX
	HFM_NOPIN,//HF_M_PIN(42),	//HFGPIO_F_UART0_CTS
	
	HFM_NOPIN,//HF_M_PIN(27),	//HFGPIO_F_SPI_MISO
	HFM_NOPIN,//HF_M_PIN(28),	//HFGPIO_F_SPI_CLK
	HFM_NOPIN,//HF_M_PIN(29),	//HFGPIO_F_SPI_CS
	HFM_NOPIN,//HF_M_PIN(30),	//HFGPIO_F_SPI_MOSI
	
	HF_M_PIN(29),	//HFGPIO_F_UART1_TX,
	HFM_NOPIN,	//HFGPIO_F_UART1_RTS,
	HF_M_PIN(30),	//HFGPIO_F_UART1_RX,
	HFM_NOPIN,	//HFGPIO_F_UART1_CTS,	
	
	HF_M_PIN(43),	//HFGPIO_F_NLINK
	HF_M_PIN(44),	//HFGPIO_F_NREADY
	HF_M_PIN(45),	//HFGPIO_F_NRELOAD
	HFM_NOPIN,//HF_M_PIN(7),	//HFGPIO_F_SLEEP_RQ
	HFM_NOPIN,//HF_M_PIN(8),	//HFGPIO_F_SLEEP_ON
		
	HF_M_PIN(15),		//HFGPIO_F_WPS
	HFM_NOPIN,		//HFGPIO_F_RESERVE1
	HFM_NOPIN,		//HFGPIO_F_RESERVE2
	HFM_NOPIN,		//HFGPIO_F_RESERVE3
	HFM_NOPIN,		//HFGPIO_F_RESERVE4
	HFM_NOPIN,		//HFGPIO_F_RESERVE5
	
	HF_M_PIN(7),	//HFGPIO_F_USER_DEFINE
	HF_M_PIN(8),	//HFGPIO_F_USER_DEFINE+1	
};
#else
#error "invalid project !you must define module type(__LPB100__,__LPT100__,_LPT200__)"
#endif

const hfat_cmd_t user_define_at_cmds_table[]=
{
	{"SSLNETP",use_cmd_rsslnetp,"AT+SSLNETP\r\n",NULL} ,//the last item must be null
	{"SSLLK",user_cmd_deal_rssllk,"AT+SSLLK\r\n",NULL} ,//the last item must be null
	{NULL,NULL,NULL,NULL} //the last item must be null
};

/*static int USER_FUNC socketa_recv_callback(uint32_t event,char *data,uint32_t len,uint32_t buf_len)
{
	if(event==HFNET_SOCKETA_DATA_READY)
	{
		HF_Debug(DEBUG_LEVEL_LOW,"socketa recv %d bytes %d\n",len,buf_len);
	}
	else if(event==HFNET_SOCKETA_CONNECTED)
		u_printf("socket a connected!\n");
	else if(event==HFNET_SOCKETA_DISCONNECTED)
		u_printf("socket a disconnected!\n");
	
	return len;
}

static int USER_FUNC socketb_recv_callback(uint32_t event,char *data,uint32_t len,uint32_t buf_len)
{
	if(event==HFNET_SOCKETB_DATA_READY)
		HF_Debug(DEBUG_LEVEL_LOW,"socketb recv %d bytes %d\n",len,buf_len);
	else if(event==HFNET_SOCKETB_CONNECTED)
		u_printf("socket b connected!\n");
	else if(event==HFNET_SOCKETB_DISCONNECTED)
		u_printf("socket b disconnected!\n");
			
	return len;
}*/

char hfwifi_is_connected_flag=0;
static int hfsys_event_callback( uint32_t event_id,void * param)
{
	switch(event_id)
	{
		case HFE_WIFI_STA_CONNECTED:
			break;
			
		case HFE_WIFI_STA_DISCONNECTED:
			hfwifi_is_connected_flag=0;
			break;

		case HFE_DHCP_OK:
			hfwifi_is_connected_flag=1;
			break;
			
		default:
			break;
	}
	return 0;
}

static void show_reset_reason(void)
{
	uint32_t reset_reason=0;
	
	reset_reason = hfsys_get_reset_reason();
	
	
#if 1
	u_printf("reset_reasion:%08x\n",reset_reason);
#else	
	if(reset_reason&HFSYS_RESET_REASON_ERESET)
	{
		u_printf("ERESET\n");
	}
	if(reset_reason&HFSYS_RESET_REASON_IRESET0)
	{
		u_printf("IRESET0\n");
	}
	if(reset_reason&HFSYS_RESET_REASON_IRESET1)
	{
		u_printf("IRESET1\n");
	}
	if(reset_reason==HFSYS_RESET_REASON_NORMAL)
	{
		u_printf("RESET NORMAL\n");
	}
	if(reset_reason&HFSYS_RESET_REASON_WPS)
	{
		u_printf("RESET FOR WPS\n");
	}
	if(reset_reason&HFSYS_RESET_REASON_SMARTLINK_START)
	{
		u_printf("RESET FOR SMARTLINK START\n");
	}
	if(reset_reason&HFSYS_RESET_REASON_SMARTLINK_OK)
	{
		u_printf("RESET FOR SMARTLINK OK\n");
	}
	if(reset_reason&HFSYS_RESET_REASON_WPS_OK)
	{
		u_printf("RESET FOR WPS OK\n");
	}
#endif
	
	return;
}

void app_init(void)
{
	u_printf("app_init\n");
}

static int USER_FUNC uart_recv_callback(uint32_t event,char *data,uint32_t len,uint32_t buf_len)
{

	char *tmpBuf = upload; //用于接收
	char *pBuf = upload_handle;//用于处理
	int i = 0;
	char *p;
	int cnt = 0;
	int start_addr = 0;

	memset(tmpBuf,0,UART_BUF_SIZE);		 //此时如果接收buffer中的数据还没有被copy到处理buffer，只能被冲掉
	memcpy(tmpBuf,data,len);
	
	while(buffer_avaliable_flag != 0)  //不空闲，接收buffer中的数据等待被copy到处理buffer
	{
		msleep(20);				
	}
	
		
		memcpy(pBuf,tmpBuf,len);
		buffer_avaliable_flag = 1;   //还没处理完
		
		if(wifi_connected_flag == 1)
		{
			u_printf("MCU to FW,RAW,len %d:\r\n %s\r\n\r\n",len,pBuf);
			if(strstr(pBuf,"LiteSync"))
			{

				p = strtok(pBuf,"@");
				
					do
				{
					u_printf("MCU to FW,Sync:\r\n %s\r\n\r\n",p);
					if(i == 1)
					{
						strcpy(room_type,p);
					}
					else if(i == 2)
					{
						strcpy(room_name,p);
					}

					i++;
				}
				while((p = strtok(NULL, "@")) != NULL);
				
				if(i == 1)
				{
					strcpy(room_type,"baby room");		
					strcpy(room_name,"FLEX");		
				}
				
				i = 0;
				Sync_Flag = 1;			
			}
			else
			{
				if(pBuf[0]=='A' && pBuf[1]=='T' && pBuf[2] == '+')
				{
					//no need to send AT command to either SSL server or other local devices 
				}
				else
				{
					
	//解析数据源  from local device or lucis server	

					if((p = strstr(pBuf,"EVENTSOURCE"))!= NULL)
					{
						if((p[12]=='S')&&(p[13]=='S')&&(p[14]=='L'))
						{
						//	u_printf("SSL SEND!!!!!!!!!!\r\n");
							
							start_addr = &p[0] - &pBuf[0]; //words before "EVENTSOURCE" field  
							
							
							if(user_ssl_par.connect_flag == 1)
							{
	//  remove "EVENTSOURCE" field before real network transport							
								for(cnt=(start_addr);cnt<(strlen(pBuf)-17);cnt++)
								{
									pBuf[cnt] = pBuf[cnt+17];
								}
								pBuf[cnt] = '\0';
								
								u_printf("MCU to FW,SSL:\r\n %s\r\n\r\n",pBuf);
							
								if (CyaSSL_write(ssl, pBuf, strlen(pBuf)) != strlen(pBuf))// CyaSSL_write
								{
									HF_Debug(10,"SSL_write failed");
								}

							}	
						}
						else if((p[12]=='T')&&(p[13]=='C')&&(p[14]=='P'))
						{
						//	u_printf("TCP SEND!!!!!!!!!!\r\n");			

	//  remove "EVENTSOURCE" field before real network transport
							start_addr = &p[0] - &pBuf[0];  
							for(cnt=(start_addr);cnt<(strlen(pBuf)-17);cnt++)
							{
								pBuf[cnt] = pBuf[cnt+17];
							}
							pBuf[cnt] = '\0';						
							u_printf("MCU to FW,TCP,fd is %d:\r\n %s\r\n\r\n",tcp_uartfd,pBuf);
							
							
							write(tcp_uartfd,pBuf,strlen(pBuf));    //组播  tcp 发送		  //此处的len 应该使用去除EVENTSOURCE处后的
						}
								
					}				
				}
			
			}
			memset(pBuf,0,UART_BUF_SIZE);
			buffer_avaliable_flag = 0; //已经处理完，buf空闲，可以处理下一条消息			
		//	u_printf("tcp fd %d\r\n",tcp_uartfd);
			
			//tcp_uartfd = 0;
		}
	
	return len;
}
int USER_FUNC app_main (void)
{
	//time_t now=time(NULL);

	
	HF_Debug(DEBUG_LEVEL,"sdk version(%s),the app_main start time is %s %s\n",hfsys_get_sdk_version(),__DATE__,__TIME__);
	if(hfgpio_fmap_check(module_type)!=0)
	{
		while(1)
		{
			HF_Debug(DEBUG_ERROR,"gpio map file error\n");
			msleep(1000);
		}
		//return 0;
	}
	show_reset_reason();

	
	while(!hfnet_wifi_is_active())
	{
		msleep(50);
	}
	#if 0
	int up_result=0;
	up_result = hfupdate_auto_upgrade();
	if(up_result<0)
	{
		u_printf("no entry the auto upgrade mode\n");
	}
	else if(up_result==0)
	{
		u_printf("upgrade success\n");
	}
	else
	{
		u_printf("upgrade fail %d\n",up_result);
	}
	#endif

	user_init_atcmd_para();
	if(hfsys_register_system_event((hfsys_event_callback_t)hfsys_event_callback)!=HF_SUCCESS)
	{
		HF_Debug(DEBUG_ERROR,"register system event fail\n");
	}
	
	/*if(hfnet_start_assis(ASSIS_PORT)!=HF_SUCCESS)
	{
		HF_Debug(DEBUG_WARN,"start httpd fail\n");
	}*/
	
	if(hfnet_start_uart(HFTHREAD_PRIORITIES_LOW,(hfnet_callback_t)uart_recv_callback)!=HF_SUCCESS)
	{
		HF_Debug(DEBUG_WARN,"start uart fail!\n");
	}	
	
	/*delete Just for more RAM
	if(hfnet_start_socketa(HFTHREAD_PRIORITIES_LOW,(hfnet_callback_t)socketa_recv_callback)!=HF_SUCCESS)
	{
		HF_Debug(DEBUG_WARN,"start socketa fail\n");
	}
	if(hfnet_start_socketb(HFTHREAD_PRIORITIES_LOW,(hfnet_callback_t)socketb_recv_callback)!=HF_SUCCESS)
	{
		HF_Debug(DEBUG_WARN,"start socketb fail\n");
	}*/
	if(hfnet_start_httpd(HFTHREAD_PRIORITIES_MID)!=HF_SUCCESS)
	{
		HF_Debug(DEBUG_WARN,"start httpd fail\n");
	}

	extern void SSL_test_thread(void *arg);
	while(hfwifi_is_connected_flag != 1)
	{
		msleep(3000);
	}
	
wifi_connected_flag = 1;
	
#if 0
	if(hfthread_create(SSL_test_thread, "SSL_test", 1024, NULL, HFTHREAD_PRIORITIES_LOW,NULL,NULL)!= HF_SUCCESS)
	{
		HF_Debug(DEBUG_ERROR,"start SSL_test_thread fail\n");
	}
#endif
#if 1
	if(hfthread_create(User_cmd_ssl, "User_ssl", 1024, NULL, HFTHREAD_PRIORITIES_LOW,NULL,NULL)!= HF_SUCCESS)
	{
		HF_Debug(DEBUG_ERROR,"start User_ssl_thread fail\n");
	}
#endif
	hfthread_create(upnp_main_thread,"upnp_main",256,(void*)1,1,NULL,NULL);
	hfthread_create(Muticast_send,"Muticast_send",256,(void*)1,1,NULL,NULL);

	
	return 1;
}

