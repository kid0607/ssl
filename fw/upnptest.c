
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
#include <stdio.h>
#include <httpc/httpc.h>
#include "upnptest.h"
//#include "../example.h"
#include "hfmsgq.h"

//#if (EXAMPLE_USE_DEMO==USER_UPNP_DEMO)

#define SSDP_PORT				(1900)
#define SSDP_ADDR				("239.255.255.250")
#define MAXCLIENT 10

int send_fd;
struct sockaddr_in send_addr;  
int listenfd,connfd,sockfd;	
int nready,maxi,client[MAXCLIENT];
char tcpserver_buf[400];
char tcpserver_encodedbuf[400];
struct sockaddr_in servaddr,cliaddr;
socklen_t clilen;
int recv_num = 0;
extern int tcp_uartfd;
char local_ip[20] = {0};
char room_type[30] = "baby room";
char room_name[30] = "FLEX";
char MAC[20] = {0};
int count = 0;
extern int Sync_Flag;
int tcp_send_finish = 1;


char *info="HTTP/1.1 202 Accepted\r\nDate: Mon,31 June 2014 22:01:45 GMT\r\nContent-type: application/json;charset=UTF-8\r\nContent-Length: 156\r\n\r\n{\"id\":\"jxtu0904-2013\",\"type\":\"baby room\",\"name\":\"fanjj house\",\"channel0cap\":[\"dimmer\",\"switch\"], \"channel1cap\":[\"switch\"],\"channelname\":[\"c1\",\"\",\"c3\",\"c4\"]}";	
char *channel="HTTP/1.1 200 OK\r\nDate: Mon,31 June 2014 22:01:45 GMT\r\nContent-type: application/json;charset=UTF-8\r\nContent-Length: 45\r\n\r\n{\"id\":\"jxtu0904-2013\",\"channel0\":[\"on\",\"50\"]}";




void TCP_Encode(void)
{
	char *p = NULL;
	int i = 0;
	int j=0;


	p = strstr(tcpserver_buf,"Date");
		
	if(p!=NULL)
	{
			
		i = &p[0] -  &tcpserver_buf[0]; //gross characters  before ¡°Date¡±
			
	
		for(j=0;j<i;j++)    //PartI:words before "Date"
		{
			tcpserver_encodedbuf[j] = tcpserver_buf[j];
		}		
		
		strncpy(&tcpserver_encodedbuf[i],"EVENTSOURCE:TCP\r\n",17); //PartII£ºadded identification
	
		strcpy(&tcpserver_encodedbuf[i+17],&tcpserver_buf[i]);

	}	
}


	USER_FUNC  void Muticast_send(void* arg)
{
    int  cnt;  
		struct ip_mreq mreq;     
		//char *message=buf;  
  
    /* create what looks like an ordinary UDP socket */  
   if ((send_fd=socket(AF_INET,SOCK_DGRAM,0)) < 0)   
   {  
 
    }  
 
   /* set up destination address */  
    memset(&send_addr,0,sizeof(send_addr));  
    send_addr.sin_family=AF_INET;  
    send_addr.sin_addr.s_addr=inet_addr(SSDP_ADDR);  
    send_addr.sin_port=htons(SSDP_PORT);  
  
		while(1)
		{
			
		}
		/*
        if (sendto(send_fd,message, strlen(message), 0, (struct sockaddr *) &send_addr, sizeof(send_addr)) < 0)   
        {    
								//u_printf("kim+++ send error\n");
        }  
				else
				{
					//u_printf("kim+++ succed\n\n");
				}
		*/

	}

static int get_mac_addr(void)
{
	char *words[6]={NULL};
	char rsp[64]={0};
	char is_ap_mode=0;

	if(hfat_send_cmd("AT+WMODE\r\n",sizeof("AT+WMODE\r\n"),rsp,64)!=0)
		return 0;

	if(hfat_get_words(rsp,words, 6)>0)
	{
		if(strcmp("STA",words[1])==0)
		{
			is_ap_mode=0;
		}
	}

	/* get mac as device id */
	//u_printf("kim:words start\r\n");
	if(is_ap_mode)
	{
		hfat_send_cmd("AT+WAMAC\r\n",sizeof("AT+WAMAC\r\n"),rsp,64);
	}
	else
		hfat_send_cmd("AT+WSMAC\r\n",sizeof("AT+WSMAC\r\n"),rsp,64);	
	
		if(hfat_get_words(rsp,words, 6)>0)
	{
			strcpy(MAC,words[1]);
	}		
}
	

