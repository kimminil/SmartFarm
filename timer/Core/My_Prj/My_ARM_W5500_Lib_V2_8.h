/*
 * My_ARM_W5500_Lib_V2_8.h
 *
 *  Created on: 2018. 8. 20.
 *  Name: Song Myoung Gyu
 *  Hp: 010-2403-4398
 *  E-main: mgsong@hanmail.net
 *
 *  V1.0 = 최조작성                           = 2018, 08, 20
 *  V2.0 = 기존파일 통합                       = 2023, 01, 08
 *  V2.6 = Server 프로그램 추가                = 2023, 01, 10
 *  V2.7 = MCU 디바이스 추가                   = 2023, 06, 22
 *  V2.8 = 파일명 변경, my_printf 채널번호 추가   = 2024, 02, 14
 */


#ifndef __W5500__
#define __W5500__

#include <stdlib.h>
#include <string.h>

#include "main.h"

// 2023, 06, 22 추가 = 최초
// 2025, 06, 10 추가 = AIOT 4기생
#if F446RE
   #include "stm32f4xx_hal.h" // F446 시리즈
   #include "main.h"
#elif F103RB
   #include "stm32f1xx_hal.h"  // F103, F시리즈
   #include "main.h"
#elif L152RE
   #include "stm32l1xx_hal.h"    //L152/151/162 시리즈
   #include "main.h"
#endif

#include "spi.h"

// =============================================================================
typedef unsigned char 	u8;			typedef volatile unsigned char 	vu8;
typedef char 			s8;         typedef volatile char 			vs8;
typedef unsigned int 	u16;        typedef volatile unsigned short vu16;
typedef int 			s16;        typedef volatile short 			vs16;
typedef unsigned long 	u32;        typedef volatile unsigned int 	vu32;
typedef long 			s32;        typedef volatile int 			vs32;
//==============================================================================

// 2023, 1, 9 수정
#define SPI_1          hspi1
#define SPI_2          hspi2
#define W5500_SPI_CH	SPI_2    //	hspi2

#define TRUE     			(1)
#define FALSE     			(0)

#define SOCK_TCPS0       0
#define SOCK_TCPS1       1
#define SOCK_TCPS2       2
#define SOCK_TCPS3       3
#define SOCK_TCPS4       4
#define SOCK_TCPS5       5
#define SOCK_TCPS6       6
#define SOCK_TCPS7       7

// 2023, 1, 9 추가
#define PORT_TCPS       5000 // 8000
#define PORT_UDPS       3000

// w5500 lib coll = wiznet  == 인크루드 패치 경로에서 core 자체 폴더를 추가 시킵니다.
#include "w5500_lib\Ethernet\W5500\w5500.h"
#include "w5500_lib\Ethernet\socket.h"
#include "w5500_lib\Ethernet\wizchip_conf.h"
#include "w5500_lib\Application\loopback\loopback.h"


/////////////////////////////////////////
// SOCKET NUMBER DEFINION for Examples //
/////////////////////////////////////////
#define SOCK_TCPS       1  //test �Ҵ�� 1�� �ؾ� ��
#define SOCK_UDPS       0  //test �Ҵ�� 1�� �ؾ� ��

// 2023, 1, 10 추가
#define getSn_SR(sn) \
		WIZCHIP_READ(Sn_SR(sn))

////////////////////////////////////////////////
// Shared Buffer Definition for LOOPBACK TEST //
////////////////////////////////////////////////
#define MAX_SOCK_NUM		8	/**< Maxmium number of socket  */
#define DATA_BUF_SIZE		2048

extern wiz_NetInfo gWIZNETINFO;

//2023, 1, 9 수정
extern SPI_HandleTypeDef  W5500_SPI_CH;  // hspi2;
static uint32_t randomKey;

typedef struct _DESTINATION_SETUP
{
	// CLient IP, Port
	uint16_t port;
	uint8_t  destip[4];
}DESTINATION_SETUP;

DESTINATION_SETUP Destination_Setup;

