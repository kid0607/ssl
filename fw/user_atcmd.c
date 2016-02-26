
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

#include <cyassl/openssl/ssl.h>
#include <cyassl/internal.h>
#include <cyassl/cyassl_config.h>
#include <stdarg.h>
#include <string.h>
#include "user_atcmd.h"

CYASSL*         ssl     = 0;
user_ssl user_ssl_par;
char user_ssl_recvbuf[400];
char user_ssl_encodedbuf[400];



static const char user_cert[]="-----BEGIN CERTIFICATE-----\nMIIENjCCAx6gAwIBAgIBATANBgkqhkiG9w0BAQUFADBvMQswCQYDVQQGEwJTRTEUMBIGA1UEChMLQWRkVHJ1c3QgQUIxJjAkBgNVBAsTHUFkZFRydXN0IEV4dGVybmFsIFRUUCBOZXR3b3JrMSIwIAYDVQQDExlBZGRUcnVzdCBFeHRlcm5hbCBDQSBSb290MB4XDTAwMDUzMDEwNDgzOFoXDTIwMDUzMDEwNDgzOFowbzELMAkGA1UEBhMCU0UxFDASBgNVBAoTC0FkZFRydXN0IEFCMSYwJAYDVQQLEx1BZGRUcnVzdCBFeHRlcm5hbCBUVFAgTmV0d29yazEiMCAGA1UEAxMZQWRkVHJ1c3QgRXh0ZXJuYWwgQ0EgUm9vdDCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALf3GjPm8gAELTngTlvtH7xsD821+iO2zt6bETOXpClMfZOfvUq8k+0DGuOPz+VtUFrWlymUWoCwSXrbLpX9uMq/NzgtHj6RQa1wVsfwTz/oMp50ysiQVOnGXw94nZpAPA6sYapeFI+eh6FqUNzXmk6vBbOmcZSccbNQYArHE504B4YCqOmoaSYYkKtMsE8jqzpPhNjfzp/haW+710LXa0Tkx63ubUFfclpxCDezeWWkWaCUN/cALw3CknLa0Dhy2xSoRcRdKn23tNbE7qzNE0S3ySvdQwAl+mG5aWpYIxG3pzOPVnVZ9c0p10a3CitlttNCbxWyuHv77+ldU9U0WicCAwEAAaOB3DCB2TAdBgNVHQ4EFgQUrb2YejS0Jvf6xCZU7wO94CTLVBowCwYDVR0PBAQDAgEGMA8GA1UdEwEB/wQFMAMBAf8wgZkGA1UdIwSBkTCBjoAUrb2YejS0Jvf6xCZU7wO94CTLVBqhc6RxMG8xCzAJBgNVBAYTAlNFMRQwEgYDVQQKEwtBZGRUcnVzdCBBQjEmMCQGA1UECxMdQWRkVHJ1c3QgRXh0ZXJuYWwgVFRQIE5ldHdvcmsxIjAgBgNVBAMTGUFkZFRydXN0IEV4dGVybmFsIENBIFJvb3SCAQEwDQYJKoZIhvcNAQEFBQADggEBALCb4IUlwtYj4g+WBpKdQZic2YR5gdkeWxQHIzZlj7DYd7usQWxHYINRsPkyPef89iYTx4AWpb9a/IfPeHmJIZriTAcKhjW88t5RxNKWt9x+Tu5w/Rw56wwCURQtjr0W4MHfRnXnJK3s9EK0hZNwEGe6nQY1ShjTK3rMUUKhemPR5ruhxSvCNr4TDea9Y355e6cJDUCrat2PisP29owaQgVR1EX1n6diIWgVIEM8med8vSTYqZEXc4g/VhsxOBi0cQ+azcgOno4uG+GMmIPLHzHxREzGBHNJdmAPx/i9F4BrLunMTA5amnkPIAou1Z5jJh5VkpTYghdae9C8x49OhgQ=\n-----END CERTIFICATE-----\n\
-----BEGIN CERTIFICATE-----\nMIIFdDCCBFygAwIBAgIQJ2buVutJ846r13Ci/ITeIjANBgkqhkiG9w0BAQwFADBvMQswCQYDVQQGEwJTRTEUMBIGA1UEChMLQWRkVHJ1c3QgQUIxJjAkBgNVBAsTHUFkZFRydXN0IEV4dGVybmFsIFRUUCBOZXR3b3JrMSIwIAYDVQQDExlBZGRUcnVzdCBFeHRlcm5hbCBDQSBSb290MB4XDTAwMDUzMDEwNDgzOFoXDTIwMDUzMDEwNDgzOFowgYUxCzAJBgNVBAYTAkdCMRswGQYDVQQIExJHcmVhdGVyIE1hbmNoZXN0ZXIxEDAOBgNVBAcTB1NhbGZvcmQxGjAYBgNVBAoTEUNPTU9ETyBDQSBMaW1pdGVkMSswKQYDVQQDEyJDT01PRE8gUlNBIENlcnRpZmljYXRpb24gQXV0aG9yaXR5MIICIjANBgkqhkiG9w0BAQEFAAOCAg8AMIICCgKCAgEAkehUktIKVrGsDSTdxc9EZ3SZKzejfSNwAHG8U9/E+ioSj0t/EFa9n3Byt2F/yUsPF6c947AEYe7/EZfH9IY+Cvo+XPmT5jR62RRr55yzhaCCenavcZDX7P0N+pxs+t+wgvQUfvm+xKYvT3+Zf7X8Z0NyvQwA1onrayzT7Y+YHBSrfuXjbvzYqOSSJNpDa2K4Vf3qwbxstovzDo2a5JtsaZn4eEgwRdWt4Q08RWD8MpZRJ7xnw8outmvqRsfHIKCxH2XeSAi6pE6p8oNGN4Tr6MyBSENnTnIqm1y9TBsoilwie7SrmNnu4FGDwwlGTm0+mfqVF9p8M1dBPI1R7Qu2XK8sYxrfV8g/vOldxJuvRZnio1oktLqpVj3Pb6r/SVi+8Kj/9Lit6Tf7urj0Czr56ENCHonYhMsT8dm74YlguIwoVqwUHZwK53Hrzw7dPamWoUi9PPevtQ0iTMARgexWO/bTouJbt7IEIlKVgJNp6I5MZfGRAy1wdALqi2cVKWlSArvX31BqVUa/oKMoYX9w0MOiqiwhqkfOKJwGRXa/ghgntNWutMtQ5mv0TIZxMOmm3xaG4Nj/QN370EKIf6MzOi5cHkERgWPOGHFrK+ymircxXDpqR+DDeVnWIBqv8mqYqnK8V0rSS527EPywTEHl7R09XiidnMy/s1Hap0flhFMCAwEAAaOB9DCB8TAfBgNVHSMEGDAWgBStvZh6NLQm9/rEJlTvA73gJMtUGjAdBgNVHQ4EFgQUu69+Aj36pvE8hI6t7jiY7NkyMtQwDgYDVR0PAQH/BAQDAgGGMA8GA1UdEwEB/wQFMAMBAf8wEQYDVR0gBAowCDAGBgRVHSAAMEQGA1UdHwQ9MDswOaA3oDWGM2h0dHA6Ly9jcmwudXNlcnRydXN0LmNvbS9BZGRUcnVzdEV4dGVybmFsQ0FSb290LmNybDA1BggrBgEFBQcBAQQpMCcwJQYIKwYBBQUHMAGGGWh0dHA6Ly9vY3NwLnVzZXJ0cnVzdC5jb20wDQYJKoZIhvcNAQEMBQADggEBAGS/g/FfmoXQzbihKVcN6Fr30ek+8nYEbvFScLsePP9NDXRqzIGCJdPDoCpdTPW6i6FtxFQJdcfjJw5dhHk3QBN39bSsHNA7qxcS1u80GH4r6XnTq1dFDK8o+tDb5VCViLvfhVdpfZLYUspzgb8c8+a4bmYRBbMelC1/kZWSWfFMzqORcUx8Rww7Cxn2obFshj5cqsQugsv5B5a6SE2Q8pTIqXOi6wZ7I53eovNNVZ96YUWYGGjHXkBrI/V5eu+MtWuLt29G9HvxPUsE2JOAWVrgQSQdso8VYFhH2+9uRv0V9dlfmrPb2LjkQLPNlzmuhbsdjrzch5vRpu/xO28QOG8=\n-----END CERTIFICATE-----\n\
-----BEGIN CERTIFICATE-----\n\
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\n-----END CERTIFICATE-----\n";


