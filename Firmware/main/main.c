#include "common.h"
#include "bsp.h"
#include "bsp_os.h"
#include "led_drv.h"
#include "tc_common.h"
#include "uart_drv.h"
#include "can_drv.h"
#include "gpio_drv.h"
#include "protocol.h"

OS_MUTEX	TX_MUTEX;		//uart tx mutex
OS_MUTEX	RX_MUTEX;		//uart rx mutex

OS_MUTEX	FIFO_MUTEX;

//static FIFO_T stFiFo;

FIFO_T canFiFo;
extern u32 enter_count;
extern u8 packet[12];
//u8 uart_test[20]={0xaa,0xaa,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x55,0x55};
//u8 can_test[8] = {0,1,2,3,4,5,6,7};
extern u8 mode_changed[4];
extern u8 changed_enable[4];

extern control_flag_t control[4];
extern u8 set_power[4];
/*----------------------------------------------------------------------------*/
//macro and variables
#define  APP_CFG_TASK_START_STK_SIZE                    256u
#define  APP_CFG_TASK_START_PRIO                        2u

#define  APP_COMM_TASK_STK_SIZE                    256u
#define  APP_COMM_TASK_PRIO                        3u

#define  APP_UART_TASK_STK_SIZE                    256u
#define  APP_UART_TASK_PRIO                        4u

static  OS_TCB   app_task_start_tcb;
static  CPU_STK  app_task_start_stk[APP_CFG_TASK_START_STK_SIZE];

static  OS_TCB   app_comm_task_tcb;
static  CPU_STK  app_comm_task_stk[APP_COMM_TASK_STK_SIZE];

static  OS_TCB   app_uart_task_tcb;
static  CPU_STK  app_uart_task_stk[APP_UART_TASK_STK_SIZE];

/*----------------------------------------------------------------------------*/
//local function
STATIC void app_comm_task(void *p_arg)
{
	OS_ERR      err;
	(void)p_arg;
//	u8 len=0,i;
	//u8 pack[16];
	//u8 data;
	packet_t frame;
	//MSG("Creating Application Tasks: %d\r\n",__FPU_USED);
	
	
	while (DEF_TRUE) 
    {   
		if(data_received()) //有正确的数据帧
		{
			data_unpack(&frame);
			do_actions(frame);
		}
		OSTimeDlyHMSM(0, 0, 0, 10, OS_OPT_TIME_HMSM_STRICT, &err);
    }
}

STATIC void app_uart_task(void *p_arg)
{
	OS_ERR      err;
	u8 i;
	
	(void)p_arg;
	
	//MSG("Creating Application Tasks: %d\r\n",__FPU_USED);
	
	while (DEF_TRUE) 
    {   
		
		for(i=0;i<4;i++)
 		{
			//if((mode_changed[i] == 1 )&& (changed_enable[i] == 0))
			if(control[i].mode_change == 1 && control[i].changed_enable == 0)
			{
				MSG("change mode start--%d\n",i);
				change_mode(i);
				//changed_enable[i]=1;
				control[i].changed_enable = 1;
				MSG("change mode complete--%d\n",i);
			}
			if(control[i].crash_stop == 1)
			{
				MSG("crash stop start -- %d\n",i);
				do_crash_stop(i);
				control[i].crash_stop = 0;
				MSG("crash stop stop -- %d\n",i);
			}
			if(control[i].return_enable == 1)
			{
				MSG("return home start-- %d\n",i);
				do_return_home(i);
				control[i].return_enable =0;
				MSG("return home stop-- %d\n",i);
			}
			if(set_power[i] == 1)
			{
				MSG("pwr key%d pressed start\n",i);
				set_power[i] = 0;
				gpio_value_set(i);
				//set_power[index] = 1;
				BSP_OS_TimeDly(1000);
				set_controller(i);
				MSG("pwr key%d pressed complete\n",i);		
			}else if(set_power[i] == 2){
				set_power[i] = 0;
				MSG("pwr key%d released start\n",i);
				set_controller(i);
				BSP_OS_TimeDly(1000);
				gpio_value_reset(i);			
				MSG("pwr key%d released complete\n",i);
				
			}
		}
		OSTimeDlyHMSM(0, 0, 0, 50, OS_OPT_TIME_HMSM_STRICT, &err);
    }
}


