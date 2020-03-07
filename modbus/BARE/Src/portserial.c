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

#include "port.h"
#include "stm32f10x.h"
#include "includes.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

/* ----------------------- static functions ---------------------------------*/
static void prvvUARTTxReadyISR( void );
static void prvvUARTRxISR( void );

/* ----------------------- Start implementation -----------------------------*/
void vMBPortSerialEnable( BOOL xRxEnable, BOOL xTxEnable )
{
    /* If xRXEnable enable serial receive interrupts. If xTxENable enable
     * transmitter empty interrupts.
     */
    /* If xRXEnable enable serial receive interrupts. If xTxENable enable
     * transmitter empty interrupts.
     */
  
    if (xRxEnable)  
    {
        USART_ITConfig(MODBUS_USART_PORT, USART_IT_RXNE, ENABLE);  //ʹ�ܽ����ж�
    }
    else  
    {
        USART_ITConfig(MODBUS_USART_PORT, USART_IT_RXNE, DISABLE);  //ʧ�ܽ����ж�
    }
    
    if (xTxEnable)  
    {
        USART_ITConfig(MODBUS_USART_PORT, USART_IT_TC, ENABLE);  //ʹ�ܷ����ж�
    }
    else  
    {
        USART_ITConfig(MODBUS_USART_PORT, USART_IT_TC, DISABLE);  //ʧ�ܷ����ж�
    }
}

void vMBPortClose( void )
{
    USART_Cmd(MODBUS_USART_PORT,DISABLE);       //�ر�usart    
}

BOOL xMBPortSerialInit( UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits, eMBParity eParity )
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef  NVIC_InitStructure;
    
    RCC_APB2PeriphClockCmd(MODBUS_USART_TX_PERIPH,ENABLE);   //��GPIO��ʱ��
    RCC_APB2PeriphClockCmd(MODBUS_USART_RX_PERIPH,ENABLE);   //��GPIO��ʱ��
    MODBUS_USART_CLOCK_API(MODBUS_USART_PERIPH,ENABLE);      //��usartʱ��  
    
    GPIO_InitStructure.GPIO_Pin	= MODBUS_USART_TX_PIN;         //USART��TXΪ�����������ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	
	GPIO_Init(MODBUS_USART_TX_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin	= MODBUS_USART_RX_PIN ;         //USART��RXΪ�����������ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	
	GPIO_Init(MODBUS_USART_RX_PORT,&GPIO_InitStructure);

    if (eParity == MB_PAR_NONE)
    {
        USART_InitStructure.USART_Parity = USART_Parity_No;             //��������żλ���
    }
    else if (eParity == MB_PAR_EVEN)
    {
        USART_InitStructure.USART_Parity = USART_Parity_Even;           //����żλ���
    }
    else 
    {
        USART_InitStructure.USART_Parity = USART_Parity_Odd;           //������λ���
    }
    
    if (ucDataBits == 9)
    {
        USART_InitStructure.USART_WordLength  = USART_WordLength_9b;        //��������Ϊ9λ
    }
    else
    {
        USART_InitStructure.USART_WordLength  = USART_WordLength_8b;        //��������Ϊ8λ
    }
    
	USART_InitStructure.USART_BaudRate = ulBaudRate ;          //����������
	USART_InitStructure.USART_StopBits = USART_StopBits_1;           //ֹͣλΪ1
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   //��Ӳ����
    USART_InitStructure.USART_Mode =  USART_Mode_Rx | USART_Mode_Tx;  //����usart���ܺͷ�������
    
    USART_Init(MODBUS_USART_PORT,&USART_InitStructure); //��ʼ��usart 
	
    NVIC_InitStructure.NVIC_IRQChannel                   = MODBUS_USART_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    
    NVIC_Init(&NVIC_InitStructure);
    
    USART_Cmd(MODBUS_USART_PORT,ENABLE);                  //����usart    
   
    return TRUE;   
}

BOOL xMBPortSerialPutByte( CHAR ucByte )
{
    /* Put a byte in the UARTs transmit buffer. This function is called
     * by the protocol stack if pxMBFrameCBTransmitterEmpty( ) has been
     * called. */
    
    USART_SendData(MODBUS_USART_PORT, ucByte);  //����һ���ֽ�
    
    return TRUE;
}

BOOL xMBPortSerialGetByte( CHAR * pucByte )
{
    /* Return the byte in the UARTs receive buffer. This function is called
     * by the protocol stack after pxMBFrameCBByteReceived( ) has been called.
     */
    
    *pucByte = USART_ReceiveData(MODBUS_USART_PORT);  //����һ���ֽ�
    
    return TRUE;
}

/* Create an interrupt handler for the transmit buffer empty interrupt
 * (or an equivalent) for your target processor. This function should then
 * call pxMBFrameCBTransmitterEmpty( ) which tells the protocol stack that
 * a new character can be sent. The protocol stack will then call 
 * xMBPortSerialPutByte( ) to send the character.
 */
static void prvvUARTTxReadyISR( void )
{
    pxMBFrameCBTransmitterEmpty(  );
}

/* Create an interrupt handler for the receive interrupt for your target
 * processor. This function should then call pxMBFrameCBByteReceived( ). The
 * protocol stack will then call xMBPortSerialGetByte( ) to retrieve the
 * character.
 */
static void prvvUARTRxISR( void )
{
    pxMBFrameCBByteReceived(  );
}

/*****************************************************************************
 *   ��������MODBUS_USART_PORT_IRQHandler
 *   ��  �ܣ������жϷ�����
 *   ��  �룺
 *          ��
 *   ��  ����
 *          ��
 ****************************************************************************/
void MODBUS_USART_IRQHandler(void)
{
#if SYSTEM_SUPPORT_OS	 	
	OSIntEnter();    
#endif
    
    if (USART_GetITStatus(MODBUS_USART_PORT, USART_IT_RXNE) == SET)  //�����ж�
    {
        prvvUARTRxISR();
        USART_ClearITPendingBit(MODBUS_USART_PORT, USART_IT_RXNE);
    }
    
    if (USART_GetITStatus(MODBUS_USART_PORT, USART_IT_TC) == SET)  //�����ж�
    {
        prvvUARTTxReadyISR();
        USART_ClearITPendingBit(MODBUS_USART_PORT, USART_IT_TC);
    }

#if SYSTEM_SUPPORT_OS	 
	OSIntExit();  											 
#endif	
}

/***********************************�ļ�����***********************************/