static int get_if_ip_addr(void)
{
	char *words[6]={NULL};
	char rsp[64]={0};
	char is_ap_mode=0;

	if(hfat_send_cmd("AT+WMODE\r\n",sizeof("AT+WMODE\r\n"),rsp,64)!=0)
		return 0;

	if(hfat_get_words(rsp,words, 6)>0)
	{
		if(strcmp("STA",words[1])==0)
		{
			is_ap_mode=0;
		}
	}

	if(is_ap_mode)
	{
		hfat_send_cmd("AT+LANN\r\n",sizeof("AT+LANN\r\n"),rsp,64);
	}
	else
		hfat_send_cmd("AT+WANN\r\n",sizeof("AT+WANN\r\n"),rsp,64);
	
	if(hfat_get_words(rsp,words, 6)>0)
	{
		////u_printf("\nresult:%s\nmode:%s\nIP:%s\nMASK:%s\nGW:%s\n",words[0],words[1],words[2],words[3],words[4]);
				
		if(is_ap_mode)
		{
			 memcpy(local_ip,words[1],strlen(words[1]));
			 local_ip[strlen(words[1])] = '\0';
		}
		else
		{
			 memcpy(local_ip,words[2],strlen(words[2]));			
			 local_ip[strlen(words[2])] = '\0';
		}
		if(is_ap_mode)
			return inet_addr(words[1]);
		else
			return inet_addr(words[2]);
	}
	
	return 0;
}

static int send_packet(int fd, const void *data, size_t len,struct sockaddr_in *to)
{
	int slen=0;
	static struct sockaddr_in toaddr;

	//DEBUG_PRINTF("%s len=%d fd=%d %d\n",__FUNCTION__,len,fd,svr->sockfd);
	if (to==NULL)
	{
		memset(&toaddr, 0, sizeof(struct sockaddr_in));
		toaddr.sin_family = AF_INET;
		toaddr.sin_port = htons(SSDP_PORT);
		toaddr.sin_len = sizeof(struct sockaddr_in);
		toaddr.sin_addr.s_addr = inet_addr(SSDP_ADDR);
		to = &toaddr;
	}
	
	slen = sendto(fd, (const char*)data, len, 0, (struct sockaddr *) to, sizeof(struct sockaddr_in));
	if(slen<0)
		//u_printf("slen %d\n",slen);
	
	return slen;
}


static int send_msearch_packet(int fd)
{
	char *msearch="M-SEARCH * HTTP/1.1\r\nHOST: 239.255.255.250:1900\r\nMAN: \"ssdp:discover\"\r\nMX: 2\r\nST: LPB100\r\n\r\n";
	send_packet(fd,msearch,strlen(msearch),NULL);
	return 0;
}


static int send_alive_packet(int fd,char *pkt_buffer,int len,uint32_t if_addr)
{
	char *alivepkt="M-SEARCH * HTTP/1.1\r\nHOST: 239.255.255.250:1900\r\nST:urn:device:Lucis-tech Romeo:1\r\nMAN: \"ssdp:discover\"\r\nMX: 10\r\n";
	sprintf(pkt_buffer,alivepkt,if_addr&0xFF,(if_addr>>8)&0xff,(if_addr>>16)&0xff,(if_addr>>24)&0xff);
	send_packet(fd,alivepkt,strlen(alivepkt),NULL);
	return 0;
}