/*
char user_cert1[]="-----BEGIN CERTIFICATE-----\nMIIENjCCAx6gAwIBAgIBATANBgkqhkiG9w0BAQUFADBvMQswCQYDVQQGEwJTRTEUMBIGA1UEChMLQWRkVHJ1c3QgQUIxJjAkBgNVBAsTHUFkZFRydXN0IEV4dGVybmFsIFRUUCBOZXR3b3JrMSIwIAYDVQQDExlBZGRUcnVzdCBFeHRlcm5hbCBDQSBSb290MB4XDTAwMDUzMDEwNDgzOFoXDTIwMDUzMDEwNDgzOFowbzELMAkGA1UEBhMCU0UxFDASBgNVBAoTC0FkZFRydXN0IEFCMSYwJAYDVQQLEx1BZGRUcnVzdCBFeHRlcm5hbCBUVFAgTmV0d29yazEiMCAGA1UEAxMZQWRkVHJ1c3QgRXh0ZXJuYWwgQ0EgUm9vdDCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALf3GjPm8gAELTngTlvtH7xsD821+iO2zt6bETOXpClMfZOfvUq8k+0DGuOPz+VtUFrWlymUWoCwSXrbLpX9uMq/NzgtHj6RQa1wVsfwTz/oMp50ysiQVOnGXw94nZpAPA6sYapeFI+eh6FqUNzXmk6vBbOmcZSccbNQYArHE504B4YCqOmoaSYYkKtMsE8jqzpPhNjfzp/haW+710LXa0Tkx63ubUFfclpxCDezeWWkWaCUN/cALw3CknLa0Dhy2xSoRcRdKn23tNbE7qzNE0S3ySvdQwAl+mG5aWpYIxG3pzOPVnVZ9c0p10a3CitlttNCbxWyuHv77+ldU9U0WicCAwEAAaOB3DCB2TAdBgNVHQ4EFgQUrb2YejS0Jvf6xCZU7wO94CTLVBowCwYDVR0PBAQDAgEGMA8GA1UdEwEB/wQFMAMBAf8wgZkGA1UdIwSBkTCBjoAUrb2YejS0Jvf6xCZU7wO94CTLVBqhc6RxMG8xCzAJBgNVBAYTAlNFMRQwEgYDVQQKEwtBZGRUcnVzdCBBQjEmMCQGA1UECxMdQWRkVHJ1c3QgRXh0ZXJuYWwgVFRQIE5ldHdvcmsxIjAgBgNVBAMTGUFkZFRydXN0IEV4dGVybmFsIENBIFJvb3SCAQEwDQYJKoZIhvcNAQEFBQADggEBALCb4IUlwtYj4g+WBpKdQZic2YR5gdkeWxQHIzZlj7DYd7usQWxHYINRsPkyPef89iYTx4AWpb9a/IfPeHmJIZriTAcKhjW88t5RxNKWt9x+Tu5w/Rw56wwCURQtjr0W4MHfRnXnJK3s9EK0hZNwEGe6nQY1ShjTK3rMUUKhemPR5ruhxSvCNr4TDea9Y355e6cJDUCrat2PisP29owaQgVR1EX1n6diIWgVIEM8med8vSTYqZEXc4g/VhsxOBi0cQ+azcgOno4uG+GMmIPLHzHxREzGBHNJdmAPx/i9F4BrLunMTA5amnkPIAou1Z5jJh5VkpTYghdae9C8x49OhgQ=\n-----END CERTIFICATE-----\n";
char user_cert2[]="-----BEGIN CERTIFICATE-----\nMIIFdDCCBFygAwIBAgIQJ2buVutJ846r13Ci/ITeIjANBgkqhkiG9w0BAQwFADBvMQswCQYDVQQGEwJTRTEUMBIGA1UEChMLQWRkVHJ1c3QgQUIxJjAkBgNVBAsTHUFkZFRydXN0IEV4dGVybmFsIFRUUCBOZXR3b3JrMSIwIAYDVQQDExlBZGRUcnVzdCBFeHRlcm5hbCBDQSBSb290MB4XDTAwMDUzMDEwNDgzOFoXDTIwMDUzMDEwNDgzOFowgYUxCzAJBgNVBAYTAkdCMRswGQYDVQQIExJHcmVhdGVyIE1hbmNoZXN0ZXIxEDAOBgNVBAcTB1NhbGZvcmQxGjAYBgNVBAoTEUNPTU9ETyBDQSBMaW1pdGVkMSswKQYDVQQDEyJDT01PRE8gUlNBIENlcnRpZmljYXRpb24gQXV0aG9yaXR5MIICIjANBgkqhkiG9w0BAQEFAAOCAg8AMIICCgKCAgEAkehUktIKVrGsDSTdxc9EZ3SZKzejfSNwAHG8U9/E+ioSj0t/EFa9n3Byt2F/yUsPF6c947AEYe7/EZfH9IY+Cvo+XPmT5jR62RRr55yzhaCCenavcZDX7P0N+pxs+t+wgvQUfvm+xKYvT3+Zf7X8Z0NyvQwA1onrayzT7Y+YHBSrfuXjbvzYqOSSJNpDa2K4Vf3qwbxstovzDo2a5JtsaZn4eEgwRdWt4Q08RWD8MpZRJ7xnw8outmvqRsfHIKCxH2XeSAi6pE6p8oNGN4Tr6MyBSENnTnIqm1y9TBsoilwie7SrmNnu4FGDwwlGTm0+mfqVF9p8M1dBPI1R7Qu2XK8sYxrfV8g/vOldxJuvRZnio1oktLqpVj3Pb6r/SVi+8Kj/9Lit6Tf7urj0Czr56ENCHonYhMsT8dm74YlguIwoVqwUHZwK53Hrzw7dPamWoUi9PPevtQ0iTMARgexWO/bTouJbt7IEIlKVgJNp6I5MZfGRAy1wdALqi2cVKWlSArvX31BqVUa/oKMoYX9w0MOiqiwhqkfOKJwGRXa/ghgntNWutMtQ5mv0TIZxMOmm3xaG4Nj/QN370EKIf6MzOi5cHkERgWPOGHFrK+ymircxXDpqR+DDeVnWIBqv8mqYqnK8V0rSS527EPywTEHl7R09XiidnMy/s1Hap0flhFMCAwEAAaOB9DCB8TAfBgNVHSMEGDAWgBStvZh6NLQm9/rEJlTvA73gJMtUGjAdBgNVHQ4EFgQUu69+Aj36pvE8hI6t7jiY7NkyMtQwDgYDVR0PAQH/BAQDAgGGMA8GA1UdEwEB/wQFMAMBAf8wEQYDVR0gBAowCDAGBgRVHSAAMEQGA1UdHwQ9MDswOaA3oDWGM2h0dHA6Ly9jcmwudXNlcnRydXN0LmNvbS9BZGRUcnVzdEV4dGVybmFsQ0FSb290LmNybDA1BggrBgEFBQcBAQQpMCcwJQYIKwYBBQUHMAGGGWh0dHA6Ly9vY3NwLnVzZXJ0cnVzdC5jb20wDQYJKoZIhvcNAQEMBQADggEBAGS/g/FfmoXQzbihKVcN6Fr30ek+8nYEbvFScLsePP9NDXRqzIGCJdPDoCpdTPW6i6FtxFQJdcfjJw5dhHk3QBN39bSsHNA7qxcS1u80GH4r6XnTq1dFDK8o+tDb5VCViLvfhVdpfZLYUspzgb8c8+a4bmYRBbMelC1/kZWSWfFMzqORcUx8Rww7Cxn2obFshj5cqsQugsv5B5a6SE2Q8pTIqXOi6wZ7I53eovNNVZ96YUWYGGjHXkBrI/V5eu+MtWuLt29G9HvxPUsE2JOAWVrgQSQdso8VYFhH2+9uRv0V9dlfmrPb2LjkQLPNlzmuhbsdjrzch5vRpu/xO28QOG8=\n-----END CERTIFICATE-----\n";
char user_cert3[]="-----BEGIN CERTIFICATE-----\n\
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\n-----END CERTIFICATE-----\n";
*/
//char user_cert[]="test";

