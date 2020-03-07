/*************************************************************************
* @�ļ�  : led.h
* @����  : caochao
* @����  : 10-1-2018
* @ժҪ  : ���h�ļ�ʱ����led�ĸ�������
* �޸��� :
*  ����  :
*
* �޸��� :
*  ����  :
************************************************************************/
/* �����ֹ�ݹ�İ��� */
#ifndef _LED_H
#define _LED_H

#ifdef __cplusplus
extern "C"{
#endif
 
#define LED1_PORT     GPIOB
#define LED1_PIN      GPIO_Pin_5
#define LED1_PERIPH   RCC_APB2Periph_GPIOB	
    
#define LED2_PORT     GPIOE
#define LED2_PIN      GPIO_Pin_5
#define LED2_PERIPH   RCC_APB2Periph_GPIOE	
    
#define LED1_ON       GPIO_ResetBits(LED1_PORT,LED1_PIN)
#define LED1_OFF      GPIO_SetBits(LED1_PORT,LED1_PIN)	
#define LED2_ON       GPIO_ResetBits(LED2_PORT,LED2_PIN)
#define LED2_OFF      GPIO_SetBits(LED2_PORT,LED2_PIN)
		
void LED1_Init(void);
void LED2_Init(void);

#ifdef __cplusplus
}
#endif

#endif/*led.h*/
/*********************************�ļ�����*****************************/
