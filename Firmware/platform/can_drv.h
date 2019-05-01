#ifndef _CAN_DRV_H_
#define _CAN_DRV_H_

#define CAN1_TX_PIN		GPIO_Pin_9
#define CAN1_RX_PIN		GPIO_Pin_8
#define CAN1_GPIO		GPIOB


u8 CAN1_Init(void);

void CAN1_Send_Msg(u8* msg,u8 len);
u8 CAN1_Receive_Msg(u8 *buf);


#endif
