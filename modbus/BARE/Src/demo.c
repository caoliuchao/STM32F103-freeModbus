/*
 * FreeModbus Libary: BARE Demo Application
 * Copyright (C) 2006 Christian Walter <wolti@sil.at>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id$
 */

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "demo.h"
#include "mbport.h"
#include "mbutils.h"
#include "includes.h"

//功能码可以分为位操作和字操作两类。位操作的最小单位为BIT，字操作的最小单位为两个字节。
//[位操作指令] 读线圈状态01H，读(离散)输入状态02H，写单个线圈06H和写多个线圈0FH。
//[字操作指令] 读保持寄存器03H，写单个寄存器06H，写多个保持寄存器10H。
///Modbus中，数据可以分为两大类，分别为Coil和Register，每一种数据，
//根据读写方式的不同，又可细分为两种（只读，读写）。Modbus四种数据类型：
//Discretes Input     位变量     `   只读
//Coils               位变量         读写
//Input Registers     16-bit类型     只读
//Holding Registers   16-bit类型     读写
 
/*线圈    输出端口。可设定端口的输出状态，也可以读取该位的输出状态。 DO           电磁阀输出，MOSFET输出，LED显示等。
 状态    可分为两种不同的执行状态，例如保持型或边沿触发型。        数字量输出 
       
 离散     输入端口。通过外部设定改变输入状态，可读但不可写     DI                拨码开关，接近开关等。                                               
 输入状态                                                  数字量输入

 保持    输出参数或保持参数，控制器运行时被设定的某些参数。     AO               模拟量输出设定值，PID运行参数，变量阀输出大小，
 寄存器  可读可写                                           模拟量输出        传感器报警上限下限

 输入    输入参数。控制器运行时从外部设备获得的参数            AI
 寄存器  可读但不可写。                                      模拟量输入        模拟量输入*/

/* ----------------------- Static variables ---------------------------------*/
USHORT  usRegInputStart = REG_INPUT_START;
USHORT  usRegInputBuf[REG_INPUT_NREGS] = {0x0101,0x0202,0x0303,0x0404,0x0505,
                                          0x0606,0x0707,0x0808,0x0909,0x0A0A};
        
USHORT  usRegHoldingStart = REG_HOLDING_START;
USHORT  usRegHoldingBuf[REG_HOLDING_NREGS] = {0x0101,0x0202,0x0303,0x0404,0x0505,
                                              0x0606,0x0707,0x0808,0x0909,0x0A0A};
        
UCHAR   usRegCoilsStart = REG_COILS_START;
UCHAR   ucRegCoilsBuf[REG_COILS_NREGS] = {0x1F,0x02,0x83,0x04,0x05,0x06,0x07,0x08,0x09,0x0A};
        
UCHAR   usRegDiscreteStart = REG_DISCRETE_START;
UCHAR   ucRegDiscreteBuf[REG_DISCRETE_NREGS] = {0x1F,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A};

/* ----------------------- Start implementation -----------------------------*/
eMBErrorCode eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    int             iRegIndex;

    if( ( usAddress >= REG_INPUT_START ) && ( usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS ) )
    {
        iRegIndex = ( int )( usAddress - usRegInputStart );
        while( usNRegs > 0 )
        {
            *pucRegBuffer++ =
                ( unsigned char )( usRegInputBuf[iRegIndex] >> 8 );
            *pucRegBuffer++ =
                ( unsigned char )( usRegInputBuf[iRegIndex] & 0xFF );
            iRegIndex++;
            usNRegs--;
        }
        
        /*
         *增加自己的代码，如果使用RTOS可以通过信号量通知相应的任务指执行
         */
    }
    else
    {
        eStatus = MB_ENOREG;
    }

    return eStatus;
}

eMBErrorCode eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs,eMBRegisterMode eMode )
{
    eMBErrorCode eStatus = MB_ENOERR;
    int16_t iRegIndex;
    
    if( ( (int16_t)usAddress >= REG_HOLDING_START ) && ( usAddress + usNRegs <= REG_HOLDING_START + REG_HOLDING_NREGS ) )
    {
        iRegIndex = ( int16_t )( usAddress - REG_HOLDING_START );
    
        switch ( eMode )
        {
        case MB_REG_READ:
            while( usNRegs > 0 )
            {
                *pucRegBuffer++ = ( uint8_t )( usRegHoldingBuf[iRegIndex] >> 8 );
                *pucRegBuffer++ = ( uint8_t )( usRegHoldingBuf[iRegIndex] & 0xFF );
                iRegIndex++;
                usNRegs--;
            }
            break;
    
        case MB_REG_WRITE:
            while( usNRegs > 0 )
            {
                usRegHoldingBuf[iRegIndex] = *pucRegBuffer++ << 8;
                usRegHoldingBuf[iRegIndex] |= *pucRegBuffer++;
                iRegIndex++;
                usNRegs--;
            }
            
            /*
            *增加自己的代码，如果使用RTOS可以通过信号量通知相应的任务指执行
            */
                
            break;
        }
    }
    else
    {
        eStatus = MB_ENOREG;
    }
    
    return eStatus;
}

eMBErrorCode eMBRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils,eMBRegisterMode eMode )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    int             iNCoils = ( int )usNCoils;
    unsigned short  usBitOffset;

    /* Check if we have registers mapped at this block. */
    if( ( usAddress >= REG_COILS_START ) && ( usAddress + usNCoils <= REG_COILS_START + REG_COILS_NREGS*8) )
    {
        usBitOffset = ( unsigned short )( usAddress - REG_COILS_START );
        
        switch ( eMode )
        {
                /* Read current values and pass to protocol stack. */
            case MB_REG_READ:
                while( iNCoils > 0 )
                {
                    *pucRegBuffer++ = xMBUtilGetBits(ucRegCoilsBuf,usBitOffset,(unsigned char)( iNCoils >8 ? 8 :iNCoils ));
                    iNCoils -= 8;
                    usBitOffset += 8;
                }
                
                break;

                /* Update current register values. */
            case MB_REG_WRITE:
                while( iNCoils > 0 )
                {
                    xMBUtilSetBits(ucRegCoilsBuf, usBitOffset, (unsigned char)(iNCoils > 8 ? 8:iNCoils),*pucRegBuffer++ );
                    iNCoils -= 8;
                    usBitOffset += 8;
                }
               
               /*
                *增加自己的代码，如果使用RTOS可以通过信号量通知相应的任务指执行
                */
                
                break;
        }
    }
    else
    {
        eStatus = MB_ENOREG;
    }
    
    return eStatus;
}

eMBErrorCode eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete )
{

    eMBErrorCode    eStatus = MB_ENOERR;
    int iNDiscrete = (int)usNDiscrete;
    unsigned short  usBitOffset;

    /* Check if we have registers mapped at this block. */
    if((usAddress >= REG_DISCRETE_START) && (usAddress + iNDiscrete <= REG_DISCRETE_START + REG_DISCRETE_NREGS*8))
    {
        usBitOffset = ( unsigned short )( usAddress - REG_DISCRETE_START);
        
        while( iNDiscrete > 0 )
        {
            *pucRegBuffer++ = xMBUtilGetBits( ucRegDiscreteBuf, usBitOffset,(unsigned char)( iNDiscrete > 8 ? 8 : iNDiscrete ) );
            iNDiscrete -= 8;
            usBitOffset += 8;
        }
        
       /*
        *增加自己的代码，如果使用RTOS可以通过信号量通知相应的任务指执行
        */
    }
    else
    {
        eStatus = MB_ENOREG;
    }
    
    return eStatus;
}
