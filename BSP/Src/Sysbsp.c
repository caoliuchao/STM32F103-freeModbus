/********************************************************************************
 * @�汾  ��V1.0
 * @�ļ�  : sysbsp.c
 * @����  : caochao
 * @����  : 9-19-2018
 * @�汾  : version1.0
 * @��λ  : �������ӿƼ���ѧ
 * @ժҪ  : ���C�ļ�ʱӲ���ĳ�ʼ��
 *
 * �޸��� :
 *  ����  :
 *
 * �޸��� :
 *  ����  :
 
 ********************************************************************************/
#include "stm32f10x.h"
#include "includes.h"
#include "sysbsp.h"

/********************************************************************************
 *  ������:  BSP_Init
 *  ��  ��:  ϵͳӲ���ĳ�ʼ��
 *  ��  ��:   
 *           ��
 *  ��  ��:   
 *           true����ʼ���ɹ�
 *          false����ʼ��ʧ��
 ********************************************************************************/
bool SYSBSP_Init(void)
{ 
    eMBErrorCode    eStatus;
    
    /*��ʼ��led*/
    LED1_Init();
    LED2_Init();

    /*��ʼ������*/
    USART1_Configuration();
    
    //��ʼ��freemodbus ����RTUģʽ��ID��
    eStatus = eMBInit(MB_RTU, 0x0A,2, 38400, MB_PAR_NONE);
	if (MB_ENOERR != eStatus)
    {
        printf("freemodbus ����RTU��ģʽʧ��\r\n");
        return false;
    }
    
    /* ����Э��ջ�Լ����ںͶ�ʱ�����ѡ�*/
    eStatus = eMBEnable();
    if (MB_ENOERR != eStatus)
    {
        printf("ʹ��ModbusProtocolStackʧ��\r\n");
        return false;
    }
    
    return true;
}
/**************************************�ļ�����*************************************/

