/*
*********************************************************************************************************
*                                     MICIRUM BOARD SUPPORT PACKAGE
*
*                             (c) Copyright 2013; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                     BOARD SUPPORT PACKAGE (BSP)
*
*                                       IAR Development Kits
*                                              on the
*
*                                    STM32F429II-SK KICKSTART KIT
*
* Filename      : bsp.h
* Version       : V1.00
* Programmer(s) : FF
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                                 MODULE
*
* Note(s) : (1) This header file is protected from multiple pre-processor inclusion through use of the
*               BSP present pre-processor macro definition.
*
*           (2) This file and its dependencies requires IAR v6.20 or later to be compiled.
*
*********************************************************************************************************
*/

#ifndef  __BSP_H
#define  __BSP_H

/*
*********************************************************************************************************
*                                                 EXTERNS
*********************************************************************************************************
*/

#ifdef   BSP_MODULE
#define  BSP_EXT
#else
#define  BSP_EXT  extern
#endif

#ifdef __cplusplus
 extern "C" {
#endif
 
#include  <stdio.h>
#include  <stdarg.h>
#include  <cpu.h>
#include  <cpu_core.h>
#include  <lib_def.h>
#include  <lib_ascii.h>
#include  <stm32f10x_conf.h>
     
/*
*********************************************************************************************************
*                             REGISTERS
*********************************************************************************************************
*/
#define  BSP_REG_DEM_CR                           (*(CPU_REG32 *)0xE000EDFC)
#define  BSP_REG_DWT_CR                           (*(CPU_REG32 *)0xE0001000)
#define  BSP_REG_DWT_CYCCNT                       (*(CPU_REG32 *)0xE0001004)
#define  BSP_REG_DBGMCU_CR                        (*(CPU_REG32 *)0xE0042004)
     
#define  BSP_DBGMCU_CR_TRACE_IOEN_MASK                 0x10u
#define  BSP_DBGMCU_CR_TRACE_MODE_ASYNC                0x00u
#define  BSP_DBGMCU_CR_TRACE_MODE_SYNC_01              0x40u
#define  BSP_DBGMCU_CR_TRACE_MODE_SYNC_02              0x80u
#define  BSP_DBGMCU_CR_TRACE_MODE_SYNC_04              0xC0u
#define  BSP_DBGMCU_CR_TRACE_MODE_MASK                 0xC0u

#define  BSP_BIT_DEM_CR_TRCENA                    0x01000000u
#define  BSP_BIT_DWT_CR_CYCCNTENA                 0x00000001u

/*-----------------------------------函数声明-------------------------------------*/     
CPU_INT32U  BSP_CPU_ClkFreq (void);
     
#if (CPU_CFG_TS_32_EN == DEF_ENABLED)
CPU_INT64U  CPU_TS32_to_uSec (CPU_TS32  ts_cnts);
#endif
     
#if (CPU_CFG_TS_64_EN == DEF_ENABLED)
CPU_INT64U  CPU_TS64_to_uSec (CPU_TS64  ts_cnts);
#endif
     
#if (CPU_CFG_TS_TMR_EN == DEF_ENABLED)
CPU_TS_TMR  CPU_TS_TmrRd (void);     
#endif   
     
#ifdef __cplusplus
 }
#endif
 
#endif /*End of module include */
 
/***************************************文件结束*************************************/
 
 
 

