/*-----------------------------------------------------*/
/*              ��Ϊ����STM32ϵ�п�����                */
/*-----------------------------------------------------*/
/*                     ����ṹ                        */
/*-----------------------------------------------------*/
/*USER     �����������main��������������������      */
/*HARDWARE ��������������ֹ����������������          */
/*CORE     ������STM32�ĺ��ĳ��򣬹ٷ��ṩ�����ǲ��޸� */
/*STLIB    ���ٷ��ṩ�Ŀ��ļ������ǲ��޸�              */
/*-----------------------------------------------------*/
/*                                                     */
/*           ����main��������ں���Դ�ļ�              */
/*                                                     */
/*-----------------------------------------------------*/

#include "stm32f10x.h"  //������Ҫ��ͷ�ļ�
#include "main.h"       //������Ҫ��ͷ�ļ�
#include "delay.h"      //������Ҫ��ͷ�ļ�
#include "usart1.h"     //������Ҫ��ͷ�ļ�
#include "iic.h"        //������Ҫ��ͷ�ļ�
//#include "24c02.h" 			//������Ҫ��ͷ�ļ�
#include "usart2.h"     //������Ҫ��ͷ�ļ�
#include "timer1.h"     //������Ҫ��ͷ�ļ�
#include "timer3.h"     //������Ҫ��ͷ�ļ�
#include "timer4.h"     //������Ҫ��ͷ�ļ�
#include "wifi.h"	    	//������Ҫ��ͷ�ļ�
#include "led.h"        //������Ҫ��ͷ�ļ�
#include "mqtt.h"       //������Ҫ��ͷ�ļ�
#include "key.h"        //������Ҫ��ͷ�ļ�
#include "bsp_tempad.h"
#include "timer2.h"
#include "bsp_dht11.h"