//char cert[5] = {0};

void SSL_Encode(void)
{
	char *p = NULL;
	int i = 0;
	int j=0;

	p = strstr(user_ssl_recvbuf,"Date");
	if(p!=NULL)
	{
		i = &p[0] -  &user_ssl_recvbuf[0]; //gross characters  before “Date”
	//	u_printf("address is  00 %d, 01 %d, 02 %d, i is %d\r\n",p,&p[0],&user_ssl_recvbuf[0],i);
	
		for(j=0;j<i;j++)    //PartI:words before "Date"
		{
			user_ssl_encodedbuf[j] = user_ssl_recvbuf[j];
		}		
		strncpy(&user_ssl_encodedbuf[i],"EVENTSOURCE:SSL\r\n",17); //PartII：added identification
		strcpy(&user_ssl_encodedbuf[i+17],&user_ssl_recvbuf[i]);
		
	//	u_printf("now our buf is %s\n",user_ssl_encodedbuf);
	}	
}




void user_init_atcmd_para(void)
{
	extern user_ssl user_ssl_par;
	hfuflash_read(USER_ATCMD_PARA_ADDR, (char *)&user_ssl_par, sizeof(user_ssl));
	user_ssl_par.connect_flag=0x00;
	if(user_ssl_par.usable_flag!=0x01)
	{
		hfuflash_erase_page(USER_ATCMD_PARA_ADDR,1);
		memset((char *)&user_ssl_par,0,sizeof(user_ssl));
	}
}

