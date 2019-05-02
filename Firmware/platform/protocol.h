#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_


//protocol for send message to gcs by UART
#define HEAD1		0xAA
#define HEAD2		0xAA
#define TAIL1		0x55
#define TAIL2		0x55

#define ROCKER_MIDDLE	0
#define ROCKER_UP		1
#define ROCKER_DOWN		2

#define	ADC_CHANNEL_JS_THROTTLE	0		
#define ADC_CHANNEL_JS_ROLL		1		
#define	ADC_CHANNEL_JS_PITCH	2		
#define ADC_CHANNEL_JS_YAW		3		

#define ADC_CHANNEL_PTZ_V		0		//云台竖轴
#define ADC_CHANNEL_PTZ_H		1		//云台横轴

#define ON		1
#define OFF 	0
//飞行器编号
typedef enum{
	INDEX_SRC_DRONE1=1,
	INDEX_SRC_DRONE2,
	INDEX_SRC_DRONE3,
	INDEX_SRC_DRONE4,
	INDEX_SRC_PWRKEY1,
	INDEX_SRC_PWRKEY2,
	INDEX_SRC_PWRKEY3,
	INDEX_SRC_PWRKEY4
}index_src_enum;

//控制类型
typedef enum{
	CONTROL_SRC_CLOSE=0,
	CONTROL_SRC_OPEN,
	CONTROL_SRC_DRONE,
	CONTROL_SRC_PTZ,
	CONTROL_SRC_LAMP,
	CONTROL_SRC_WHISTLE1,
	CONTROL_SRC_WHISTLE2,
	CONTROL_SRC_SHOUTING,

	CONTROL_SRC_NUM
}control_src_enum;

//控制事件
typedef enum{
	EVENT_SRC_CLOSE=0,
	EVENT_SRC_OPEN,
	EVENT_SRC_JOYSTICK,
	EVENT_SRC_CRASHSTOP,
	EVENT_SRC_RTNHOME,
	EVENT_SRC_PTZRESET,
	EVENT_SRC_ZOOMIN,
	EVENT_SRC_ZOOMOUT,
	EVENT_SRC_ZOOMRESET,

	EVENT_SRC_NUM
}event_src_enum;

typedef struct{
	u8 packet_enable;	//飞行事件发送使能
	u8 power_enable;	//电源事件发送使能
	u8 power_trig;	
	u8 event_index;
	u8 repeat;
	u8 index_src;		//控制编号
	u8 control_type;	//控制类型
	u8 control_event;	//控制事件
	u8 adc_value[8];	//摇杆数据
}packet_t;

typedef enum{
	PACKET_START=0,
	INDEX_SRC=PACKET_START,
	CONTROL_SRC,
	EVENT_SRC,
	VALUE_ADC1,
	VALUE_ADC2,
	VALUE_ADC3,
	VALUE_ADC4,
	VALUE_ADC5,
	VALUE_ADC6,
	VALUE_ADC7,
	VALUE_ADC8,

	DATA_NUM
}packet_enum;

typedef enum{
	TYPE_DRONE=0,
	TYPE_PTZ,
	TYPE_PTZRESET,
	TYPE_ZOOMIN,
	TYPE_ZOOMOUT,
	TYPE_ZOOMRESET,

	TYPE_NUM
}type_enum;

typedef struct{
	u8 mode_change;
	u8 changed_enable;
	u8 return_enable;
	u8 crash_stop;
//	u8 set_power;
}control_flag_t;

bool data_received(void);
void data_unpack(packet_t *data);
void do_actions(packet_t pack);
void change_mode(u8 index);
void do_crash_stop(u8 index);
void do_return_home(u8 index);
void set_controller(u8 index);

#endif