// 2023 1, 10 수정
static uint8_t gDATABUF[DATA_BUF_SIZE];
static uint8_t TX_BUF[DATA_BUF_SIZE]; // TX Buffer for applications
static uint8_t recv_buf[DATA_BUF_SIZE];

int32_t ret;
uint8_t tmpstr[6];
uint32_t tmp[10];

int  w5500_init(void);
void w5500_ret(void);
void w5500_wr_b(unsigned char txByte);
unsigned char w5500_rd_b(void);
void w5500_cs(void);
void w5500_ds(void);
int isLinked(void);
void print_network_information(void);
void network_init(void);
void Loopback_ecro_server_test(void);
void Loopback_ecro_udp_test(void);
void ent_to_uart(uint8_t * buf, uint16_t size_k);
void uart_to_ent(uint8_t sn_k, uint16_t size);
void client_call_back_tx(uint8_t sn_k, uint16_t size);
void client_init(void);
void server_to_client_rx(void);
void client_rtx_chk(uint8_t sn);
void WebServer(uint8_t sn, uint8_t *buf, uint16_t port);


// W5500 Output Port Set
#define TCP_Port  5000  // tcpip port

// Server IP Adderss Setting
uint8_t   server_ip[4] = {192, 168, 1, 56};     //test 1 : Server IP 주소, test2 = PC = Client
uint16_t  port = TCP_Port; // 5000
uint8_t   Client_ip[4] = {192, 168, 1, 46};     //test 1 : MCU Client, Test2 = MCU -Server
///==============================================================

// 공인된 MAC 맥 주소 = 0x10, 0x11, 0x12, 0x13, 0x14, 0x15
// w5500용 CLient IP 설정
// CLient IP Adderss setting
wiz_NetInfo	gWIZNETINFO = {	.mac = {10, 11, 12, 13, 43, 98},
		                    .ip = {192, 168, 1, 56}, // my =w5500용 = Client IP 설정 것/ Server ip
							.sn = {255, 255, 255, 0},
							.gw = {192, 168, 1, 1}, //
							.dns = {8, 1, 5, 7}, //{219, 250, 36, 130},    // sk
							.dhcp = NETINFO_STATIC}; // dns 서버 안 쓰겠다
//=====================================================================

int w5500_init(void)
{
    uint8_t W5500SockBufSize[2][8] = {{2,2,2,2,2,2,2,2,},{2,2,2,2,2,2,2,2}};
    uint32_t tickStart;

   // w5500_ret(); // Low

    /* CS function register = call back function */
    reg_wizchip_cs_cbfunc(w5500_cs, w5500_ds);

    /* spi function register = call back function */
    reg_wizchip_spi_cbfunc(w5500_rd_b, w5500_wr_b);

    ///* Critical section */
    //reg_wizchip_cris_cbfunc(spiMutexEnter,spiMutexExit);
    //w5500_cs();

    // WIZCHIP SOCKET Buffer initialize
    if(ctlwizchip(CW_INIT_WIZCHIP,(void*)W5500SockBufSize) == -1)
    {
        printf("W5500 initialized fail ERROR!!!.\r\n");
        return -1;
    }

    /*Set network information by default*/
    wizchip_setnetinfo(&gWIZNETINFO);
    tickStart = HAL_GetTick();

    do{//check phy status.
        randomKey++;
        if((HAL_GetTick() - tickStart) > 3000 )
        {
            printf("Ethernet Link timeout.\r\n");
            return -1;
        }
    }while(!isLinked());

    /*
        // PHY link status check  = 왕규거가 더 좋음 = 차후 왕규거로 대체
        my_printf("PHY Link Check...");
        do {
        	 if(ctlwizchip(CW_GET_PHYLINK, (void*)&tmp) == -1)
        	  {
        		 printf("Unknown PHY Link stauts.\r\n");
        	  }
        	}while(tmp == PHY_LINK_OFF);
       */

  // Network initialization */
  network_init();

    return 1;
}

void w5500_ret(void)
{
  // Gpio Pin
  //HAL_GPIO_WritePin(RESET_GPIO_Port, RESET_Pin, 0);
  HAL_Delay(10);
  //HAL_GPIO_WritePin(RESET_GPIO_Port, RESET_Pin, 1);
}