void user_cmd_deal_rssllk(pat_session_t s,int argc,char *argv[],char *rsp,int len)
{
	extern user_ssl user_ssl_par;
	char ssl_rsp[100]={'\0'};
	char tail[5]={'\0'};
	
	tail[0]= (0x0d);
	tail[1]= 0x0a;
	tail[2]= 0x0d;
	tail[3]= 0x0a;
	tail[4]= '\0';
	
	switch(argc)
	{
		case 0:
			{
				if(user_ssl_par.connect_flag!=0x01)
					sprintf(ssl_rsp,"%s%s","+ok=off",tail);
				else
					sprintf(ssl_rsp,"%s%s","+ok=on",tail);
				hfuart_send(HFUART0,ssl_rsp,strlen(ssl_rsp),100);
				break;
			}
		default:
			{
				sprintf(ssl_rsp,"%s%s","+ERR=-4",tail);
				hfuart_send(HFUART0,ssl_rsp,strlen(ssl_rsp),100);
				return ;
			}
	}
	return ;
}

unsigned short user_power(int len)
{
	unsigned short data = 1;
	int i = 0;

	for(i = 0;i < len;i++)
		data = data*10;
	return data;
}
void use_cmd_rsslnetp(pat_session_t s,int argc,char *argv[],char *rsp,int len)
{
	extern user_ssl user_ssl_par;
	char ssl_rsp[100]={'\0'};
	char tail[5]={'\0'};
	
	tail[0]= (0x0d);
	tail[1]= 0x0a;
	tail[2]= 0x0d;
	tail[3]= 0x0a;
	tail[4]= '\0';
	
	switch(argc)
	{
		case 0:
			{
				sprintf(ssl_rsp,"%s%d,%s%s","+ok=",user_ssl_par.ssl_port,user_ssl_par.ssl_addr,tail);
				hfuart_send(HFUART0,ssl_rsp,strlen(ssl_rsp),100);
				break;
			}
		case 1:
			{
				sprintf(ssl_rsp,"%s%s","+ERR=-4",tail);
				hfuart_send(HFUART0,ssl_rsp,strlen(ssl_rsp),100);
				break;
			}
		case 2:
			{
				user_ssl_par.usable_flag=0x01;
				user_ssl_par.connect_flag=0x00;

				unsigned short port = 0x0000;
				int i=0;
				int len = strlen(argv[0]);
				for(i=0;i<len;i++)
					port = port + (argv[0][i]-'0')*user_power(len-i-1);
				user_ssl_par.ssl_port=port;
				Memcpy(user_ssl_par.ssl_addr,argv[1],strlen(argv[1]));
				user_ssl_par.ssl_addr[strlen(argv[1])]='\0';

				hfuflash_erase_page(USER_ATCMD_PARA_ADDR,1);
				hfuflash_write(USER_ATCMD_PARA_ADDR, (char *)&user_ssl_par,sizeof(user_ssl));
				
				sprintf(ssl_rsp,"%s%s","+ok",tail);
				hfuart_send(HFUART0,ssl_rsp,strlen(ssl_rsp),100);
				break;
			}
		default:
			{
				sprintf(ssl_rsp,"%s%s","+ERR=-4",tail);
				hfuart_send(HFUART0,ssl_rsp,strlen(ssl_rsp),100);
				return ;
			}
	}
	return ;
}



