/********************************************************************************
 * @�汾  ��V1.0
 * @�ļ�  : task.c
 * @����  : caochao
 * @����  : 9-19-2018
 * @��λ  : �������ӿƼ���ѧ
 * @ժҪ  : ���C�ļ�ʱʵ����ucosII������
 *
 * �޸��� :
 *  ����  :
 *
 * �޸��� :
 *  ����  :
 *******************************************************************************/
#include "stm32f10x.h"
#include "includes.h"
#include "task.h"
#include  <cpu.h>

/*-----------------------------��ʼ�����ջ-------------------------------------------*/
OS_TCB     TaskStartTCB;
__align(8) CPU_STK START_TASK_STK[START_STK_SIZE];   //���������ջ�ռ�	

/*----------------------------���ڴ�ӡ��ջ--------------------------------------------*/
OS_TCB     TaskUsart1TCB;
__align(8) CPU_STK USART1_TASK_STK[USART1_STK_SIZE]; //���������ջ�ռ�	

/*-----------------------------LED1�����ջ------------------------------------------*/
OS_TCB     TaskLed1TCB;
__align(8) CPU_STK LED1_TASK_STK[LED1_STK_SIZE];     //���������ջ�ռ�	

/*-----------------------------LED2�����ջ------------------------------------------*/
OS_TCB     TaskLed2TCB;
__align(8) CPU_STK LED2_TASK_STK[LED2_STK_SIZE];      //���������ջ�ռ�	

/*------------------------------modbus�����ջ----------------------------------------*/
OS_TCB     TaskModbusTCB;
__align(8) CPU_STK MODBUS_TASK_STK[MODBUS_STK_SIZE];     //���������ջ�ռ�

/*-----------------------------Head�����ջ-----------------------------------------*/
OS_TCB     TaskHeadTCB;
__align(8) CPU_STK HEAD_TASK_STK[HEAD_STK_SIZE];      //���������ջ�ռ�	

/************************************************************************************
 *   ��������start_task(void *pdata)
 *   ��  �ܣ���ʼ���񣬽�����������
 *   ��  �룺
 *          pdata ָ��
 *   ��  ����
 *          ��
 **********************************************************************************/
void start_task(void *pdata)
{
    OS_ERR      err;

    Systick_Init();       /*Systick��ʼ������*/
    SYSBSP_Init();        /*Initialize  */
       
    CPU_Init();           /* Initialize the uC/CPU services*/
      
    Mem_Init();           /* Initialize memory managment module                   */
    Math_Init();          /* Initialize mathematical module                       */
 
#if OS_CFG_STAT_TASK_EN > 0u
    OSStatTaskCPUUsageInit(&err);     /* Compute CPU capacity with no task running            */
#endif

#ifdef CPU_CFG_INT_DIS_MEAS_EN
    CPU_IntDisMeasMaxCurReset();
#endif

#if OS_CFG_SCHED_ROUND_ROBIN_EN
    OSSchedRoundRobinCfg(DEF_ENABLED,10,&err);
#endif

    OSTaskCreate((OS_TCB       *)&TaskUsart1TCB,       /* Create the start task                                */
                 (CPU_CHAR     *)"Usart1  Task",
                 (OS_TASK_PTR   )usart1_task,
                 (void         *)0u,
                 (OS_PRIO       )USART1_TASK_PRIO,
                 (CPU_STK      *)&USART1_TASK_STK[0u],
                 (CPU_STK_SIZE  )USART1_STK_SIZE/10u,
                 (CPU_STK_SIZE  )USART1_STK_SIZE,
                 (OS_MSG_QTY    )0u,
                 (OS_TICK       )0u,
                 (void         *)0u,
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR       *)&err); 
                 
    OSTaskCreate((OS_TCB       *)&TaskLed1TCB,       /* Create the start task                                */
                 (CPU_CHAR     *)"led1  Task",
                 (OS_TASK_PTR   )led1_task,
                 (void         *)0u,
                 (OS_PRIO       )LED1_TASK_PRIO,
                 (CPU_STK      *)&LED1_TASK_STK[0u],
                 (CPU_STK_SIZE  )LED1_STK_SIZE/10u,
                 (CPU_STK_SIZE  )LED1_STK_SIZE,
                 (OS_MSG_QTY    )0u,
                 (OS_TICK       )0u,
                 (void         *)0u,
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR       *)&err);     

    OSTaskCreate((OS_TCB       *)&TaskLed2TCB,       /* Create the start task                                */
                 (CPU_CHAR     *)"led2  Task",
                 (OS_TASK_PTR   )led2_task,
                 (void         *)0u,
                 (OS_PRIO       )LED2_TASK_PRIO,
                 (CPU_STK      *)&LED2_TASK_STK[0u],
                 (CPU_STK_SIZE  )LED2_STK_SIZE/10u,
                 (CPU_STK_SIZE  )LED2_STK_SIZE,
                 (OS_MSG_QTY    )0u,
                 (OS_TICK       )0u,
                 (void         *)0u,
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR       *)&err); 
                 
