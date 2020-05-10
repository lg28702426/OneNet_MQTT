/*-------------------------------------------------*/
/*            ��Ϊ����STM32ϵ�п�����              */
/*-------------------------------------------------*/
/*                                                 */
/*              ����Wifi���ܵ�ͷ�ļ�               */
/*                                                 */
/*-------------------------------------------------*/

#ifndef __WIFI_H
#define __WIFI_H

#include "usart2.h"	    //������Ҫ��ͷ�ļ�

#define RESET_IO(x)    GPIO_WriteBit(GPIOB, GPIO_Pin_9, (BitAction)x)  //PA4����WiFi�ĸ�λ
//#define RESETU_IO(x)    GPIO_WriteBit(GPIOB, GPIO_Pin_8, (BitAction)x)

//#define     macESP8266_CH_ENABLE()                 GPIO_SetBits (  GPIOB,  GPIO_Pin_8 )
//#define     macESP8266_CH_DISABLE()                GPIO_ResetBits ( GPIOB, GPIO_Pin_8 )


#define WiFi_printf       u2_printf           //����2���� WiFi
#define WiFi_RxCounter    Usart2_RxCounter    //����2���� WiFi
#define WiFi_RX_BUF       Usart2_RxBuff       //����2���� WiFi
#define WiFi_RXBUFF_SIZE  USART2_RXBUFF_SIZE  //����2���� WiFi




#define SSID   "aaaa"                     //·����SSID����
#define PASS   "12345678"                 //·��������



void WiFi_ResetIO_Init(void);
char WiFi_SendCmd(char *cmd, int timeout);
char WiFi_Reset(int timeout);
char WiFi_JoinAP(int timeout);
char WiFi_Connect_Server(int timeout);
char WiFi_Smartconfig(int timeout);
char WiFi_WaitAP(int timeout);
char WiFi_Connect_IoTServer(void);


#endif