void w5500_wr_b(unsigned char txByte)
{
  //unsigned char rtnByte;
  while (HAL_SPI_GetState(&hspi2) != HAL_SPI_STATE_READY);
  //HAL_SPI_TransmitReceive(&hspi1,&txByte,&rtnByte,1,10);

  HAL_SPI_Transmit(&W5500_SPI_CH, &txByte, 1, 10000); // 왕규
}

unsigned char w5500_rd_b(void)
{
 // unsigned char txByte = 0xff;//dummy
  unsigned char rtnByte;
  while (HAL_SPI_GetState(&hspi2) != HAL_SPI_STATE_READY);
  //HAL_SPI_TransmitReceive(&hspi1,&txByte,&rtnByte,1,10);

  HAL_SPI_Receive(&W5500_SPI_CH, &rtnByte, 1, 10000); // 왕규

  return rtnByte;
}

void w5500_cs(void)
{
  HAL_GPIO_WritePin(w5500_cs_GPIO_Port, w5500_cs_Pin, 0); //GPIO_PIN_RESET); //0
}

void w5500_ds(void)
{
  HAL_GPIO_WritePin(w5500_cs_GPIO_Port, w5500_cs_Pin, 1);//GPIO_PIN_SET);
}

int isLinked(void)
{
    return (getPHYCFGR() & PHYCFGR_LNK_ON);
}

void print_network_information(void)
{
    wizchip_getnetinfo(&gWIZNETINFO);
    printf("MAC: %02d:%02d:%02d:%02d:%02d:%02d\r\n",gWIZNETINFO.mac[0],gWIZNETINFO.mac[1],gWIZNETINFO.mac[2],
        		  gWIZNETINFO.mac[3],gWIZNETINFO.mac[4],gWIZNETINFO.mac[5]);
    printf("IP address : %d.%d.%d.%d\n\r",gWIZNETINFO.ip[0],gWIZNETINFO.ip[1],gWIZNETINFO.ip[2],gWIZNETINFO.ip[3]);
    printf("SM Mask    : %d.%d.%d.%d\n\r",gWIZNETINFO.sn[0],gWIZNETINFO.sn[1],gWIZNETINFO.sn[2],gWIZNETINFO.sn[3]);
    printf("Gate way   : %d.%d.%d.%d\n\r",gWIZNETINFO.gw[0],gWIZNETINFO.gw[1],gWIZNETINFO.gw[2],gWIZNETINFO.gw[3]);
	printf("DNS Server : %d.%d.%d.%d\n\r",gWIZNETINFO.dns[0],gWIZNETINFO.dns[1],gWIZNETINFO.dns[2],gWIZNETINFO.dns[3]);
}

/////////////////////////////////////////////////////////////
// Intialize the network information to be used in WIZCHIP //
/////////////////////////////////////////////////////////////
void network_init(void)
{
  	 uint8_t tmpstr[6];

/*
  	// target server/clent ip set
	Destination_Setup.destip[0] = 192;
	Destination_Setup.destip[1] = 168;
	Destination_Setup.destip[2] = 1;
	Destination_Setup.destip[3] = 3;

	// target server/clent port set
	Destination_Setup.port = 5000; // test 용 = user에서 받아야 함
*/

	ctlnetwork(CN_SET_NETINFO, (void*)&gWIZNETINFO);

	HAL_Delay(20);
	ctlnetwork(CN_GET_NETINFO, (void*)&gWIZNETINFO);

	// Display Network Information
	ctlwizchip(CW_GET_ID, (void*)tmpstr);

	printf("\r\n=== %s NET CONF ===\r\n", (char *)tmpstr);
	printf("MAC: %02d:%02d:%02d:%02d:%02d:%02d\r\n",gWIZNETINFO.mac[0],gWIZNETINFO.mac[1],gWIZNETINFO.mac[2],
	          gWIZNETINFO.mac[3],gWIZNETINFO.mac[4],gWIZNETINFO.mac[5]);
	printf("CIP: %d.%d.%d.%d\r\n", gWIZNETINFO.ip[0],gWIZNETINFO.ip[1],gWIZNETINFO.ip[2],gWIZNETINFO.ip[3]);
	printf("GAR: %d.%d.%d.%d\r\n", gWIZNETINFO.gw[0],gWIZNETINFO.gw[1],gWIZNETINFO.gw[2],gWIZNETINFO.gw[3]);
	printf("SUB: %d.%d.%d.%d\r\n", gWIZNETINFO.sn[0],gWIZNETINFO.sn[1],gWIZNETINFO.sn[2],gWIZNETINFO.sn[3]);
	printf("DNS: %d.%d.%d.%d\r\n", gWIZNETINFO.dns[0],gWIZNETINFO.dns[1],gWIZNETINFO.dns[2],gWIZNETINFO.dns[3]);
	printf("======================\r\n");
}

