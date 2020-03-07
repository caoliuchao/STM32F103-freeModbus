/*************************************************************************
* @文件  : led.h
* @作者  : caochao
* @日期  : 10-1-2018
* @摘要  : 这个h文件时声明led的各个函数
* 修改人 :
*  日期  :
*
* 修改人 :
*  日期  :
************************************************************************/
/* 定义防止递归的包容 */
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
/*********************************文件结束*****************************/