STATIC void app_task_create(void)
{

	OS_ERR      err;
#if 1	
    OSTaskCreate((OS_TCB       *)&app_comm_task_tcb,              
                 (CPU_CHAR     *)"App communication Task",
                 (OS_TASK_PTR   )app_comm_task, 
                 (void         *)0,
                 (OS_PRIO       )APP_COMM_TASK_PRIO,
                 (CPU_STK      *)&app_comm_task_stk[0],
                 (CPU_STK_SIZE  )app_comm_task_stk[APP_COMM_TASK_STK_SIZE / 10],
                 (CPU_STK_SIZE  )APP_COMM_TASK_STK_SIZE,
                 (OS_MSG_QTY    )0,
                 (OS_TICK       )0,
                 (void         *)0,
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR       *)&err);
#endif

#if 1	
    OSTaskCreate((OS_TCB       *)&app_uart_task_tcb,              
                 (CPU_CHAR     *)"App uart Task",
                 (OS_TASK_PTR   )app_uart_task, 
                 (void         *)0,
                 (OS_PRIO       )APP_UART_TASK_PRIO,
                 (CPU_STK      *)&app_uart_task_stk[0],
                 (CPU_STK_SIZE  )app_uart_task_stk[APP_UART_TASK_STK_SIZE / 10],
                 (CPU_STK_SIZE  )APP_UART_TASK_STK_SIZE,
                 (OS_MSG_QTY    )0,
                 (OS_TICK       )0,
                 (void         *)0,
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR       *)&err);
#endif


}

STATIC void app_task_start(void *p_arg)
{
    OS_ERR      err;
	//u32 i =0;
	



   (void)p_arg;

    BSP_Init();                                                 /* Initialize BSP functions                             */
    CPU_Init();                                                 /* Initialize the uC/CPU services                       */
    
    BSP_Tick_Init();                                            /* Start Tick Initialization                            */
    
    Mem_Init();                                                 /* Initialize Memory Management Module                  */
    Math_Init();                                                /* Initialize Mathematical Module                       */

    BSP_Peripheral_Init();

#if OS_CFG_STAT_TASK_EN > 0u
    OSStatTaskCPUUsageInit(&err);                               /* Compute CPU capacity with no task running            */
#endif

#ifdef CPU_CFG_INT_DIS_MEAS_EN
    CPU_IntDisMeasMaxCurReset();
#endif
    MSG("Creating Application Tasks: %d\r\n",__FPU_USED);
	OSMutexCreate((OS_MUTEX*	)&FIFO_MUTEX,
				  (CPU_CHAR*	)"UART FIFO_MUTEX",
				  (OS_ERR*		)&err);
	OSMutexCreate((OS_MUTEX*	)&TX_MUTEX,
				  (CPU_CHAR*	)"UART TX_MUTEX",
				  (OS_ERR*		)&err);
	OSMutexCreate((OS_MUTEX*	)&RX_MUTEX,
				  (CPU_CHAR*	)"UART RX_MUTEX",
				  (OS_ERR*		)&err);

    app_task_create();    
	
	//	gpio_value_set(GPIO_SRC_PWRCTL1);
	//	gpio_value_set(GPIO_SRC_PWRCTL2);
	//	gpio_value_set(GPIO_SRC_PWRCTL3);
	//	gpio_value_set(GPIO_SRC_PWRCTL4);

    while (DEF_TRUE) 
    {   
        //tc_run_all();
 //       MSG("------------loop-------------\r\n");

		LED1_ON;
		OSTimeDlyHMSM(0, 0, 0, 500, OS_OPT_TIME_HMSM_STRICT, &err);
		LED1_OFF;
		OSTimeDlyHMSM(0, 0, 0, 500, OS_OPT_TIME_HMSM_STRICT, &err);
    }
}


/*----------------------------------------------------------------------------*/
int main(void)
{
    OS_ERR err;

    //BSP_IntDisAll();
    OSInit(&err);

    OSTaskCreate((OS_TCB       *)&app_task_start_tcb,              
                 (CPU_CHAR     *)"App Task Start",
                 (OS_TASK_PTR   )app_task_start, 
                 (void         *)0,
                 (OS_PRIO       )APP_CFG_TASK_START_PRIO,
                 (CPU_STK      *)&app_task_start_stk[0],
                 (CPU_STK_SIZE  )app_task_start_stk[APP_CFG_TASK_START_STK_SIZE / 10],
                 (CPU_STK_SIZE  )APP_CFG_TASK_START_STK_SIZE,
                 (OS_MSG_QTY    )0,
                 (OS_TICK       )0,
                 (void         *)0,
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR       *)&err);

    OSStart(&err);                                              
    
    (void)&err;
    
    return 0;
}