void w5500_init_test(void)
{
	// w5500 init & spi if test
	  uint32_t w5500_buf[10];

	  print_network_information();

	  // 버전정보읽기 = 버전레지스터 = 0x04 값
	  ret = getVERSIONR(); // w5500 reg 값리드 = 0x04가 읽히면 spi if ok
	  printf("W5500 칩 버전정보 0x04 이면 ok = %d\n\r", ret);

	  // RCR, RTR 레지스터 읽기 = 초기값 = RCR = 0x08;, RTR = 0x07d0;
	  w5500_buf[1] = getRCR();
	  w5500_buf[2] = getRTR();
	  printf("RCR = 0x08, RTR = 0x07D0 이면 OK = %x, %x\n\r", w5500_buf[1], w5500_buf[2]);

	  // RCR, RTR 레지스터 쓰고 읽기
	  setRCR(128);
	  setRTR(1234);
	  w5500_buf[1] = getRCR();
	  w5500_buf[2] = getRTR();
	  printf("RCR = 128, RTR = 1234 이면 OK = %d, %d\n\r", w5500_buf[1], w5500_buf[2]);
}


void w5500_init_my(void)
{
  uint8_t temp;
  uint8_t w5500_fifosize[2][8] = {{2,2,2,2,2,2,2,2},{2,2,2,2,2,2,2,2}};

  w5500_cs();

  // spi func reg
  reg_wizchip_spi_cbfunc(w5500_rd_b, w5500_wr_b);

  // cs func reg
  reg_wizchip_cs_cbfunc(w5500_cs, w5500_ds);

  if(ctlwizchip(CW_INIT_WIZCHIP,(void*)w5500_fifosize) == -1)
   {
	 printf("w5500 init fail\r\n");
   }

  do  // chk pye ststus
   {
	 if(ctlwizchip(CW_GET_PHYLINK,(void*)&temp) == -1)
	 {
       printf("UNKNOWN PHY Link status\n\r");
	 }
   }while(temp == PHY_LINK_OFF);
}


void Loopback_echo_server_test(void)
{
	  // Loopback echo server test = tcp-ip
	  if((ret = loopback_tcps(SOCK_TCPS0, gDATABUF, TCP_Port)) < 0) // ent_port = 5000
	    {
	   	  printf("SOCKET ERROR : %ld\r\n", ret);
	    }
}

void Loopback_echo_udp_test(void)
{
	   // Loopback echo server test = udp
	  if( (ret = loopback_udps(SOCK_UDPS, gDATABUF, PORT_UDPS)) < 0)  //3000
       {
		  printf("SOCKET ERROR : %ld\r\n", ret);
       }
}

void Loopback_echo_client_test(void)
{
	   // Loopback echo server test = udp
	  if( (ret = loopback_tcpc(SOCK_UDPS, gDATABUF, gWIZNETINFO.ip,  TCP_Port)) < 0) // ent_port = 5000
       {
		  printf("SOCKET ERROR : %ld\r\n", ret);
       }
}

//============================================

// Ver 2.0 통합 == 2023, 01, 08

