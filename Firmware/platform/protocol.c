#include "common.h"
#include "stm32f4xx_gpio.h"
#include "protocol.h"
#include "uart_drv.h"
#include "gpio_drv.h"
#include "bsp.h"
#include "bsp_os.h"

extern FIFO_T canFiFo;
u8 packet[12] = {0};
u8 mode_changed[4]={0};
u8 changed_enable[4] ={0};

control_flag_t control[4]={0};
u8 set_power[4]={0};

u8 checksum(u8 *data,u8 size)
{
	u8 ret;
	u16 sum=0;

	while(size--)
	{
		sum += *(data++);
	}

	ret = sum %256;

	return ret;
}

bool data_received(void)
{
	u8 read;
	u8 pos;
	bool ret=FALSE;

	if(Fifo_DataLen(&canFiFo) >=16)
	{
		if(Fifo_Read(&canFiFo,&read) == TRUE && read == HEAD1)
		{
			if(Fifo_Read(&canFiFo,&read) == TRUE && read == HEAD2)
			{
				for(pos=0;pos<12;pos++)
				{
					Fifo_Read(&canFiFo,&read);
					packet[pos] = read;
				}
				if(Fifo_Read(&canFiFo,&read) == TRUE && read == TAIL1)
				{
					if(Fifo_Read(&canFiFo,&read) == TRUE && read == TAIL2)
					{
						if(checksum(packet,11) == packet[11])
						ret=TRUE;
					}
				}
			}
		}
	}
	return ret;
	
}


void data_unpack(packet_t *data)
{
	u8 index;
	data->index_src = packet[INDEX_SRC];
	data->control_type = packet[CONTROL_SRC];
	data->control_event = packet[EVENT_SRC];

	for(index=0;index<8;index++)
		data->adc_value[index] = packet[VALUE_ADC1+index];
}

void pack_channels(u8 index,packet_t pack)
{
	u8 tx_buf[14]={0};
	u8 i;

	
		tx_buf[0] = HEAD1;
		tx_buf[1] = HEAD2;
		if(index == TYPE_DRONE)
		{
			tx_buf[2] = TYPE_DRONE;
			for(i=0;i<8;i++)
			{
				tx_buf[3+i] = pack.adc_value[i];
			}
		}else if(index == TYPE_PTZ){
			tx_buf[2] = TYPE_PTZ;
			for(i=0;i<4;i++)
			{
				tx_buf[7+i] = pack.adc_value[i];
			}
		}else if(index == TYPE_PTZRESET){
			tx_buf[2] =TYPE_PTZRESET;
		}else if(index == TYPE_ZOOMIN){
			tx_buf[2] =TYPE_ZOOMIN;
		}else if(index == TYPE_ZOOMOUT){
			tx_buf[2] =TYPE_ZOOMOUT;
		}else if(index == TYPE_ZOOMRESET){
			tx_buf[2] =TYPE_ZOOMRESET;
		}
		tx_buf[11] = checksum(tx_buf+2,9);
		tx_buf[12] = TAIL1;
		tx_buf[13] = TAIL2;	

		if(pack.index_src == INDEX_SRC_DRONE1){
			uart_drv_gcs1_send(tx_buf,14);
		}else if(pack.index_src == INDEX_SRC_DRONE2){
			uart_drv_gcs2_send(tx_buf,14);
		}else if(pack.index_src == INDEX_SRC_DRONE3){
			uart_drv_gcs3_send(tx_buf,14);
		}else if(pack.index_src == INDEX_SRC_DRONE4){
			uart_drv_gcs4_send(tx_buf,14);
		} 
}

void change_mode(u8 index)
{
	switch(index)
	{
		case 0:
			gpio_value_reset(GPIO_SRC_MODE1);
			BSP_OS_TimeDly(1000);
			gpio_value_set(GPIO_SRC_MODE1);
			break;
		case 1:
			gpio_value_set(GPIO_SRC_MODES_2);
			gpio_value_reset(GPIO_SRC_MODEP_2);
			BSP_OS_TimeDly(1000);
			gpio_value_set(GPIO_SRC_MODEP_2);
			gpio_value_reset(GPIO_SRC_MODES_2);
			break;
		case 2:
			gpio_value_set(GPIO_SRC_MODES_3);
			gpio_value_reset(GPIO_SRC_MODEP_3);
			BSP_OS_TimeDly(1000);
			gpio_value_set(GPIO_SRC_MODEP_3);
			gpio_value_reset(GPIO_SRC_MODES_3);
			break;
		case 3:
			gpio_value_set(GPIO_SRC_MODEA_4);
			gpio_value_reset(GPIO_SRC_MODEP_4);
			BSP_OS_TimeDly(1000);
			gpio_value_set(GPIO_SRC_MODEP_4);
			gpio_value_reset(GPIO_SRC_MODEA_4);
			break;
	}
}

void do_crash_stop(u8 index)
{
	switch(index)
	{
		case 0:
			gpio_value_set(GPIO_SRC_EMG1);
			BSP_OS_TimeDly(1000);
			gpio_value_reset(GPIO_SRC_EMG1);
			break;
		case 1:
			gpio_value_set(GPIO_SRC_EMG2);
			BSP_OS_TimeDly(1000);
			gpio_value_reset(GPIO_SRC_EMG2);
			break;
		case 2:
			gpio_value_set(GPIO_SRC_EMG3);
			BSP_OS_TimeDly(1000);
			gpio_value_reset(GPIO_SRC_EMG3);
			break;
		case 3:
			gpio_value_set(GPIO_SRC_EMG4);
			BSP_OS_TimeDly(1000);
			gpio_value_reset(GPIO_SRC_EMG4);
			break;
	}
}

