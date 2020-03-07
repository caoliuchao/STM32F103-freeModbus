/********************************************************************************
 * @版本  ：V1.0
 * @文件  : task.c
 * @作者  : caochao
 * @日期  : 9-19-2018
 * @单位  : 西安电子科技大学
 * @摘要  : 这个h文件时对UCOSIII的任务堆栈大小和优先级的宏定义
 *
 * 修改人 :
 *  日期  :
 *
 * 修改人 :
 *  日期  :
 *******************************************************************************/
/* 防止重复编译 */
#ifndef TASK_H
#define TASK_H

#ifdef __cplusplus
 extern "C" {
#endif
 
#include "includes.h"
 
/*----------------------------START 任务---------------------------------------*/
#define START_TASK_PRIO      			30    //设置任务优先级 开始任务的优先级设置为最低
#define START_STK_SIZE  				128   //设置任务堆栈大小
     
/*----------------------------MODBUS任务----------------------------------------*/
#define MODBUS_TASK_PRIO                5//设置任务优先级
#define MODBUS_STK_SIZE                 128 //设置任务堆栈大
 
     
/*----------------------------LED1任务------------------------------------------*/
#define LED1_TASK_PRIO       			7    //设置任务优先级
#define LED1_STK_SIZE  		    	    64  //设置任务堆栈大小
     
/*-----------------------------LED2任务-----------------------------------------*/
#define LED2_TASK_PRIO       			8    //设置任务优先级
#define LED2_STK_SIZE  		    	    64  //设置任务堆栈大小

/*-----------------------------USART1-------------------------------------------*/
#define USART1_TASK_PRIO                10    //设置任务优先级
#define USART1_STK_SIZE                 128  //设置任务堆栈大小
     
/*-----------------------------HEAD-------------------------------------------*/
#define HEAD_TASK_PRIO                  11    //设置任务优先级
#define HEAD_STK_SIZE                   128  //设置任务堆栈大小       

/*---------------------------开始任务堆栈----------------------------------------*/
extern OS_TCB     TaskStartTCB;
extern __align(8) CPU_STK START_TASK_STK[START_STK_SIZE];   //创建任务堆栈空间	
 
void start_task(void *pdata);  //任务函数接口
void usart1_task(void *pdata); //任务函数接口
void led1_task(void *pdata);   //任务函数接口
void led2_task(void *pdata);   //任务函数接口
void head_task(void *pdata);   //任务函数接口
void modbus_task(void *pdata);   //任务函数接口

#ifdef __cplusplus
 }
#endif

#endif/*task.h*/
/*********************************文件结束**************************************/
 
 
 

