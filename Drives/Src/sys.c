/***************************************************************************
* @文件  : sys.c
* @作者  : caochao
* @日期  : 10-1-2018
* @摘要  : 这个C文件systick设置的实现

* 修改人 :
*  日期  :
*
* 修改人 :
*  日期  :
**************************************************************************/                                                                                                       
#include "stm32f10x.h"
#include "sys.h"
#include "includes.h"

uint8_t  fac_us = 0;  //us延时倍乘数
uint16_t fac_ms = 0;  //ms延时倍乘数

/*
 **************************************************************************************
 *  函数名：Systick_Init(void)
 *  功  能：配置滴答时钟
 *  输  入：
 *         无
 *  输  出：
 *         无
 **************************************************************************************
 */
void Systick_Init(void)
{ 
#if SYSTEM_SUPPORT_OS	//如果SYSTEM_SUPPORT_OS不为零 则说明使用ucosII了.	
    uint32_t reload;    
#endif  
    
    SysTick->CTRL &= SysTick_CLKSource_HCLK_Div8;	//选择外部时钟  HCLK/8
    fac_us = SystemCoreClock/8000000;	          //Systick滴答1us的计数次数
    
#if SYSTEM_SUPPORT_OS	        //如果SYSTEM_SUPPORT_OS不为零 则说明使用ucosIII了.
    reload = fac_us * 1000000/OS_CFG_TICK_RATE_HZ ;//根据OS_CFG_TICK_RATE_HZ 设定溢出时间
							            //reload为24位寄存器,最大值:16777216,在72M下,约合1.86s左右	
    fac_ms = 1000/OS_CFG_TICK_RATE_HZ ;   //代表ucos可以延时的最少单位	
    
    SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;   	//开启SYSTICK中断
    SysTick->LOAD = reload - 1; 	  //每1/OS_TICKS_PER_SEC秒中断一次	
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;   	//开启SYSTICK    
#else 
	fac_ms = (uint16_t)fac_us*1000;//非ucos下,代表每个ms需要的systick时钟数
#endif     
    
}

/************************************文件结束************************************/


