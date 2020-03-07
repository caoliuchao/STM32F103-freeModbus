/********************************************************************************
 * @版本  ：V1.0
 * @文件  : sysbsp.c
 * @作者  : caochao
 * @日期  : 9-19-2018
 * @版本  : version1.0
 * @单位  : 西安电子科技大学
 * @摘要  : 这个C文件时硬件的初始化
 *
 * 修改人 :
 *  日期  :
 *
 * 修改人 :
 *  日期  :
 
 ********************************************************************************/
#include "stm32f10x.h"
#include "includes.h"
#include "sysbsp.h"

/********************************************************************************
 *  函数名:  BSP_Init
 *  功  能:  系统硬件的初始化
 *  输  入:   
 *           无
 *  输  出:   
 *           true：初始化成功
 *          false：初始化失败
 ********************************************************************************/
bool SYSBSP_Init(void)
{ 
    eMBErrorCode    eStatus;
    
    /*初始化led*/
    LED1_Init();
    LED2_Init();

    /*初始化串口*/
    USART1_Configuration();
    
    //初始化freemodbus 设置RTU模式和ID等
    eStatus = eMBInit(MB_RTU, 0x0A,2, 38400, MB_PAR_NONE);
	if (MB_ENOERR != eStatus)
    {
        printf("freemodbus 设置RTU或模式失败\r\n");
        return false;
    }
    
    /* 激活协议栈以及串口和定时器而已。*/
    eStatus = eMBEnable();
    if (MB_ENOERR != eStatus)
    {
        printf("使能ModbusProtocolStack失败\r\n");
        return false;
    }
    
    return true;
}
/**************************************文件结束*************************************/