char *CMD1 = "open_led";    //���ؿ������״̬��ת����->�� ��->��  
//char *CMD2 = "APP+OneSW=?";    //���ز�ѯ����ظ���������ǰ����״̬
//char *CMD3 = "APP+OneSWCD";    //���ÿ��ص���ʱ������ʱʱ�䵽��ʱ�򣬿���״̬��ת
int   CDTime= 0;               //��¼���ص���ʱʱ��
extern __IO u16 ADC_ConvertedValue;
u16 Current_Temp;	  //����ʹ��ADC��оƬ�ڲ��¶�
DHT11_Data_TypeDef DHT11_Data;
int main(void) 
{	
	//
	Delay_Init();                   //��ʱ���ܳ�ʼ��              
	Usart1_Init(9600);              //����1���ܳ�ʼ����������9600
	Usart2_Init(115200);            //����2���ܳ�ʼ����������115200	
	TIM4_Init(300,7200);            //TIM4��ʼ������ʱʱ�� 300*7200*1000/72000000 = 30ms
  LED_Init();	                    //LED��ʼ��
//	Temp_ADC1_Init();
   DHT11_Init();

//KEY_Init();	//������ʼ��
	 
	WiFi_ResetIO_Init();            //��ʼ��WiFi�ĸ�λIO
  MQTT_Buff_Init();               //��ʼ������,����,�������ݵ� ������ �Լ���״̬����
	OneNetIoT_Parameter_Init();	    //��ʼ������OneNet��IoTƽ̨MQTT�������Ĳ���	
	while(1)                        //��ѭ��
	{		
		
	     Current_Temp=(V25-ADC_ConvertedValue)/AVG_SLOPE+25;
		//	printf("\r\n The IC current tem= %3d ��\r\n", Current_Temp);	      

		/*--------------------------------------------------------------------*/
		/*   Connect_flag=1ͬ����������������,���ǿ��Է������ݺͽ���������    */
		/*--------------------------------------------------------------------*/
		if(Connect_flag==1){     
			/*-------------------------------------------------------------*/
			/*                     �����ͻ���������                      */
			/*-------------------------------------------------------------*/
				if(MQTT_TxDataOutPtr != MQTT_TxDataInPtr){                //if�����Ļ���˵�����ͻ�������������
				//3������ɽ���if
				//��1�֣�0x10 ���ӱ���
				//��2�֣�0x82 ���ı��ģ���ConnectPack_flag��λ����ʾ���ӱ��ĳɹ�
				//��3�֣�SubcribePack_flag��λ��˵�����ӺͶ��ľ��ɹ����������Ŀɷ�
				if((MQTT_TxDataOutPtr[1]==0x10)||((MQTT_TxDataOutPtr[1]==0x82)&&(ConnectPack_flag==1))||(SubcribePack_flag==1)){    
					u1_printf("��������:0x%x\r\n",MQTT_TxDataOutPtr[1]);  //������ʾ��Ϣ
					MQTT_TxData(MQTT_TxDataOutPtr);                       //��������
					MQTT_TxDataOutPtr += BUFF_UNIT;                       //ָ������
					if(MQTT_TxDataOutPtr==MQTT_TxDataEndPtr)              //���ָ�뵽������β����
						MQTT_TxDataOutPtr = MQTT_TxDataBuf[0];            //ָ���λ����������ͷ
				} 				
			}//�����ͻ��������ݵ�else if��֧��β
			
			/*-------------------------------------------------------------*/
			/*                     ������ջ���������                      */
			/*-------------------------------------------------------------*/
			if(MQTT_RxDataOutPtr != MQTT_RxDataInPtr){  //if�����Ļ���˵�����ջ�������������														
				u1_printf("���յ�����:");
				/*-----------------------------------------------------*/
				/*                    ����CONNACK����                  */
				/*-----------------------------------------------------*/				
				//if�жϣ����һ��������4���ֽڣ���һ���ֽ���0x20����ʾ�յ�����CONNACK����
				//��������Ҫ�жϵ�4���ֽڣ�����CONNECT�����Ƿ�ɹ�
				if((MQTT_RxDataOutPtr[0]==4)&&(MQTT_RxDataOutPtr[1]==0x20)){             			
				    switch(MQTT_RxDataOutPtr[4]){					
						case 0x00 : u1_printf("CONNECT���ĳɹ�\r\n");                            //���������Ϣ	
								    ConnectPack_flag = 1;                                        //CONNECT���ĳɹ������ı��Ŀɷ�
									break;                                                       //������֧case 0x00                                              
						case 0x01 : u1_printf("�����Ѿܾ�����֧�ֵ�Э��汾��׼������\r\n");     //���������Ϣ
									Connect_flag = 0;                                            //Connect_flag���㣬��������
									break;                                                       //������֧case 0x01   
						case 0x02 : u1_printf("�����Ѿܾ������ϸ�Ŀͻ��˱�ʶ����׼������\r\n"); //���������Ϣ
									Connect_flag = 0;                                            //Connect_flag���㣬��������
									break;                                                       //������֧case 0x02 
						case 0x03 : u1_printf("�����Ѿܾ�������˲����ã�׼������\r\n");         //���������Ϣ
									Connect_flag = 0;                                            //Connect_flag���㣬��������
									break;                                                       //������֧case 0x03
						case 0x04 : u1_printf("�����Ѿܾ�����Ч���û��������룬׼������\r\n");   //���������Ϣ
									Connect_flag = 0;                                            //Connect_flag���㣬��������						
									break;                                                       //������֧case 0x04
						case 0x05 : u1_printf("�����Ѿܾ���δ��Ȩ��׼������\r\n");               //���������Ϣ
									Connect_flag = 0;                                            //Connect_flag���㣬��������						
									break;                                                       //������֧case 0x05 		
						default   : u1_printf("�����Ѿܾ���δ֪״̬��׼������\r\n");             //���������Ϣ 
									Connect_flag = 0;                                            //Connect_flag���㣬��������					
									break;                                                       //������֧case default 								
					}				
				}			
				//if�жϣ����һ��������5���ֽڣ���һ���ֽ���0x90����ʾ�յ�����SUBACK����
				//��������Ҫ�ж϶��Ļظ��������ǲ��ǳɹ�
				else if((MQTT_RxDataOutPtr[0]==5)&&(MQTT_RxDataOutPtr[1]==0x90)){ 
						switch(MQTT_RxDataOutPtr[5]){					
						case 0x00 :
						case 0x01 : u1_printf("���ĳɹ�\r\n");            //���������Ϣ
							        SubcribePack_flag = 1;                //SubcribePack_flag��1����ʾ���ı��ĳɹ����������Ŀɷ���
									Ping_flag = 0;                        //Ping_flag����
   								    TIM3_ENABLE_30S();                    //����30s��PING��ʱ��
						          TIM2_ENABLE_2S();
						          Tempeature_State();
									break;                                //������֧                                             
						default   : u1_printf("����ʧ�ܣ�׼������\r\n");  //���������Ϣ 
									Connect_flag = 0;                     //Connect_flag���㣬��������
									break;                                //������֧ 								
					}					
				}
				//if�жϣ����һ��������2���ֽڣ���һ���ֽ���0xD0����ʾ�յ�����PINGRESP����
				else if((MQTT_RxDataOutPtr[0]==2)&&(MQTT_RxDataOutPtr[1]==0xD0)){ 
					u1_printf("PING���Ļظ�\r\n"); 		  //���������Ϣ 
					if(Ping_flag==1){                     //���Ping_flag=1����ʾ��һ�η���
						 Ping_flag = 0;    				  //Ҫ���Ping_flag��־
					}else if(Ping_flag>1){ 				  //���Ping_flag>1����ʾ�Ƕ�η����ˣ�������2s����Ŀ��ٷ���
						Ping_flag = 0;     				  //Ҫ���Ping_flag��־
						TIM3_ENABLE_30S(); 				  //PING��ʱ���ػ�30s��ʱ��
					}				
				}	
				//if�жϣ������һ���ֽ���0x30����ʾ�յ����Ƿ�������������������
				//����Ҫ��ȡ��������
				else if(MQTT_RxDataOutPtr[1]==0x30){ 
					u1_printf("�������ȼ�0����\r\n"); 		   		//���������Ϣ 
					MQTT_DealPushdata_Qs0(MQTT_RxDataOutPtr);   //����ȼ�0��������
				}				
								
				MQTT_RxDataOutPtr += BUFF_UNIT;                     //ָ������
				if(MQTT_RxDataOutPtr==MQTT_RxDataEndPtr)            //���ָ�뵽������β����
					MQTT_RxDataOutPtr = MQTT_RxDataBuf[0];          //ָ���λ����������ͷ                        
			}//������ջ��������ݵ�else if��֧��β
			
			/*-------------------------------------------------------------*/
			/*                     ���������������                      */
			/*-------------------------------------------------------------*/
			if(MQTT_CMDOutPtr != MQTT_CMDInPtr){                             //if�����Ļ���˵�����������������			
				MQTT_CMDOutPtr[MQTT_CMDOutPtr[0]+1] = '\0';                  //�����ַ���������        
				u1_printf("����:%s\r\n",&MQTT_CMDOutPtr[1]);                 //���������Ϣ
				if(!memcmp(&MQTT_CMDOutPtr[1],CMD1,strlen(CMD1))){           //�ж�ָ������CMD1
					LED1_OUT(!LED1_IN_STA);                                  //����1״̬��ת ���������Ϩ�𣬷�֮���Ϩ��͵���
				//	LED_CD_State();                                          //�жϿ��غ͵���ʱ״̬����������������
				}
//				else if(!memcmp(&MQTT_CMDOutPtr[1],CMD2,strlen(CMD2))){     //�ж�ָ������CMD2���ظ�����״̬
//				//	LED_CD_State();                                          //�жϿ��غ͵���ʱ״̬����������������				
//				}else if(!memcmp(&MQTT_CMDOutPtr[1],CMD3,strlen(CMD3))){     //�ж�ָ������CMD3�����õ���ʱʱ�� 
//					//CDTime = MQTT_CMDOutPtr[12];                             //��¼����ʱʱ��
//					if(CDTime!=0){                                           //���CDTime������0������if
//						u1_printf("����ʱ:%d����\r\n",CDTime);               //���������Ϣ
//						TIM1_ENABLE_60S();                                   //������ʱ��1 60s�Ӷ�ʱ
//					}else{                                                   //��֮CDTime����0������else��ȡ����ʱ
//					    u1_printf("ȡ������ʱ\r\n");                         //���������Ϣ
//						TIM_Cmd(TIM1,DISABLE);                               //�ر�TIM1
//					}
//					LED_CD_State();                                          //�жϿ��غ͵���ʱ״̬����������������							
//				}else u1_printf("δָ֪��\r\n");                             //���������Ϣ
				MQTT_CMDOutPtr += BUFF_UNIT;                             	 //ָ������
				if(MQTT_CMDOutPtr==MQTT_CMDEndPtr)           	             //���ָ�뵽������β����
					MQTT_CMDOutPtr = MQTT_CMDBuf[0];          	             //ָ���λ����������ͷ				
			}//��������������ݵ�else if��֧��β	
		}//Connect_flag=1��if��֧�Ľ�β
		
		/*--------------------------------------------------------------------*/
		/*      Connect_flag=0ͬ�������Ͽ�������,����Ҫ�������ӷ�����         */
		/*--------------------------------------------------------------------*/
		else{ 
			u1_printf("��Ҫ���ӷ�����\r\n");                 //���������Ϣ
			TIM_Cmd(TIM4,DISABLE);                           //�ر�TIM4 
			TIM_Cmd(TIM3,DISABLE);                           //�ر�TIM3  
			WiFi_RxCounter=0;                                //WiFi������������������                        
			memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);          //���WiFi���ջ����� 
			if(WiFi_Connect_IoTServer()==0){   			     //���WiFi�����Ʒ�������������0����ʾ��ȷ������if
				u1_printf("����TCP���ӳɹ�\r\n");            //���������Ϣ
				Connect_flag = 1;                            //Connect_flag��1����ʾ���ӳɹ�	
				WiFi_RxCounter=0;                            //WiFi������������������                        
				memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);      //���WiFi���ջ����� 
				MQTT_Buff_ReInit();                          //���³�ʼ�����ͻ�����                    
			}				
		}
	}
}
/*-------------------------------------------------*/
/*���������жϿ��غ͵���ʱ״̬����������������     */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
//void LED_CD_State(void)
//{
////	int i=0;
//	char temp[20];                  		//����һ����ʱ������
//	//int ww[20]={1,2,3,4,5,6,7,9};
//	
//	memset(temp,0,20);             		    //�����ʱ������
//	//sprintf(temp,"tem");          	    //�����ظ�����	
//	
//	if(LED1_IN_STA) temp[13] = '0';  		//���LED1�Ǹߵ�ƽ��˵����Ϩ��״̬������1״̬λ��0
//	else            temp[13] = '1';			//��֮��˵���ǵ���״̬������1״̬λ��1	
//	temp[14] = CDTime;                       //����ʣ�൹��ʱʱ��
//// sprintf(temp,"CurrentHumidity:%d.CurrentTemperature:%d",10,20);
//	MQTT_PublishQs0(P_TOPIC_NAME,temp,0);   //������ݣ�������������	

