#ifndef _LED_DRV_H_
#define _LED_DRV_H_
//#include "binding.h" 

typedef enum{
    LED_SRC_START = 0,
    LED_SRC_1 = LED_SRC_START,

    LED_SRC_NUM
}led_src_enum;
	


#define LED1_PIN	GPIO_Pin_5
#define LED1_GPIO	GPIOA

void led_drv_init(void);
void led_on(led_src_enum src);
void led_off(led_src_enum src);

#define LED1_ON      led_on(LED_SRC_1)
#define LED1_OFF      led_off(LED_SRC_1)


#endif