// Ethernet ==>> Serial
void ent_to_uart(uint8_t *buf, uint16_t size_k)
{
	// pc : Server
	//w5500 : client

    uint8_t i;

    for(i=0 ; i < size_k ; i++)
     {
        tx_send(buf[i], 2);
     }
  printf("\r\n");
}

// Serial ==>> Ethernet
void uart_to_ent(uint8_t sn_k, uint16_t size)
{
  // pc : Server
  //w5500 : client

  //send ?븿?닔瑜? ?씠?슜?빐 SERIAL_BUF?쓽 ?뜲?씠?꽣瑜? TCP濡? ?쟾?넚?븳?떎.
  send(sn_k, rx_d -> Rx_data_2, size); //user, rx_cnt);

  /*
     user[0] = my_buf[0];
     user[1] = my_buf[2];
     user[2] = my_buf[4];
     user[3] = my_buf[6];
  */

   /*
	 sprintf(my_buf,"%s", rx_buf);
	 my_printf("my = %s\n\r",my_buf);
	 tx_send(my_buf[0], 1);
	 tx_send(my_buf[2], 1);
	*/
}

void client_call_back_tx(uint8_t sn_k, uint16_t size)
{
	// pc : Server
	//w5500 : client

  // call back func
  send(sn_k, gDATABUF, size);
}

void client_init(void)
{
	// pc : Server
	//w5500 : client

	uint8_t sn = SOCK_TCPS0;  //SOCK_TCPS0;
	//sn = SOCK_TCPS0;

k1:
  //Socket Creation
  if(socket(sn, Sn_MR_TCP, port, 0x00)==sn)
   {
	 printf("%d:Socket Opened\r\n",sn);
	 printf("%d:Connecting, Client-ip[%d.%d.%d.%d] port [%d]\r\n", sn, gWIZNETINFO.ip[0], gWIZNETINFO.ip[1], gWIZNETINFO.ip[2], gWIZNETINFO.ip[3], port);
	 printf("%d:Connecting, Server-ip[%d.%d.%d.%d] port [%d]\r\n", sn, server_ip[0], server_ip[1], server_ip[2], server_ip[3], port);
	}
   else
    {
	  printf("%d:Socket Error\r\n",sn);

	  //while(1);

	  HAL_Delay(100);
	  goto k1;
    }

    network_init();

    //Connent TCP Server
    if(connect(sn, server_ip, port)==SOCK_OK)
     {
    	printf("%d:Connect OK!!\r\n",sn);

    	printf("%d:Connecting, Client-ip[%d.%d.%d.%d] port [%d]\r\n", sn, gWIZNETINFO.ip[0], gWIZNETINFO.ip[1], gWIZNETINFO.ip[2], gWIZNETINFO.ip[3], port);
    	printf("%d:Connecting, Server-ip[%d.%d.%d.%d] port [%d]\r\n", sn, server_ip[0], server_ip[1], server_ip[2], server_ip[3], port);
     }
   else
      {
        printf("%d:Connect Error\r\n",sn);
        //while(1);
        HAL_Delay(100);
        goto k1;
      }
}


