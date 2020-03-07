/*************************************************************************
* @�ļ�  : usart.c
* @����  : caochao
* @����  : 10-1-2018
* @ժҪ  : ���h�ļ�ʱ����usart��printf������ʵ��
* �޸��� :
*  ����  :
*
* �޸��� :
*  ����  :
*************************************************************************/
#include <stdio.h>
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"
#include "usart.h"

/*--------�������´���,֧��printf����,������Ҫѡ��use MicroLIB---------------*/ 
#pragma import(__use_no_semihosting)              
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
}; 
/* FILE is typedef�� d in stdio.h. */ 
FILE __stdout;     

void _ttywrch(int ch)
{
    ch = ch;     
}

//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 

/**********************************************************************
*   ��������fputc(int ch,FILE *p)
*   ��  �ܣ�printf�����ض���
*   ��  �룺
*          ch Ҫ��ӡ���ַ�
*   ��  ����
***********************************************************************/
int fputc(int ch,FILE *p)
{
    while (USART_GetFlagStatus(USART1_PORT,USART_FLAG_TC) == RESET)
	{
		;
	}
    
    USART_SendData(USART1_PORT,(uint8_t)ch);
   
	return ch;
}

/***********************************************************************
*  ������:   USART1_Configuration
* ��������:  usart1�Ĺ�������
* ��    ��:   
*           �� 
* ��    ��:   
*           ��
***********************************************************************/
void USART1_Configuration(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    
    RCC_APB2PeriphClockCmd(USART1_TX_PERIPH,ENABLE);   //��GPIO��ʱ��
    RCC_APB2PeriphClockCmd(USART1_RX_PERIPH,ENABLE);   //��GPIO��ʱ��
    RCC_APB2PeriphClockCmd(USART1_PERIPH,ENABLE);      //��usart1ʱ��    
    
    GPIO_InitStructure.GPIO_Pin	= USART1_TX_PIN;         //USART��TXΪ�����������ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	
	GPIO_Init(USART1_TX_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin	= USART1_RX_PIN ;         //USART��RXΪ�����������ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	
	GPIO_Init(USART1_RX_PORT,&GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = USART1_BaudRate ;          //����������
	USART_InitStructure.USART_WordLength  = USART_WordLength_8b;     //��������Ϊ8λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;           //ֹͣλΪ1
	USART_InitStructure.USART_Parity = USART_Parity_No;              //��������żλ���
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   //��Ӳ����
    USART_InitStructure.USART_Mode =  USART_Mode_Rx | USART_Mode_Tx;  //����usart���ܺͷ�������
    
    USART_Init(USART1_PORT,&USART_InitStructure); //��ʼ��usart 
	
	USART_Cmd(USART1_PORT,ENABLE);  //����usart    
}

/*********************************�ļ�����*********************************/

