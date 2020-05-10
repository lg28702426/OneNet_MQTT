/*-------------------------------------------------*/
/*            ��γ����STM32ϵ�п�����              */
/*-------------------------------------------------*/
/*                                                 */
/*            ʵ��ɨ�谴�����ܵ�Դ�ļ�             */
/*                                                 */
/*-------------------------------------------------*/

#include "stm32f10x.h"  //������Ҫ��ͷ�ļ�
#include "delay.h"      //������Ҫ��ͷ�ļ�
#include "key.h"      //������Ҫ��ͷ�ļ�

/*-------------------------------------------------*/
/*����������ʼ��KEY��������                        */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void KEY_Init(void)
{	
	GPIO_InitTypeDef GPIO_InitStructure;                       //����һ������IO�Ľṹ��
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA , ENABLE);    //ʹ��PA�˿�ʱ��
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB , ENABLE); 
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_12;                 //׼������PA12
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;   		   //�������룬Ĭ��״̬�ǵ͵�ƽ
	GPIO_Init(GPIOA, &GPIO_InitStructure);            		   //����PA12
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_5;                 //׼������PA1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   		   //�������룬Ĭ��״̬�Ǹߵ�ƽ
	GPIO_Init(GPIOB, &GPIO_InitStructure);            		   //����PA1
} 
/*-------------------------------------------------*/
/*������������ɨ�躯��                             */
/*��  ������                                       */
/*����ֵ�����ް������£��еĻ����ĸ���������       */
/*-------------------------------------------------*/
char KEY_Scan(void)
{	 	  
	if(KEY1_IN_STA==1){                  //�жϿ���KEY1��IO�����ƽ���Ƿ��Ǹߵ�ƽ������ǽ���if	
		Delay_Ms(5);                     //��ʱ5ms����������������
		if(KEY1_IN_STA==1){              //����������Ӱ����ٴ��жϿ���KEY1��IO�����ƽ���Ƿ��Ǹߵ�ƽ������ǽ���if��˵������������	
			while(1){                    //�ȴ�����̧��
				if(KEY1_IN_STA==0){      //�жϿ���KEY1��IO�����ƽ���Ƿ��ǵ͵�ƽ������ǽ���if	
					Delay_Ms(5);         //��ʱ5ms����������������
					if(KEY1_IN_STA==0){  //����������Ӱ����ٴ��жϿ���KEY1��IO�����ƽ���Ƿ��ǵ͵�ƽ������ǽ���if��˵������̧����
						return 1;        //����1����ʾKEY1������
					}
				}
			}
		}
	}else if(KEY2_IN_STA==0){            //�жϿ���KEY2��IO�����ƽ���Ƿ��ǵ͵�ƽ������ǽ���if	
		Delay_Ms(5);                     //��ʱ5ms����������������
		if(KEY2_IN_STA==0){              //����������Ӱ����ٴ��жϿ���KEY2��IO�����ƽ���Ƿ��ǵ͵�ƽ������ǽ���if��˵������������	
			while(1){                    //�ȴ�����̧��
				if(KEY2_IN_STA==1){      //�жϿ���KEY2��IO�����ƽ���Ƿ��Ǹߵ�ƽ������ǽ���if
					Delay_Ms(5);         //��ʱ5ms����������������
					if(KEY2_IN_STA==1){  //����������Ӱ����ٴ��жϿ���KEY2��IO�����ƽ���Ƿ��Ǹߵ�ƽ������ǽ���if��˵������̧����
						return 2;        //����2����ʾKEY2������
					}
				}
			}
		}
	}	
	return 0;                            //���û�а������£�����0
}
/*-------------------------------------------------*/
/*�������������ⲿ�жϳ�ʼ������.                  */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/	   
void KEY_Exti_Init(void)
{
	NVIC_InitTypeDef   NVIC_InitStructure; //����һ������IO�Ľṹ��
	EXTI_InitTypeDef   EXTI_InitStructure; //����һ�������жϵĽṹ�� 

  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	          //ʹ��GPIO���ù���ʱ��
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource12);    //����KEY1-PA12�ж���
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource5);    //����KEY2�ж���

  	EXTI_InitStructure.EXTI_Line = EXTI_Line12;	                  //׼�������ⲿ12�ж���
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	          //�ⲿ�ж�
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;        //�����ش����ж�
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;                     //ʹ��
  	EXTI_Init(&EXTI_InitStructure);	 	                          //�����ⲿ�ж�12��
	
  	EXTI_InitStructure.EXTI_Line = EXTI_Line5;	                  //׼������KEY2�ⲿ1�ж���
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	          //�ⲿ�ж�
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;       //�½��ش����ж�
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;                     //ʹ��
  	EXTI_Init(&EXTI_InitStructure);	 	                          //�����ⲿ�ж�5��

		NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;			  //׼�������ⲿ�ж�12
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;     //��ռ���ȼ�3 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		      //�����ȼ�0
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			      //ʹ���ⲿ�ж�0ͨ��
  	NVIC_Init(&NVIC_InitStructure);                               //�����ⲿ�ж�0

    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			  //׼�������ⲿ�ж�5
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;     //��ռ���ȼ�3
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		      //�����ȼ�1
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				  //ʹ���ⲿ�ж�1ͨ��
  	NVIC_Init(&NVIC_InitStructure);                               //�����ⲿ�ж�1
}




