void client_rtx_chk(uint8_t sn)
{
	// PC : Server
	// W5500 : Client

	//uint8_t sn = SOCK_TCPS0;
	uint16_t size = 0;

	// W5500?쓽 Sn_SR ?젅吏??뒪?꽣瑜? ?씫?뼱?꽌 W5500?쓽 ?긽?깭瑜? ?솗?씤?븳?떎.
	// W5500?쓽 ?긽?깭?뿉 ?뵲?씪 ?룞?옉 ?삎?깭媛? 寃곗젙?맂?떎.
    switch(getSn_SR(sn))
      {
                case SOCK_CLOSED:
                	//W5500dl close 紐⑤뱶 ?씪硫? ?냼耳볦쓣 ?깮?꽦?븳?떎.
                	//?냼耳볦쓣 ?깮?꽦?븳?떎. ?냼耳볧븿?닔 ?쟾?떖 ?씤?옄?뒗 ("?냼耳? 踰덊샇, ?넻?떆 紐⑤뱶, ?룷?듃 踰덊샇, ?뵆?옒洹?)" ?씠?떎.
                	// Socket Closed State = ?냼耳볦쓣 ?깮?꽦 ?븿
                	 if(socket(sn, Sn_MR_TCP, port, 0x00)==sn)
                	   {
                	      printf("%d:Socket Opened\r\n",sn);
                	    }
                	  else
                	   {
                	      printf("%d:Socket Error\r\n",sn);
                	      // while(1);
                	   }
                    break;

                case SOCK_INIT:
                	 //?냼耳볦깮?꽦?씠 ?셿猷? ?릺硫?, W5500 init 紐⑤뱶媛? ?릺硫?,
                	 // init 紐⑤뱶 ?씪 ?븣 W5500?씠 server 紐⑤뱶濡? ?룞?옉?븷吏? client 紐⑤뱶濡? ?룞?옉?븷吏? ?젙?븯寃? ?맂?떎.
                     // TCP Socket Creatation = ?꽌踰꾩뿉 ?젒?냽 ?떆?룄?븿
                	  printf("%d:Connecting, Server-ip[%d.%d.%d.%d] port [%d]\r\n", sn, server_ip[0], server_ip[1], server_ip[2], server_ip[3], port);

                	  // connect ?븿?닔瑜? ?궗?슜?빐?꽌 TCP?꽌踰꾨줈 ?젒?냽?쓣 ?떆?룄 ?븳?떎.
                	  // connect ?븿?닔?쓽 ?쟾?떖 ?씤?옄?뒗 "(?꽌踰꾩쓽 ?냼耳볥쾲?샇, ?꽌踰꾩쓽 IP, ?꽌踰꾩쓽 ?룷?듃踰덊샇)" ?씠?떎.
                	  // connect ?븿?닔瑜? ?궗?슜?븯寃? ?릺硫? W5500?? client 紐⑤뱶媛? ?릺怨?,
                	  // listen ?븿?닔瑜? ?궗?슜?븯寃? ?릺硫? W5500?? server 紐⑤뱶媛? ?맂?떎.
                	  // listen 紐⑤뱶?뒗 W5500?씠 server濡? ?룞?옉?븷?븣 ?씠怨?,
                	  // client?쓽 ?젒?냽?쓣 湲곕떎由ш퀬 ?엳?쓣 ?긽?깭?씠?떎.

                	  //Connent TCP Server
                	  if(connect(sn, server_ip, port)==SOCK_OK)
                	  {
                		printf("Server Connecting OK\r\n");
                	  }
                	  else
                	   {
                	      printf("%d:Connect Error\r\n",sn);
                	     // while(1);
                	   }
                    break;

                case SOCK_LISTEN:
                    // TCP Server Mode


                    break;

                case SOCK_ESTABLISHED:
                    // TCP ESTABLISHED = ?뜲?씠?꽣 ?넚?닔?떊 媛??뒫
                	// TCP ?뿰寃곗씠 ?셿猷? ?맟?쓣 ?븣, established 紐⑤뱶媛? ?맂?떎.
                	// established ?씪?븣 server?? client?궗?씠?뿉 ?뜲?씠?꽣 ?쟾?넚?씠 媛??뒫?븯?떎.

                    //Connect Interrupt Check
                	// W5500?쓽 Sn_IR ?젅吏??뒪?꽣瑜? ?씫?뼱 connect ?씤?꽣?읇?듃瑜? ?솗?씤?븳?떎.
                	// established 紐⑤뱶濡? 蹂?寃쎈맟?쓣 ?븣 ?븳踰? ?씤?꽣?읇?듃媛? 諛쒖깮?븳?떎.
                	if(getSn_IR(sn) & Sn_IR_CON)
                     {
                       printf("%d:Connected-Server-PC\r\n",sn);
                       //Sn_IR ?젅吏??뒪?꽣?쓽 connect ?씤?꽣?읇?듃瑜? 珥덇린?솕 ?븳?떎.
                       setSn_IR(sn,Sn_IR_CON);
                     }

                    //Receive Data Check
                	// Sn_RX_RSR ?젅吏??뒪?꽣瑜? ?씫?뼱 W5500?쓽 RX 踰꾪띁?뿉 ???옣?맂 ?뜲?씠?꽣 ?궗?씠利덈?? ?씫?뼱
                	// size 蹂??닔(?닔?떊?븷 ?궗?씠利?)?뿉 ???옣?븳?떎.
                    if((size = getSn_RX_RSR(sn)) > 0)
                     {
                        if(size > DATA_BUF_SIZE) size = DATA_BUF_SIZE;
                        recv(sn,gDATABUF,size); // recv ?븿?닔瑜? ?씠?슜?빐 TCP ?뜲?씠?꽣瑜? ?닔?떊?븳?떎.

                        // ?뵒踰꾧렇 ?슜
                        ent_to_uart(gDATABUF, size);

                        // system command chk func
                       // com_chk();


                        // echo Loopbak test
                        client_call_back_tx(sn,size);
                      }
                    break;

                case SOCK_CLOSE_WAIT:
                     //Disconnect request

                    break;
            }

}

