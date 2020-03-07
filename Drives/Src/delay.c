/********************************************************************************
 * @版本  ：V1.0
 * @文件  : delay.c
 * @作者  : caochao
 * @日期  : 9-19-2018
 * @单位  : 西安电子科技大学
 * @摘要  : 这个C文件包含了各个不同延时函数的实现
 *
 * 修改人 :
 *  日期  :
 *
 * 修改人 :
 *  日期  :
 *******************************************************************************/
#include "stm32f10x.h"
#include "delay.h"
#include "includes.h"

extern uint8_t  fac_us;  //us延时倍乘数
extern uint16_t fac_ms;  //ms延时倍乘数

#if SYSTEM_SUPPORT_OS	//SYSTEM_SUPPORT_OS不为零 使用了ucos

/************************************************************************************
*   函数名：Delay1us(uint32_t nus)
*   功  能：延时
*   输  入：
*           nus 为要延时的us数. 
*           注意： nus >= 0
*   输  出：
*           无
************************************************************************************/  								   
void Delay1us(uint32_t nus)
{		
	uint32_t ticks;
	uint32_t told;
    uint32_t tnow;
    uint32_t tcnt;
	uint32_t reload;	
	
    reload = SysTick->LOAD;     //LOAD的值	
	ticks = nus  * fac_us;   //需要的节拍数 		 
	tcnt = 0;
	told = SysTick->VAL;        	//刚进入时的计数器值
	while(1)
	{
		tnow = SysTick->VAL;	
		if (tnow != told)
		{	    
			if (tnow < told)
            {
                tcnt += told-tnow;//这里注意一下SYSTICK是一个递减的计数器就可以了.
            }
			else 
            {
                tcnt += reload-tnow+told;	 
            }                
			told = tnow;
			if (tcnt >= ticks)
            {
                break;//时间超过/等于要延迟的时间,则退出.
            }
		}  
	} 									    
}

/***********************************************************************************
*   函数名：Delay1ms(uint16_t nms)
*   功  能：延时
*   输  入：
*           nms 为要延时的ms数.
*           注意：nms >= 0
*   输  出：
*           无
**********************************************************************************/
void Delay1ms(uint16_t nms)
{	
    OS_ERR   ERR;
	if(OSRunning == true)   //如果os已经在跑了	    
	{		  
		if(nms >= fac_ms)    //延时的时间大于ucos的最少时间周期 
		{
   			OSTimeDly(nms/fac_ms,OS_OPT_TIME_DLY,&ERR);  //ucos延时
		}
        
		nms = nms % fac_ms;				//ucos已经无法提供这么小的延时了,采用普通方式延时    
	}
    
	Delay1us(nms * 1000);	//普通方式延时,此时ucos无法启动调度.
}

#else       //不用ucos时

/**********************************************************************************
*   函数名：Delay1us(uint32_t nus)
*   功  能：延时
*   输  入：
*           nus 为要延时的us数.	
*   输  出：
*           无
**********************************************************************************/
void Delay1us(uint32_t nus)
{			    	 
	SysTick->LOAD = nus * fac_us; //时间加载	  		 
	SysTick->VAL = 0x00;        //清空计数器
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;   //开始计数 
	do
    {
        ;
    }while(!( (SysTick ->CTRL) & (SysTick_CTRL_COUNTFLAG_Msk) ));  //等待时间到达  
    
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       //关闭计数器
	SysTick->VAL =0X00;       //清空计数器	 
}

/**********************************************************************************
*   函数名：Delay1ms(uint16_t nms)
*   功  能：延时
*   输  入：
*           nms 为要延时的ms数. 
*           注意 ：对72M条件下,nms <= 1864 	
*   输  出：
*           无
**********************************************************************************/
void Delay1ms(uint16_t nms)
{	 		  	  	   
	SysTick->LOAD = (uint32_t)nms * fac_ms;    //时间加载(SysTick->LOAD为24bit)
	SysTick->VAL =0x00;               //清空计数器
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;       //开始计数  
    
	do
    {
        ;
    }while(!( (SysTick ->CTRL) & (SysTick_CTRL_COUNTFLAG_Msk) ));  //等待时间到达 
    
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       //关闭计数器
	SysTick->VAL =0X00;       //清空计数器	  	    
} 

#endif

/************************************ 文件结束***************************************/

