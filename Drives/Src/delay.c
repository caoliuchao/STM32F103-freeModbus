/********************************************************************************
 * @�汾  ��V1.0
 * @�ļ�  : delay.c
 * @����  : caochao
 * @����  : 9-19-2018
 * @��λ  : �������ӿƼ���ѧ
 * @ժҪ  : ���C�ļ������˸�����ͬ��ʱ������ʵ��
 *
 * �޸��� :
 *  ����  :
 *
 * �޸��� :
 *  ����  :
 *******************************************************************************/
#include "stm32f10x.h"
#include "delay.h"
#include "includes.h"

extern uint8_t  fac_us;  //us��ʱ������
extern uint16_t fac_ms;  //ms��ʱ������

#if SYSTEM_SUPPORT_OS	//SYSTEM_SUPPORT_OS��Ϊ�� ʹ����ucos

/************************************************************************************
*   ��������Delay1us(uint32_t nus)
*   ��  �ܣ���ʱ
*   ��  �룺
*           nus ΪҪ��ʱ��us��. 
*           ע�⣺ nus >= 0
*   ��  ����
*           ��
************************************************************************************/  								   
void Delay1us(uint32_t nus)
{		
	uint32_t ticks;
	uint32_t told;
    uint32_t tnow;
    uint32_t tcnt;
	uint32_t reload;	
	
    reload = SysTick->LOAD;     //LOAD��ֵ	
	ticks = nus  * fac_us;   //��Ҫ�Ľ����� 		 
	tcnt = 0;
	told = SysTick->VAL;        	//�ս���ʱ�ļ�����ֵ
	while(1)
	{
		tnow = SysTick->VAL;	
		if (tnow != told)
		{	    
			if (tnow < told)
            {
                tcnt += told-tnow;//����ע��һ��SYSTICK��һ���ݼ��ļ������Ϳ�����.
            }
			else 
            {
                tcnt += reload-tnow+told;	 
            }                
			told = tnow;
			if (tcnt >= ticks)
            {
                break;//ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�.
            }
		}  
	} 									    
}

/***********************************************************************************
*   ��������Delay1ms(uint16_t nms)
*   ��  �ܣ���ʱ
*   ��  �룺
*           nms ΪҪ��ʱ��ms��.
*           ע�⣺nms >= 0
*   ��  ����
*           ��
**********************************************************************************/
void Delay1ms(uint16_t nms)
{	
    OS_ERR   ERR;
	if(OSRunning == true)   //���os�Ѿ�������	    
	{		  
		if(nms >= fac_ms)    //��ʱ��ʱ�����ucos������ʱ������ 
		{
   			OSTimeDly(nms/fac_ms,OS_OPT_TIME_DLY,&ERR);  //ucos��ʱ
		}
        
		nms = nms % fac_ms;				//ucos�Ѿ��޷��ṩ��ôС����ʱ��,������ͨ��ʽ��ʱ    
	}
    
	Delay1us(nms * 1000);	//��ͨ��ʽ��ʱ,��ʱucos�޷���������.
}

#else       //����ucosʱ

/**********************************************************************************
*   ��������Delay1us(uint32_t nus)
*   ��  �ܣ���ʱ
*   ��  �룺
*           nus ΪҪ��ʱ��us��.	
*   ��  ����
*           ��
**********************************************************************************/
void Delay1us(uint32_t nus)
{			    	 
	SysTick->LOAD = nus * fac_us; //ʱ�����	  		 
	SysTick->VAL = 0x00;        //��ռ�����
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;   //��ʼ���� 
	do
    {
        ;
    }while(!( (SysTick ->CTRL) & (SysTick_CTRL_COUNTFLAG_Msk) ));  //�ȴ�ʱ�䵽��  
    
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       //�رռ�����
	SysTick->VAL =0X00;       //��ռ�����	 
}

/**********************************************************************************
*   ��������Delay1ms(uint16_t nms)
*   ��  �ܣ���ʱ
*   ��  �룺
*           nms ΪҪ��ʱ��ms��. 
*           ע�� ����72M������,nms <= 1864 	
*   ��  ����
*           ��
**********************************************************************************/
void Delay1ms(uint16_t nms)
{	 		  	  	   
	SysTick->LOAD = (uint32_t)nms * fac_ms;    //ʱ�����(SysTick->LOADΪ24bit)
	SysTick->VAL =0x00;               //��ռ�����
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;       //��ʼ����  
    
	do
    {
        ;
    }while(!( (SysTick ->CTRL) & (SysTick_CTRL_COUNTFLAG_Msk) ));  //�ȴ�ʱ�䵽�� 
    
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       //�رռ�����
	SysTick->VAL =0X00;       //��ռ�����	  	    
} 

#endif

/************************************ �ļ�����***************************************/