void do_return_home(u8 index)
{
	switch(index)
	{
		case 0:
			gpio_value_set(GPIO_SRC_RTH1);
			BSP_OS_TimeDly(2500);
			gpio_value_reset(GPIO_SRC_RTH1);
			break;
		case 1:
			gpio_value_set(GPIO_SRC_RTH2);
			BSP_OS_TimeDly(2500);
			gpio_value_reset(GPIO_SRC_RTH2);
			break;
		case 2:
			gpio_value_set(GPIO_SRC_RTH3);
			BSP_OS_TimeDly(2500);
			gpio_value_reset(GPIO_SRC_RTH3);
			break;
		case 3:
			gpio_value_set(GPIO_SRC_RTH4);
			BSP_OS_TimeDly(2500);
			gpio_value_reset(GPIO_SRC_RTH4);
			break;
	}
}

void set_controller(u8 index)
{
		
	switch(index)
	{
		case 0:		//mavic air
			//short press(500ms)+long press(2000ms)
			gpio_value_set(GPIO_SRC_PWRKEY_L1);
			gpio_value_reset(GPIO_SRC_PWRKEY_R1);
			BSP_OS_TimeDly(500);
			gpio_value_reset(GPIO_SRC_PWRKEY_L1);
			gpio_value_set(GPIO_SRC_PWRKEY_R1);
			
			BSP_OS_TimeDly(300);

			gpio_value_set(GPIO_SRC_PWRKEY_L1);
			gpio_value_reset(GPIO_SRC_PWRKEY_R1);
			BSP_OS_TimeDly(2000);
			gpio_value_reset(GPIO_SRC_PWRKEY_L1);
			gpio_value_set(GPIO_SRC_PWRKEY_R1);
			break;
		case 1:		//mavic 2
			//short press(500ms)+long press(2000ms)
			gpio_value_reset(GPIO_SRC_PWRKEY2);
			BSP_OS_TimeDly(500);
			gpio_value_set(GPIO_SRC_PWRKEY2);
			
			BSP_OS_TimeDly(300);

			gpio_value_reset(GPIO_SRC_PWRKEY2);
			BSP_OS_TimeDly(2000);
			gpio_value_set(GPIO_SRC_PWRKEY2);
			break;
		case 2:		//mavic 2
			//short press(500ms)+long press(2000ms)
			gpio_value_reset(GPIO_SRC_PWRKEY3);
			BSP_OS_TimeDly(500);
			gpio_value_set(GPIO_SRC_PWRKEY3);
			
			BSP_OS_TimeDly(300);

			gpio_value_reset(GPIO_SRC_PWRKEY3);
			BSP_OS_TimeDly(2000);
			gpio_value_set(GPIO_SRC_PWRKEY3);
			break;
		case 3:		//m210
			//short press(500ms)+long press(2000ms)
			gpio_value_set(GPIO_SRC_PWRKEY4);
			BSP_OS_TimeDly(500);
			gpio_value_reset(GPIO_SRC_PWRKEY4);
			
			BSP_OS_TimeDly(300);

			gpio_value_set(GPIO_SRC_PWRKEY4);
			BSP_OS_TimeDly(2000);
			gpio_value_reset(GPIO_SRC_PWRKEY4);
			break;
	}
		
}

void do_actions(packet_t pack)
{
	u8 index;
	
	switch(pack.index_src)
	{
		case INDEX_SRC_DRONE1:
		case INDEX_SRC_DRONE2:
		case INDEX_SRC_DRONE3:
		case INDEX_SRC_DRONE4:
			if(pack.control_type == CONTROL_SRC_DRONE)
			{
				index = pack.index_src -1;
				if(pack.control_event == EVENT_SRC_JOYSTICK)
				{
					//frist time need change fly mode				
					if(control[index].mode_change == 0)
						control[index].mode_change =1;
					//send joystick data to gcs1 by uart
					if(control[index].changed_enable == 1)
						pack_channels(TYPE_DRONE,pack);
				}else if(pack.control_event ==EVENT_SRC_CRASHSTOP){
					//drone crash stop
					control[index].crash_stop = 1;
				}else if(pack.control_event ==EVENT_SRC_RTNHOME){
					//go back to home
					control[index].return_enable = 1;
				}	

			}else if(pack.control_type == CONTROL_SRC_PTZ){
				if(pack.control_event == EVENT_SRC_JOYSTICK)
				{
					//send ptz data to gcs by uart
					//MSG("ptz mode--%d\n",pack.index_src);
					pack_channels(TYPE_PTZ,pack);
				}else if(pack.control_event == EVENT_SRC_PTZRESET){
					//ptz reset
					MSG("ptz reset--%d\n",pack.index_src);
					pack_channels(TYPE_PTZRESET,pack);
				}else if(pack.control_event == EVENT_SRC_ZOOMIN){
					//zoom in
					MSG("zoom in--%d\n",pack.index_src);
					pack_channels(TYPE_ZOOMIN,pack);
				}else if(pack.control_event == EVENT_SRC_ZOOMOUT){
					//zoom out
					MSG("zoom out--%d\n",pack.index_src);
					pack_channels(TYPE_ZOOMOUT,pack);
				}else if(pack.control_event == EVENT_SRC_ZOOMRESET){
					//zoom reset
					MSG("zoom reset--%d\n",pack.index_src);
					pack_channels(TYPE_ZOOMRESET,pack);
				}
					
			}
			break;
		case INDEX_SRC_PWRKEY1:
		case INDEX_SRC_PWRKEY2:
		case INDEX_SRC_PWRKEY3:
		case INDEX_SRC_PWRKEY4:
			index = pack.index_src-5;
			if(pack.control_type == CONTROL_SRC_OPEN)
			{		
				set_power[index] = 1;
			}else{
				set_power[index] = 2;
			}
			break;
	}
}