//rx func
void server_to_client_rx(void)
{
	uint8_t sn = SOCK_TCPS0;
	uint16_t size=0;

 if((size = getSn_RX_RSR(sn)) > 0)
  {
	if(size > DATA_BUF_SIZE) size = DATA_BUF_SIZE;
	  recv(sn,gDATABUF,size);
	  ent_to_uart(gDATABUF, size);
	  uart_to_ent(sn, size);
  }
}
//================================================

// Web Server Func
//2023, 1, 9 추가
// default Server 프로그램-1
void WebServer(uint8_t sn, uint8_t *buf, uint16_t port)
{
	// PC    : Client
	// W5500 : Server

	uint16_t size = 0, sentsize=0, i=10000;

	uint8_t destip[4];  // Client Debuging
	uint16_t destport;  // Client Debuging

	switch(getSn_SR(sn))
	   {
	     case SOCK_ESTABLISHED :
	        if(getSn_IR(sn) & Sn_IR_CON)
	         {
	            getSn_DIPR(sn, destip);

	            // debuging
	            destport = getSn_DPORT(sn);
	            printf("%d:Connected - %d.%d.%d.%d : %d\r\n",sn, destip[0], destip[1], destip[2], destip[3], destport);

	            setSn_IR(sn,Sn_IR_CON);
	         }

	       while(i){
	                 i--;
	               }

	        if((size = getSn_RX_RSR(sn)) > 0) // Don't need to check SOCKERR_BUSY because it doesn't not occur.
	         {
	            if(size > DATA_BUF_SIZE) size = DATA_BUF_SIZE;
	            ret = recv(sn, buf, size);

	            if(ret <= 0) return ret;      // check SOCKERR_BUSY & SOCKERR_XXX. For showing the occurrence of SOCKERR_BUSY.
	            sentsize = 0;

	            printf("HTTP Request received: \r\n%s\r\n", buf);
	           // proc_http(sn, (char *)buf);

	           ent_to_uart(buf, size);
	         }
	       break;

	     case SOCK_CLOSE_WAIT :
	         printf("%d:CloseWait\r\n",sn);
	         if((ret = disconnect(sn)) != SOCK_OK) return ret;
	         printf("%d:Socket Closed\r\n", sn);
	       break;

	     case SOCK_INIT :
	         printf("%d:Listen, Web Server, port [%d]\r\n", sn, port);
	         if( (ret = listen(sn)) != SOCK_OK) return ret;
	       break;

	     case SOCK_CLOSED:
	         printf("%d:Web Server start\r\n",sn);
	         if((ret = socket(sn, Sn_MR_TCP, port, 0x00)) != sn) return ret;
	         printf("%d:Socket opened\r\n",sn);
	       break;

	     default:

	       break;
	   }
}