////   	sprintf(temp,"{\"method\":%d}",Current_Temp); 
////		MQTT_PublishQs0(P_TOPIC_NAME,temp,strlen(temp));   //������ݣ�������������	
////	u1_printf("\r\n The IC current tem= %3d ��\r\n", Current_Temp);	      

//	
//	
//	 //�����ظ�ʪ���¶�����
//}

 void Tempeature_State(void)
	 {
   char temp[256];
	memset(temp,0,256);
	while(1){
		 DHT11_Read_TempAndHumidity(&DHT11_Data);
		if(DHT11_Read_TempAndHumidity(&DHT11_Data)==SUCCESS)
			{
				u1_printf("ʪ�ȣ�%d.%d,�¶ȣ�%d.%d",DHT11_Data.humi_int,DHT11_Data.humi_deci,DHT11_Data.temp_int,DHT11_Data.temp_deci);	
				break; 
		}else{
		//	u1_printf("��ȡ�¶�ʧ��");
		}
      
	}
	sprintf(temp,"{\"temperature\":%d.%d,\"humity\":%d.%d}",DHT11_Data.temp_int,DHT11_Data.temp_deci,DHT11_Data.humi_int,DHT11_Data.humi_deci); 
		MQTT_PublishQs0(P_TOPIC_NAME,temp,strlen(temp));

}



