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
 
#ifndef _DEMO_H
#define _DEMO_H

#ifdef __cplusplus
PR_BEGIN_EXTERN_C
#endif

/* ----------------------- Defines ------------------------------------------*/
#define REG_INPUT_START    (USHORT)0x0001  //��ʼ�Ĵ���
#define REG_INPUT_NREGS    (USHORT)10

#define REG_HOLDING_START  (USHORT)0x0001  //���ּĴ���
#define REG_HOLDING_NREGS  (USHORT)10      

#define REG_COILS_START    (USHORT)0x0001  //��Ȧ�Ĵ���
#define REG_COILS_NREGS    (USHORT)10      //���ּĴ�������

#define REG_DISCRETE_START (USHORT)0x0001  //��ɢ�Ĵ���
#define REG_DISCRETE_NREGS (USHORT)10     

extern  USHORT   usRegInputStart;
extern  USHORT   usRegInputBuf[REG_INPUT_NREGS];

eMBErrorCode eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs );
eMBErrorCode eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs,
                             eMBRegisterMode eMode );
eMBErrorCode eMBRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils,
                            eMBRegisterMode eMode );
eMBErrorCode eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete );

#ifdef __cplusplus
PR_END_EXTERN_C
#endif
#endif

