/*
 * FreeModbus Libary: BARE Port
 * Copyright (C) 2006 Christian Walter <wolti@sil.at>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id$
 */

/* ----------------------- Platform includes --------------------------------*/
#include "port.h"
#include "stm32f10x.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

/* ----------------------- static functions ---------------------------------*/
static void prvvTIMERExpiredISR( void );

//当波特率 > 19200bps时，采用定时器时钟固定为1750us ,
//当波特率 < 19200bps时，定时器时钟为3.5个字符传输时间长
/* ----------------------- Start implementation -----------------------------*/
BOOL xMBPortTimersInit( USHORT usTim1Timerout50us )
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef        NVIC_InitStructure;  
    
    uint16_t PrescalerValue;
    
    //时基频率 / （1 + Prescaler) = 20KHz
    PrescalerValue = (uint16_t)((SystemCoreClock / 20000) - 1);
    
    MODBUS_TIME_CLOCK_API(MODBUS_TIME_PERIPH, ENABLE);
    
    TIM_TimeBaseStructure.TIM_Period = (uint16_t)usTim1Timerout50us;
    TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    
    TIM_TimeBaseInit(MODBUS_TIME_PORT, &TIM_TimeBaseStructure);

    TIM_ARRPreloadConfig(MODBUS_TIME_PORT, ENABLE); //使能预装
   
    //初始化中断优先级
    NVIC_InitStructure.NVIC_IRQChannel                   = MODBUS_TIME_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    TIM_ClearITPendingBit(MODBUS_TIME_PORT, TIM_IT_Update);
    TIM_ITConfig(MODBUS_TIME_PORT, TIM_IT_Update, DISABLE);
    
    TIM_Cmd(MODBUS_TIME_PORT, DISABLE);
      
    return TRUE;
}


inline void vMBPortTimersEnable(  )
{
    /* Enable the timer with the timeout passed to xMBPortTimersInit( ) */
    TIM_ClearITPendingBit(MODBUS_TIME_PORT, TIM_IT_Update);
    TIM_ITConfig(MODBUS_TIME_PORT, TIM_IT_Update, ENABLE);
    TIM_SetCounter(MODBUS_TIME_PORT, 0x00000000);
    
    TIM_Cmd(MODBUS_TIME_PORT, ENABLE);
}

inline void vMBPortTimersDisable(  )
{   
    TIM_ClearITPendingBit(MODBUS_TIME_PORT, TIM_IT_Update);
    TIM_ITConfig(MODBUS_TIME_PORT, TIM_IT_Update, DISABLE);
    TIM_SetCounter(MODBUS_TIME_PORT, 0x00000000);
    
    /* Disable any pending timers. */
    TIM_Cmd(MODBUS_TIME_PORT, DISABLE);
}

/* Create an ISR which is called whenever the timer has expired. This function
 * must then call pxMBPortCBTimerExpired( ) to notify the protocol stack that
 * the timer has expired.
 */
static void prvvTIMERExpiredISR( void )
{
    ( void )pxMBPortCBTimerExpired(  );
}

/*****************************************************************************
 *   函数名：MODBUS_TIME_PORT_IRQHandler
 *   功  能：定时器中断服务函数
 *   输  入：
 *          无
 *   输  出：
 *          无
 ****************************************************************************/
void MODBUS_TIME_IRQHandler(void)
{
#if SYSTEM_SUPPORT_OS	 	
	OSIntEnter();    
#endif
    
     if (TIM_GetITStatus(MODBUS_TIME_PORT, TIM_IT_Update) != RESET)
     {
         TIM_ClearITPendingBit(MODBUS_TIME_PORT, TIM_IT_Update);
         prvvTIMERExpiredISR();
     }
     
#if SYSTEM_SUPPORT_OS	 
	OSIntExit();  											 
#endif	
}