// 2023, 1, 10 추가
// Server 프로그램 -2
int tcp_server(uint8_t sn, uint16_t port)
{
    int8_t ret;
    uint8_t dest_ip[4];  // = {192,168,30,3};
    uint16_t dest_port;  // = 5000;
    uint16_t size = 0;

    switch (getSn_SR(sn)) {

        case SOCK_CLOSED:
            /* open socket */
            printf("TCP server start\r\n");
            if ((ret = socket(sn, Sn_MR_TCP, port, 0x00)) != sn) {
                printf("socket %d open fail\r\n", sn);
                return ret;
            }
            printf("socket %d open success\r\n", sn);
            break;

        case SOCK_INIT:
            /* waiting for a client to connect */
            printf("listen %d port...\r\n", port);
            if ((ret = listen(sn)) != SOCK_OK) {
                printf("%d:listen fail\r\n", sn);
                return ret;
            }
            printf("%d:listen success\r\n", sn);
            break;
        case SOCK_ESTABLISHED:
            /* socket has been established */
            if(getSn_IR(sn) & Sn_IR_CON) {
                getSn_DIPR(sn, dest_ip);
                dest_port = getSn_DPORT(sn);
                printf("%d:a client connect success, %d.%d.%d.%d:%d\r\n", sn, dest_ip[0], dest_ip[1], dest_ip[2], dest_ip[3], dest_port);

                setSn_IR(sn,Sn_IR_CON);
            }

            // get the size of recv data in recv buffer
            if ((size = getSn_RX_RSR(sn)) > 0) {
                if (size > DATA_BUF_SIZE) {
                    size = DATA_BUF_SIZE;
                }

                //recv data
                ret = recv(sn, recv_buf, DATA_BUF_SIZE);
                if (ret <= 0) {
                	printf("%d:recv fail\r\n", sn);
                    return ret;
                } else {
                    // The actual received size
                    size = (uint16_t)ret;
                    printf("%d:recv size:%d\r\n", sn, size);
                    recv_buf[size] = '\0';
                    printf("%d:recv data:[%s]\r\n", sn, recv_buf);
                }

                //send resp data
                ret = send(sn, recv_buf, size);
                if (ret <= 0) {
                    printf("%d:send fail\r\n", sn);

                    //close the socket
                    close(sn);

                    return ret;
                } else {
                    // The actual sent size
                    size = (uint16_t)ret;
                    printf("%d:send size:%d\r\n", sn, size);
                    recv_buf[size] = '\0';
                    printf("%d:send data:[%s]\r\n", sn, recv_buf);
                }
            }
            break;

        case SOCK_CLOSE_WAIT:
            /* closing the socket */
            if ((ret = disconnect(sn)) != SOCK_OK) {
                printf("%d:disconnect fail\r\n", sn);
                return ret;
            }
            printf("%d: socket is closed\r\n", sn);
            break;

        default:
        	printf("song\r\n");
            break;
    }

    return 0;
}
//=============================================================================


/*
 //Test
	  // Serial => Ethernet
	                     if(rx_end_flag == 1)
	                     {
	                      uart_to_ent(sn, rx_cnt);
	                      rx_cnt = 0;
	                      rx_end_flag = 0;
	                     }
	             */

	  /*
	  server_to_client_rx();
    if(gDATABUF[0] == 0x0a)
     {
   	 my_printf("ok");
   	 gDATABUF[0] = 0;
     }
*/

/*
     w5500_ret();
     w5500_init_my();
     wizchip_setnetinfo(&gWIZNETINFO);
     print_network_information();
     w5500_cs();
     w5500_ret();
*/

// system test
   	// w5500_init_test();

/*
 // ��������� ���
// Socket Creatatiion
int8_t socket(uint8_t sn, uint8_t protocol, uint16_t port, uint8_t flag);

// Listen to a TCP client
int8_t listen(uint8_t sn);

// Connect to a server
int8_t connect(uint8_t sn);

// Send data to the peer
int32_t send(uint8_t sn, uint8_t* buf, uint32_t len);

// Receive data from the peer
int32_t recv(uint8_t sn, uint8_t* buf, uint32_t len);

// Disconnect the socket only used in TCP
int8_t disconnect(uint8_t sn);

// Close the socket without disconnect-processing
int8_t close(uint8_t sn);

*/


#endif /* W5500_FUNC_H_ */