//    OSTaskCreate((OS_TCB       *)&TaskHeadTCB,       /* Create the start task                                */
//                 (CPU_CHAR     *)"Head  Task",
//                 (OS_TASK_PTR   )head_task,
//                 (void         *)0u,
//                 (OS_PRIO       )HEAD_TASK_PRIO,
//                 (CPU_STK      *)&HEAD_TASK_STK[0u],
//                 (CPU_STK_SIZE  )HEAD_STK_SIZE/10u,
//                 (CPU_STK_SIZE  )HEAD_STK_SIZE,
//                 (OS_MSG_QTY    )0u,
//                 (OS_TICK       )0u,
//                 (void         *)0u,
//                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
//                 (OS_ERR       *)&err); 

    OSTaskCreate((OS_TCB       *)&TaskModbusTCB,       /* Create the modbus task    */
                 (CPU_CHAR     *)"modbus  Task",
                 (OS_TASK_PTR   )modbus_task,
                 (void         *)0u,
                 (OS_PRIO       )MODBUS_TASK_PRIO,
                 (CPU_STK      *)&MODBUS_TASK_STK[0u],
                 (CPU_STK_SIZE  )MODBUS_STK_SIZE/10u,
                 (CPU_STK_SIZE  )MODBUS_STK_SIZE,
                 (OS_MSG_QTY    )0u,
                 (OS_TICK       )0u,
                 (void         *)0u,
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR       *)&err);
                 
    OSTaskDel(&TaskStartTCB,&err);  //ɾ����ʼ����         
    if (err == OS_ERR_NONE)
    {
        printf("��ʼ����ɾ���ɹ���\r\n");
    }
    else
    {
        printf("��ʼ����ɾ���ɹ�! \r\n");
    }           
}

/**********************************************************************************
 *   ��������led1_task(void *pdata)
 *   ��  �ܣ�LED1����
 *   ��  �룺
 *          pdata ָ��
 *   ��  ����
 *          ��
 *********************************************************************************/
void led1_task(void *pdata)
{
	while (1)
	{	
		LED1_ON;         //LED1����
		Delay1ms(500);
		LED1_OFF;         //LED1����
		Delay1ms(1000);
	}
}

/**********************************************************************************
 *   ��������led2_task(void *pdata)
 *   ��  �ܣ�LED2����
 *   ��  �룺
 *          pdata ָ��
 *   ��  ����
 *          ��
 *********************************************************************************/
void led2_task(void *pdata)
{ 
	while (1)
	{
        LED2_ON;            //LED2����
		Delay1ms(1000);
		LED2_OFF;           //LED2����
		Delay1ms(500);
	}
}

/*********************************************************************
 *   ��������modbus_task(void *pdata)
 *   ��  �ܣ�modbus������
 *   ��  �룺
 *          pdata ָ��
 *   ��  ����
 *          ��
 *********************************************************************/
void modbus_task(void *pdata)
{
    while (1)
    {
        eMBPoll();
        Delay1ms(5);
    }   
}

/**********************************************************************************
 *   ��������usart1_task(void *pdata)
 *   ��  �ܣ�usart1����
 *   ��  �룺
 *          pdata ָ��
 *   ��  ����
 *          ��
 *********************************************************************************/
void usart1_task(void *pdata)
{ 
    while (1)
    {  	
        printf("CPU��ʹ����Ϊ��%d%%  \r\n",OSStatTaskCPUUsage/100);
      
		Delay1ms(500);	
    }
}

/**********************************************************************************
 *   ��������head_task(void *pdata)
 *   ��  �ܣ���������Ķ�ջͳ������
 *   ��  �룺
 *          pdata ָ��
 *   ��  ����
 *          ��
 *********************************************************************************/
void head_task(void *pdata)
{
    OS_ERR err;  
    CPU_STK_SIZE free,used; 

    while(DEF_TRUE)  
    {      
        OSTaskStkChk (&TaskUsart1TCB,&free,&used,&err);//  ��ͳ��������Ķ�ջʹ����Ҳ��ӡ����  
        printf("TaskUsart1TCB           used/free:%d/%d  usage:%%%d\r\n",used,free,(used*100)/(used+free));  
    
        OSTaskStkChk (&TaskLed1TCB,&free,&used,&err);  
        printf("TaskLed1TCB             used/free:%d/%d  usage:%%%d\r\n",used,free,(used*100)/(used+free));  
    
        OSTaskStkChk (&TaskLed2TCB,&free,&used,&err);  
        printf("TaskLed2TCB             used/free:%d/%d  usage:%%%d\r\n",used,free,(used*100)/(used+free));  

        OSTaskStkChk (&TaskHeadTCB,&free,&used,&err);//  ��ͳ��������Ķ�ջʹ����Ҳ��ӡ����  
        printf("TaskHeadTCB             used/free:%d/%d  usage:%%%d\r\n",used,free,(used*100)/(used+free));  
    
        OSTaskStkChk (&TaskModbusTCB,&free,&used,&err);//  ��ͳ��������Ķ�ջʹ����Ҳ��ӡ����  
        printf("TaskModbusTCB           used/free:%d/%d  usage:%%%d\r\n",used,free,(used*100)/(used+free));  
    
        Delay1ms(1000);	
    }  
}
/************************************�ļ�����****************************************/
                


