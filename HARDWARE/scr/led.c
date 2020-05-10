/*-------------------------------------------------*/
/*            ��Ϊ����STM32ϵ�п�����              */
/*-------------------------------------------------*/
/*                                                 */
/*              ʵ��LED���ܵ�Դ�ļ�                */
/*                                                 */
/*-------------------------------------------------*/

#include "stm32f10x.h"  //������Ҫ��ͷ�ļ�
#include "led.h"        //������Ҫ��ͷ�ļ�

/*-------------------------------------------------*/
/*����������ʼ��LED����                            */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void LED_Init(void)
{    	 
	GPIO_InitTypeDef GPIO_InitStructure;                     //����һ������GPIO�ı���
		
	//RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE);                                //ʹ��GPIOA�˿�ʱ��
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE);                                //ʹ��GPIOB�˿�ʱ��
	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;                                             //׼������PA5
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                                     //IO����50Mhz
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   		                              //���������ʽ
//	GPIO_Init(GPIOA, &GPIO_InitStructure);            		                              //����PA5
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_5;                //׼������PB0
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                                     //IO����50Mhz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   		                              //���������ʽ
	GPIO_Init(GPIOB, &GPIO_InitStructure);            		                              //����PB1 PB10 PB11
	
	LED_AllOff();   //����LEDϨ��
}

/*-------------------------------------------------*/
/*����������������LED                              */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void LED_AllOn(void)
{
	  LED1_ON;
//	LED2_ON;
	LED3_ON;
	
}

/*-------------------------------------------------*/
/*��������Ϩ������LED                              */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void LED_AllOff(void)
{
   LED1_OFF;
//	LED2_OFF;
	LED3_OFF;
	
}



