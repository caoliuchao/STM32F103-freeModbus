/**********************************************************************
* @�ļ�  : usart.h
* @����  : caochao
* @����  : 10-1-2018
* @ժҪ  : ���h�ļ�ʱ����usart��printf����������
* �޸��� :
*  ����  :
*
* �޸��� :
*  ����  :
***********************************************************************/
/* �����ֹ�ݹ�İ��� */
#ifndef _USART_H
#define _USART_H

#ifdef __cplusplus
 extern "C" {
#endif

#include <stdio.h>
	
/*-------------------------USART����غ궨��----------------------------*/
#define  USART1_PORT         USART1
#define  USART1_PERIPH       RCC_APB2Periph_USART1
#define  USART1_BaudRate     115200

#define  USART1_TX_PORT      GPIOA
#define  USART1_TX_PIN       GPIO_Pin_9
#define  USART1_TX_PERIPH    RCC_APB2Periph_GPIOA

#define  USART1_RX_PORT      GPIOA
#define  USART1_RX_PIN       GPIO_Pin_10
#define  USART1_RX_PERIPH    RCC_APB2Periph_GPIOA    

/*--------------------------����������----------------------------------*/ 
int fputc(int ch,FILE *p); 
void USART1_Configuration(void);	 
    
#ifdef __cplusplus
}
#endif

#endif /* _USART_H */
/*************************************�ı�����**************************/