void user_set_local_time(void)
{
	//set module time: 2015-11-23-00:00:00
	struct timeval tv;
	struct tm nowtm;
	nowtm.tm_year = 2016-1900;
	nowtm.tm_mon = 2-1;
	nowtm.tm_mday = 24;
	nowtm.tm_hour = 0;
	nowtm.tm_min = 0;
	nowtm.tm_sec = 0;
	tv.tv_sec = mktime(&nowtm);
	settimeofday(&tv,NULL);

	msleep(1000);
}
/* return 1 is a ipaddress */
int user_seuaddressis_ip(const char * ipaddr)
{
	char ii, ipadd;
	int i, j;
	
	ii=0;
	for (j= 0; j< 4; j++)
	{
		ipadd=0;
		for (i=0; i< 4; i++, ii++)
		{
			if (*(ipaddr+ii)=='.')
				if (i== 0)
					return 0;		//the first shall not be '.'
				else
				{
					ii++;
					break;			//this feild finished
				}
			else if ((i==3)&&(j!=3))	//not the last feild, the number shall less than 4 bits
				return 0;
			else if ((*(ipaddr+ii) > '9')||(*(ipaddr+ii) < '0'))
			{
				if ((*(ipaddr+ii) == '\0')&&(j==3)&&(i!=0))
				{
					break;
				}
				else
					return 0;			//pls input number
			}
			else
				ipadd= ipadd*10+(*(ipaddr+ii)-'0');
			if (ipadd > 255)
				return 0;
		}
	}
	return 1;
}
int user_tcp_connect_ssl_server(char *url,unsigned short port)
{
	HF_Debug(10, "addr is :%s ,port is:%d\n\r",url,port);
	int fd;	
	struct sockaddr_in addr;
	struct sockaddr_in local_addr;
	char *addrp=url;
	
	if((memcmp(url, "HTTPS://", 8)==0)||(memcmp(url, "https://", 8)==0))
		addrp= (char *)(url+8);

	ip_addr_t dest_addr;
	if(user_seuaddressis_ip((const char *)(addrp)) !=1 )
	{
		HF_Debug(10, "addr is not ip \n\r");
		if(hfnet_gethostbyname((const char *)(addrp), &dest_addr) !=HF_SUCCESS)
		{
			HF_Debug(10, "gethostbyname fail\n\r");
			return -1;
		}
		HF_Debug(10, "gethostbyname success:%d\n\r",dest_addr.addr);
		
	}
	else
		inet_aton((char *)(addrp), (ip_addr_t *) &dest_addr);
	unsigned short local_port = ((Timer1GetTime()>>16)+(Timer1GetTime())&0xFFFF)&0x1FFF;
	

	
	local_port += 0x2FFF;

	

	
	memset((char*)&addr,0,sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr=dest_addr.addr;
	fd = socket(AF_INET, SOCK_STREAM, 0);

	memset((char *)&local_addr, 0, sizeof(local_addr));
	local_addr.sin_family = AF_INET;
	local_addr.sin_len = sizeof(local_addr);
	local_addr.sin_port = htons(local_port);
	local_addr.sin_addr.s_addr= htonl(INADDR_ANY);
	bind(fd,(struct sockaddr *)&local_addr,sizeof(local_addr));
	
	HF_Debug(10, "socket creat success :%d\n\r",fd);
	if(fd<0)
		return -1;
	
	if (connect(fd, (struct sockaddr *)&addr, sizeof(addr))< 0)
	{
		HF_Debug(10, "connet to server fail\n\r");
		close(fd);
		return -1;
	}
	HF_Debug(10, "connet to server success\n\r");
	return fd;
}

void user_cmd_ssl(void)
{
	extern user_ssl user_ssl_par;
//	char *cert;
	int len;
	int i;
	
	InitMemoryTracker();//for debug, it can show how many memory used in SSL
	CyaSSL_Debugging_ON();//for debug

	CyaSSL_Init();
	CYASSL_METHOD*  method  = 0;
	CYASSL_CTX*     ctx     = 0;
//	CYASSL*         ssl     = 0;
	int sockfd;
	int recvlen;
	fd_set sockfd_set;
	int max_fd ;
	struct timeval tv;
	int ret;
	char sendbuf[]="hello world ";

	
	while(user_ssl_par.usable_flag!=0x01)
	{
	//	HF_Debug(10, "user_ssl_par is unusable\n\r");
		msleep(2000);
		continue;
	}
	
//write certificat into flash only when first try
	
	//u_printf("cert size is %d\r\n",sizeof(user_cert3));	
/*
	hfuflash_erase_page(USER_CERTIFICATE1_ADDR,3);   	//目前证书占三页
	if(hfuflash_write(USER_CERTIFICATE1_ADDR, user_cert1,CURRENT_CERTIFICATE1_SIZE) < CURRENT_CERTIFICATE1_SIZE)
	{
		u_printf("flash write fail\r\n");
	}		
	if(hfuflash_write(USER_CERTIFICATE2_ADDR, user_cert2,CURRENT_CERTIFICATE2_SIZE) < CURRENT_CERTIFICATE2_SIZE)
	{
		u_printf("flash write fail\r\n");
	}	
	if(hfuflash_write(USER_CERTIFICATE3_ADDR, user_cert3,CURRENT_CERTIFICATE3_SIZE) < CURRENT_CERTIFICATE3_SIZE)
	{
		u_printf("flash write fail\r\n");
	}		
*/
/*	
	cert = (char*)hfmem_malloc(CURRENT_CERTIFICATE_TOTAL_SIZE);   //  6k should be enough for current certificate 
	if(cert)
      u_printf("Memory Allocated at: %x",cert);
  else
      u_printf("Not Enough Memory!\n");		
	
	hfuflash_read(USER_CERTIFICATE1_ADDR,cert,CURRENT_CERTIFICATE1_SIZE);
	hfuflash_read(USER_CERTIFICATE2_ADDR,cert+CURRENT_CERTIFICATE1_SIZE-1,CURRENT_CERTIFICATE2_SIZE);	
	hfuflash_read(USER_CERTIFICATE3_ADDR,cert+CURRENT_CERTIFICATE1_SIZE+CURRENT_CERTIFICATE2_SIZE-2,CURRENT_CERTIFICATE3_SIZE);		
	for(i=0;i<CURRENT_CERTIFICATE_TOTAL_SIZE;i++)
	{
		u_printf("%c",cert[i]);
	}
*/

	
	
	method=CyaSSLv3_client_method();//step 1 CyaSSLv3_client_method
	if (method == NULL)
		HF_Debug(10, "unable to get method\n\r");

	ctx = CyaSSL_CTX_new(method);//step 2 CyaSSL_CTX_new
	if (ctx == NULL)
	{
		HF_Debug(10, "unable to get ctx\n\r");
		return;
	}

#if SSL_USE_CERT
	CyaSSL_CTX_load_verify_buffer(ctx, user_cert, sizeof(user_cert)-1, SSL_FILETYPE_PEM);//step 3 CyaSSL_CTX_load_verify_buffer
#else
	CyaSSL_CTX_set_verify(ctx, SSL_VERIFY_NONE, 0);//step 3 disable verify certificates
#endif
	ssl = CyaSSL_new(ctx);//step 4 CyaSSL_new
	if (ssl == NULL)
	{
		HF_Debug(10, "unable to get SSL object\n\r");
		goto FREE_CTX;
	}
	sockfd=user_tcp_connect_ssl_server(user_ssl_par.ssl_addr,user_ssl_par.ssl_port);//step 5 tcp_connect_ssl_server
	if(sockfd<0)
	{
		HF_Debug(10, "create socket error\n\r");
		goto FREE_SSL;
	}
	CyaSSL_set_fd(ssl, sockfd);//step 6 CyaSSL_set_fd
	if (CyaSSL_connect(ssl) != SSL_SUCCESS)//step 7 CyaSSL_connect
 	{
		int  err = CyaSSL_get_error(ssl, 0);
		char buffer[80];
		HF_Debug(10, "err = %d, %s\n", err,CyaSSL_ERR_error_string(err, buffer));
		HF_Debug(10, "SSL_connect failed\n\r");
		goto FREE_SSL;
    	}
	user_ssl_par.connect_flag = 0x01;
	HF_Debug(10, "user_ssl_par is connected\n\r");
	HF_Debug(DEBUG_LEVEL_LOW,"++++++++++++++++++++++SSL connect success++++++++++++++++++++++\n");

/*			
	if (CyaSSL_write(ssl, sendbuf, sizeof(sendbuf)) != sizeof(sendbuf))// CyaSSL_write
       		HF_Debug(10,"SSL_write failed");
*/
			
	//hfmem_free(cert);			
			
	while(1)
	{
		if(user_ssl_par.connect_flag!=0x01)
		{
			HF_Debug(10, "user_ssl_par is changed by at cmd\n\r");
			goto FREE_SSL;
		}
		tv.tv_sec=0;
		tv.tv_usec=200;
		FD_ZERO(&sockfd_set);
		FD_SET(sockfd,&sockfd_set);
		max_fd=sockfd;
		ret=select(max_fd+1,&sockfd_set,NULL,NULL,&tv);
		if(ret<0)
			continue;
		if(FD_ISSET(sockfd, &sockfd_set))
		{
			recvlen = CyaSSL_read(ssl, user_ssl_recvbuf, sizeof(user_ssl_recvbuf)-1);// CyaSSL_read
			if (recvlen > 0)
			{
			//	u_printf("Server response: recv start ----------------------------------------\n");
				CyaSSL_Debugging_OFF();
				
				if(recvlen < 400)
				{
					SSL_Encode();  //add SSL ID code 		
					u_printf("FW to MCU,REMOTE SSL:\r\n%s\r\n\r\n",user_ssl_encodedbuf);					
					hfuart_send(HFUART0, user_ssl_encodedbuf, strlen(user_ssl_encodedbuf),1000);
				}
				else
				{
					u_printf("recv num is %d, exceed the buf. get rid of it\r\n",recvlen);
				}
				CyaSSL_Debugging_ON();
				//u_printf("\n---------------------------------------- recv End!\n");	
			}
			else if (recvlen <= 0) 
			{
				user_ssl_par.connect_flag = 0x00;
				HF_Debug(10, "user_ssl_par is changed by unknown \n\r");
				int readErr = CyaSSL_get_error(ssl, 0);
				if (readErr != SSL_ERROR_WANT_READ)
					HF_Debug(10, "CyaSSL_read failed");
				goto FREE_SSL;
			}
		}
		
	}

FREE_SSL:
	CyaSSL_shutdown(ssl);
	CyaSSL_free(ssl);
	close(sockfd);
FREE_CTX:
	CyaSSL_CTX_free(ctx);
	
	
	
	
	CyaSSL_Debugging_OFF();//close debug
	ShowMemoryTracker();//peek into how memory was used
	msleep(1000);
}

void User_cmd_ssl(void)
{
	extern char hfwifi_is_connected_flag;
	while(hfwifi_is_connected_flag != 1)
	{
		msleep(3000);
	}

	user_set_local_time(); //这个地方时间怎么获取？  存在逻辑矛盾
	
	HF_Debug(DEBUG_LEVEL_LOW,"\nSSL test start!\r\n");	
	user_cmd_ssl();
	HF_Debug(DEBUG_LEVEL_LOW,"\nSSL test end!\r\n");	

	hfthread_destroy(NULL);
}





