#ifndef _LED_DRV_H_
#define _LED_DRV_H_
//#include "binding.h" 

typedef enum{
    LED_SRC_START = 0,
    LED_SRC_1 = LED_SRC_START,
    LED_SRC_2,
    LED_SRC_3,
    LED_SRC_4,
    LED_SRC_5,
    LED_SRC_6,

    LED_SRC_NUM
}led_src_enum;
	
#define LED6_PIN	GPIO_Pin_6
#define LED6_GPIO	GPIOA

#define LED5_PIN	GPIO_Pin_7
#define LED5_GPIO	GPIOA

#define LED4_PIN	GPIO_Pin_4
#define LED4_GPIO	GPIOC

#define LED3_PIN	GPIO_Pin_5
#define LED3_GPIO	GPIOC

#define LED2_PIN	GPIO_Pin_0
#define LED2_GPIO	GPIOB

#define LED1_PIN	GPIO_Pin_1
#define LED1_GPIO	GPIOB

void led_drv_init(void);
void led_on(led_src_enum src);
void led_off(led_src_enum src);

#define LED1_ON      led_on(LED_SRC_1)
#define LED2_ON      led_on(LED_SRC_2)
#define LED3_ON      led_on(LED_SRC_3)
#define LED4_ON      led_on(LED_SRC_4)
#define LED5_ON      led_on(LED_SRC_5)
#define LED6_ON      led_on(LED_SRC_6)

#define LED1_OFF      led_off(LED_SRC_1)
#define LED2_OFF      led_off(LED_SRC_2)
#define LED3_OFF      led_off(LED_SRC_3)
#define LED4_OFF      led_off(LED_SRC_4)
#define LED5_OFF      led_off(LED_SRC_5)
#define LED6_OFF      led_off(LED_SRC_6)

#endif