static int send_msearch_rsp_packet(struct sockaddr_in *to)
{
	
	int fd=-1;
	int send_num=0;
	int len;
	//char send[32]={0};
	uint8_t mac_addr[6]={0};
	int ret,maxfd;
	int uart_fd;
	fd_set rset;
	struct timeval timeout;
	struct sockaddr_in addr;
	int alen=sizeof(struct sockaddr_in);
	char ip[50] = {0};
	
	
	//char *rsp_pkt = "HTTP/1.1 200 OK\r\nExt:\r\nST:urn:device:Lucis-tech Romeo:1\r\nLOCATION:192.168.1.110:30001\r\nUSN:uuid:jxtu0904-2013:Dining room:Alysum 02\r\n\r\n"; //ÏÈ»Ø¸´¼òµ¥µÄÊý¾Ý±¨
	char p[200] = "HTTP/1.1 200 OK\r\nExt:\r\nST:urn:device:Lucis-tech Romeo:1\r\nLOCATION:";
	
	strcat(p,local_ip);   //FA´Ó·þÎñÆ÷»ñÈ¡
	//strcat(p,":30001\r\nUSN:uuid:jxtu0904-2013:Dining room:asylum 04\r\n\r\n");
	//strcat(p,":30001\r\nUSN:uuid:jxtu0904-2013:");
	
	strcat(p,":30001\r\nUSN:uuid:");
	strcat(p,MAC);
	//strcat(p,"Lite001");
	strcat(p,":lite:");	
	//strcat(p,":");		
	strcat(p,room_type);
	strcat(p,":");
	strcat(p,room_name);
	strcat(p,"\r\n\r\n");

	//u_printf("kim:%s\r\n",p);
	
	
	
	
	int tmp=1;
	
	memset((char*)&addr,0,sizeof(addr));
	
//	//u_printf("kim+++ udp back to  ip:%x\n",to->sin_addr.s_addr);
	
	addr.sin_family = AF_INET;
	addr.sin_port = htons(4566); 
	//addr.sin_addr.s_addr=inet_addr("192.168.1.139");	
	addr.sin_addr.s_addr= to->sin_addr.s_addr;
	
	fd = socket(AF_INET, SOCK_DGRAM, 0);
	if(fd<0)
		return -1;
	


	len = strlen(p);
	
	
	//hfuart_send(HFUART0,p,strlen(p), 50);  //kim for debug  °Ñudp·¢µ½MCU´®¿Ú  ²é¿´»Ø¸´µÄÐÅÏ¢ÊÇ·ñÕýÈ
	
	sendto(fd,p,len,0,(struct sockaddr*)&addr,(socklen_t)alen);
	sendto(fd,p,len,0,(struct sockaddr*)&addr,(socklen_t)alen);	
	send_num=sendto(fd,p,len,0,(struct sockaddr*)&addr,(socklen_t)alen);
	
	if(send_num == len)
	{
	

				close(fd);
				fd=-1;
	}
	else
	{
		//u_printf("len is %d,send data bytes:%d\n",len,send_num);
		//u_printf("udp errno=%d\n",errno);
		close(fd);
		
		fd=-1;
		//u_printf("udp disconnectd!\n");
	}
	
	

	return 0;
}

static int create_multicast_socket(uint32_t if_addr)
{
	char optval;
	int r;
	int sd;
	int on=0;
	struct ip_mreq mreq;
	struct sockaddr_in serveraddr;

	sd = socket(PF_INET, SOCK_DGRAM, 0);
	if (sd < 0)
	{
		return sd;
	}
	
	if ((r = setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (char *) &on, sizeof(on))) < 0)  //ÆÕÍ¨socket£¬¿ÉÖØÓÃ   kim
	{
		closesocket(sd);
		return r;
	}

        /* bind to an address */
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(SSDP_PORT);
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY); /* receive multicast */  //Ê¹ÓÃÍ¨ÅäµØÖ· Ö÷ÒªÊÇÖ¸¶¨1900 port kim
	if ((r = bind(sd, (struct sockaddr *)&serveraddr, sizeof(serveraddr))) < 0)
	{
		closesocket(sd);
		return -1; 
	}
	optval =0;
	if ((r = setsockopt(sd, IPPROTO_IP, IP_MULTICAST_LOOP, (char *) &optval, sizeof(optval))) < 0)  //½ûÖ¹»Ø»·  kim
	{
		return r;
	}
	/* Set multicast interface. */
	struct in_addr addr;	
	memset((void *)&addr, 0, sizeof(struct in_addr));
	addr.s_addr = if_addr;
	r = setsockopt(sd, IPPROTO_IP, IP_MULTICAST_IF,(char *)&addr, sizeof(addr));//·¢ËÍ×é²¥±¨ÎÄµÄ±¾µØ½Ó¿Ú    kim
	if (r == -1)
	{
		closesocket(sd);
		return -1;
	}
	/*
	* setup time-to-live
	*/
	optval = 10; /* Hop count */
	r = setsockopt(sd, IPPROTO_IP, IP_MULTICAST_TTL, &optval, sizeof(optval));//time to live  kim
	if (r)  
	{
		closesocket(sd);
		return r;
	}
	// add membership to receiving socket
	memset(&mreq, 0, sizeof(struct ip_mreq));
	mreq.imr_interface.s_addr = if_addr;    //¼ÓÈë×é²¥µÄ±¾µØ½Ó¿Ú  kim
	mreq.imr_multiaddr.s_addr = inet_addr(SSDP_ADDR);
	if ((r = setsockopt(sd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *) &mreq, sizeof(mreq))) < 0) //¼ÓÈë×é²¥  kim
	{
		closesocket(sd);
		return r;
	}
		
	return sd;
}



