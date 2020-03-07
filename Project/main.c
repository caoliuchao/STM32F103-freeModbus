/********************************************************************************
 * @�汾  ��V1.0
 * @�ļ�  : main.c
 * @����  : caochao
 * @����  : 9-19-2019
 * @��λ  : �������ӿƼ���ѧ
 * @ժҪ  : ���C�ļ�ʱʵ����ucosIII������
 *
 * �޸��� :
 *  ����  :
 *
 * �޸��� :
 *  ����  :
 *******************************************************************************/
#include "stm32f10x.h"
#include "includes.h"
#include <stdio.h>
#include "task.h"
 
/*******************************************************************************
* ������:  main
* ��  ��:  �����������
* ��  ��:   
*         �� 
* ��  ��:   
*         �У���Ϊ�в���ϵͳ��Ե�ʣ�ϵͳ��Զ���᷵�أ�
*******************************************************************************/
int main(void)
{
    OS_ERR  err;

    CPU_IntDis();           /* Disable all interrupts. */     
    
    OSInit(&err);           /* Init uC/OS-III. */
    
    OSTaskCreate((OS_TCB       *)&TaskStartTCB,  /* Create the start task                                */
                 (CPU_CHAR     *)"Start  Task",
                 (OS_TASK_PTR   )start_task,
                 (void         *)0u,
                 (OS_PRIO       )START_TASK_PRIO,
                 (CPU_STK      *)&START_TASK_STK[0u],
                 (CPU_STK_SIZE  )START_TASK_STK[START_STK_SIZE / 10u],
                 (CPU_STK_SIZE  )START_STK_SIZE,
                 (OS_MSG_QTY    )0u,
                 (OS_TICK       )0u,
                 (void         *)0u,
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR       *)&err);


    OSStart(&err);            /* Start multitasking (i.e. give control to uC/OS-III). */    
                 
    return (0u);
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
      printf("Wrong parameters value: file %s on line %d\r\n", file, line);
  }
}

#endif
/*******************************�ļ�����****************************************/
