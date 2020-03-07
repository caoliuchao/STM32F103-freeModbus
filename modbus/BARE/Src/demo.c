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

//��������Է�Ϊλ�������ֲ������ࡣλ��������С��λΪBIT���ֲ�������С��λΪ�����ֽڡ�
//[λ����ָ��] ����Ȧ״̬01H����(��ɢ)����״̬02H��д������Ȧ06H��д�����Ȧ0FH��
//[�ֲ���ָ��] �����ּĴ���03H��д�����Ĵ���06H��д������ּĴ���10H��
///Modbus�У����ݿ��Է�Ϊ�����࣬�ֱ�ΪCoil��Register��ÿһ�����ݣ�
//���ݶ�д��ʽ�Ĳ�ͬ���ֿ�ϸ��Ϊ���֣�ֻ������д����Modbus�����������ͣ�
//Discretes Input     λ����     `   ֻ��
//Coils               λ����         ��д
//Input Registers     16-bit����     ֻ��
//Holding Registers   16-bit����     ��д
 
/*��Ȧ    ����˿ڡ����趨�˿ڵ����״̬��Ҳ���Զ�ȡ��λ�����״̬�� DO           ��ŷ������MOSFET�����LED��ʾ�ȡ�
 ״̬    �ɷ�Ϊ���ֲ�ͬ��ִ��״̬�����籣���ͻ���ش����͡�        ��������� 
       
 ��ɢ     ����˿ڡ�ͨ���ⲿ�趨�ı�����״̬���ɶ�������д     DI                ���뿪�أ��ӽ����صȡ�                                               
 ����״̬                                                  ����������

 ����    ��������򱣳ֲ���������������ʱ���趨��ĳЩ������     AO               ģ��������趨ֵ��PID���в����������������С��
 �Ĵ���  �ɶ���д                                           ģ�������        ������������������

 ����    �������������������ʱ���ⲿ�豸��õĲ���            AI
 �Ĵ���  �ɶ�������д��                                      ģ��������        ģ��������*/

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
         *�����Լ��Ĵ��룬���ʹ��RTOS����ͨ���ź���֪ͨ��Ӧ������ִָ��
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
            *�����Լ��Ĵ��룬���ʹ��RTOS����ͨ���ź���֪ͨ��Ӧ������ִָ��
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
                *�����Լ��Ĵ��룬���ʹ��RTOS����ͨ���ź���֪ͨ��Ӧ������ִָ��
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
        *�����Լ��Ĵ��룬���ʹ��RTOS����ͨ���ź���֪ͨ��Ӧ������ִָ��
        */
    }
    else
    {
        eStatus = MB_ENOREG;
    }
    
    return eStatus;
}
