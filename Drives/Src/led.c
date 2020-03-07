/*************************************************************************
* @�ļ�  : led.c
* @����  : caochao
* @����  : 10-1-2018
* @ժҪ  : ���C�ļ�ʱʵ��led�ĳ�ʼ��
* �޸��� :
*  ����  :
*
* �޸��� :
*  ����  :
**************************************************************************/
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "led.h"

/**************************************************************************
*   ��������LED1_Init(void)
*   ��  �ܣ�LED1�ܽŵĳ�ʼ��
*   ��  �룺
*           ��
*   ��  ����
*           ��
**************************************************************************/
void LED1_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	
    RCC_APB2PeriphClockCmd(LED1_PERIPH,ENABLE);  //��GPIOE��ʱ��
    
    GPIO_InitStructure.GPIO_Pin	= LED1_PIN;           //GPIOE.5Ϊ�������ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	
	GPIO_Init(LED1_PORT,&GPIO_InitStructure);
}	

/************************************************************************
*   ��������LED2_Init(void)
*   ��  �ܣ�LED2�ܽŵĳ�ʼ��
*   ��  �룺
*           ��
*   ��  ����
*           ��
************************************************************************/
void LED2_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	
    RCC_APB2PeriphClockCmd(LED2_PERIPH,ENABLE);       //��GPIOE��ʱ��
    
    GPIO_InitStructure.GPIO_Pin	= LED2_PIN;       //GPIOE.5Ϊ�������ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	
	GPIO_Init(LED2_PORT,&GPIO_InitStructure);
}	

/******************************�ļ�����***********************************/


