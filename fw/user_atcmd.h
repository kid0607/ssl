
#include <hsf.h>
#include <stdlib.h>
#include <string.h>


#define USER_ATCMD_PARA_ADDR			0X00000000
#define USER_CERTIFICATE1_ADDR			0X00001000    //第2页   
#define USER_CERTIFICATE2_ADDR			0X00002000    //第3页  
#define USER_CERTIFICATE3_ADDR			0X00003000    //第4页  

#define CURRENT_CERTIFICATE_TOTAL_SIZE 5542   //少两个结束符
#define CURRENT_CERTIFICATE1_SIZE 1500
#define CURRENT_CERTIFICATE2_SIZE 1924
#define CURRENT_CERTIFICATE3_SIZE 2120

#pragma pack(push, 1)
typedef struct {
	//SSL server port
	unsigned short ssl_port;
	//SSL server addr
	char ssl_addr[100];
	char usable_flag;//0-parameter unusable	1-parameter usable	
	char connect_flag;//0-is disconnected	1-is connected
}user_ssl;    //占据了118个字节
#pragma pack(pop)
enum USERCMDOPCODE
{
	OPCODE_QUERY,
	OPCODE_SET,
	OPCODE_NONE
};

extern user_ssl user_ssl_par;



void user_init_atcmd_para(void);

void user_cmd_deal_rssllk(pat_session_t s,int argc,char *argv[],char *rsp,int len);

unsigned short user_power(int len);

void use_cmd_rsslnetp(pat_session_t s,int argc,char *argv[],char *rsp,int len);

void user_set_local_time(void);

int user_seuaddressis_ip(const char * ipaddr);

int user_tcp_connect_ssl_server(char *url,unsigned short port);

void user_cmd_ssl(void);

void User_cmd_ssl(void);