USER_FUNC  void upnp_main_thread(void* arg)
{
	int upnp_fd;
	uint32_t if_addr;
	fd_set sockfd_set;
	int max_fd ;
	struct timeval tv;	
	char *pkt_buffer;
	int ret,i;
	char *p=0;
#define UPNP_RECV_PACKET_SIZE      (1024)
	extern int hfnet_enable_multicast(int enable);
	hfnet_enable_multicast(1);
	pkt_buffer = hfmem_malloc(UPNP_RECV_PACKET_SIZE);
	if(pkt_buffer==NULL)
	{
		//u_printf("no mem\n");
		return;
	}
	while(1)
	{
		if((if_addr=get_if_ip_addr())==0)  //µÃµ½Ä£¿éµ±Ç°ipµØÖ·  kim
		{
			msleep(500);
			continue;
		}
		get_mac_addr();
		//u_printf("kim+++ if_addr is %x\n",if_addr);
		
		upnp_fd = create_multicast_socket(if_addr);
		if(upnp_fd<0)
			return;
		else
			break;
	}
	
	//tcp server  
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	
	memset((char*)&servaddr,0,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(30001);
	servaddr.sin_addr.s_addr=htonl(INADDR_ANY);	

  bind(listenfd,(struct sockaddr *)&servaddr,sizeof(servaddr));
	
	listen(listenfd,10);

	//u_printf("tcp server bind ok!\n");
	
	
	if(listenfd > upnp_fd)
		max_fd =listenfd;
	else
		max_fd =upnp_fd;
	maxi = -1;
	
	for(i=0;i<MAXCLIENT;i++)
	{
		client[i] = -1;
	}
	//
	
			
	
	while(1)
	{
		int recvsize=0;
	
		tv.tv_sec=0;
		tv.tv_usec=200;
		////u_printf("time=%d\n",hfsys_get_time());
		
		
		
		
		FD_ZERO(&sockfd_set);
		FD_SET(upnp_fd,&sockfd_set);	
		FD_SET(listenfd,&sockfd_set);
		
	
		for(i=0;i<MAXCLIENT;i++)
		{
			if(client[i]>=0)
			{
				
				FD_SET(client[i],&sockfd_set);
				
				if(client[i] > max_fd)
					max_fd =client[i];
			
			}
		}
	/*	
			count++;
			if(count == 1000)
			{
				//u_printf("send alive package\r\n");
				send_alive_packet(upnp_fd,pkt_buffer,UPNP_RECV_PACKET_SIZE,if_addr);			
				count = 0;
			}		
		*/
		ret=select(max_fd+1,&sockfd_set,NULL,NULL,&tv);
		
		

		
		
		if(ret<=0)
		{
					
			//send_alive_packet(upnp_fd,pkt_buffer,UPNP_RECV_PACKET_SIZE,if_addr);
			continue;
		}
	
		if(FD_ISSET(listenfd, &sockfd_set))   //s2
		{
		
			clilen = sizeof(cliaddr);
			connfd = accept(listenfd,(struct sockaddr *)&cliaddr,&clilen);
			for(i=0;i<MAXCLIENT;i++)
			{
				if(client[i]<0)
				{
					client[i] = connfd;
					maxi = maxi+1;
					//u_printf("accept client socket : %d  client count : %d\n",connfd,maxi+1);
					break;
				}
			}
			if(i == MAXCLIENT)
			{
				//u_printf("too many clients\r\n");
			}
			
		  /*FD_SET(connfd, &sockfd_set); //½«ÒÑÁ¬½Ósocket ¼ÓÈëselect
			
			if(connfd > max_fd)
				max_fd = connfd;  */
			
			//if(i > maxi)
			
			
			if(--ret<=0)
				continue;			
		}
	
		for(i=0;i<=maxi;i++)  //¼ì²éËùÓÐµÄÒÑÁ¬½ÓsocketÓÐÎÞÊý¾Ý  //s3
		{
			
			if(((sockfd = client[i])<0))
				continue;
			
			if(FD_ISSET(sockfd, &sockfd_set))
			{
				
				if((recv_num =recv(sockfd,tcpserver_buf,sizeof(tcpserver_buf),0))>0)
				{
					while(tcp_send_finish == 0)
					{
						u_printf("busy,try late again\r\n");
						msleep(10);
					}
					
					tcp_send_finish = 0;
					////u_printf("recv data bytes:%d\n %s\r\n",recv_num,tcpserver_buf);

					
					if(recv_num <400)  //·ÀÖ¹Á½ÌõÏûÏ¢Õ³Á¬
					{						
						TCP_Encode(); //add tcp identification code			

						u_printf("FW to MCU,LOCAL TCP,TCP fd is %d:\r\n%s\r\n\r\n",sockfd,tcpserver_encodedbuf);
						
						hfuart_send(HFUART0,tcpserver_encodedbuf,strlen(tcpserver_encodedbuf), 50);
					
				//	if(tcp_uartfd == 0)
					//{
						tcp_uartfd = sockfd;
					}
					else
					{
						u_printf("recv num is %d, a catastrophe.Get rid of it\r\n",recv_num);					
					}
					
						tcp_send_finish = 1;

					
					  //²âÊÔ²éÑ¯Éè±¸ÓÃ
/*		
					if((p = strstr(tcpserver_buf,"info"))!= NULL)   //ÊÖ»ú²éÑ¯Éè±¸ÐÅÏ¢
				{
					//u_printf("send info\r\n");
					write(sockfd,info,strlen(info));
				}
				else if((p = strstr(tcpserver_buf,"channel"))!= NULL)   //ÊÖ»ú²éÑ¯°²È«Ä£Ê½´ò¿ª¹Ø±ÕµÄ×´Ì¬
				{
					//u_printf("send channel\r\n");
					write(sockfd,channel,strlen(channel));
				}
	*/				
				
					memset(tcpserver_encodedbuf,0,400);
					memset(tcpserver_buf,0,400);		
				
				}
				else
				{
					close(sockfd);
					//FD_CLR(sockfd, &sockfd_set);
					client[i] = -1;
					maxi=maxi -1;
					//u_printf("delet client socket ,client count : %d\n",maxi+1);
					
				}
		
				if(--ret <= 0)
					break;
				
			}
		}		
		
		
		if(FD_ISSET(upnp_fd, &sockfd_set))  //s1
		{			
			struct sockaddr_in fromaddr;
			
				
			socklen_t sockaddr_size = sizeof(struct sockaddr_in);
			memset(pkt_buffer,0,UPNP_RECV_PACKET_SIZE);
			recvsize = recvfrom(upnp_fd, (char*)pkt_buffer, UPNP_RECV_PACKET_SIZE-4, 0,(struct sockaddr *) &fromaddr, &sockaddr_size);
			
	//		//u_printf("kim+++ Broadcast from  ip:%x, port:%x\n",fromaddr.sin_addr.s_addr,fromaddr.sin_port);
			if (recvsize < 0)
			{
				//u_printf("recv() fail\n");
			}
			else
			{

				
				
				if(strstr(pkt_buffer,"Lucis-tech")!=NULL)
				{
					//u_printf("recv length=%d,buffer is %s\n",recvsize,pkt_buffer);
					if(Sync_Flag == 1)   //only  after  mcu sync Fa info to wifi
					{
						//u_printf("test AAA\r\n");
						send_msearch_rsp_packet(&fromaddr);
					}
				}	

			}			
			
			if(--ret<=0)
				continue;
		}
		


	}

	if(pkt_buffer!=NULL)
		hfmem_free(pkt_buffer);
	
	return ;
}




