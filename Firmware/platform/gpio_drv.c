#include "common.h"
#include "stm32f4xx_gpio.h"
#include "gpio_drv.h"

/*----------------------------------------------------------------------------*/

typedef struct{
    GPIO_TypeDef *port;
    u16         pin;
	GPIOPuPd_TypeDef GPIO_PuPd;
}gpio_config_t;


STATIC gpio_config_t gpio_config_array[GPIO_SRC_NUM] = {
	{PWRCTL1_GPIO, PWRCTL1_PIN,GPIO_PuPd_UP}, 
	{PWRCTL2_GPIO, PWRCTL2_PIN,GPIO_PuPd_UP}, 
	{PWRCTL3_GPIO, PWRCTL3_PIN,GPIO_PuPd_UP}, 
	{PWRCTL4_GPIO, PWRCTL4_PIN,GPIO_PuPd_UP}, 
	
    {PWRKEY_L1_GPIO, PWRKEY_L1_PIN,GPIO_PuPd_DOWN},
	{PWRKEY_R1_GPIO, PWRKEY_R1_PIN,GPIO_PuPd_UP},
    {RTH1_GPIO, RTH1_PIN,GPIO_PuPd_DOWN,}, 
    {EMG1_GPIO, EMG1_PIN,GPIO_PuPd_DOWN}, 
    {MODE1_GPIO, MODE1_PIN,GPIO_PuPd_UP}, 
    
    {PWRKEY2_GPIO, PWRKEY2_PIN,GPIO_PuPd_UP},
    {RTH2_GPIO, RTH2_PIN,GPIO_PuPd_DOWN}, 
    {EMG2_GPIO, EMG2_PIN,GPIO_PuPd_DOWN}, 
    {MODES2_GPIO, MODES2_PIN,GPIO_PuPd_DOWN},
    {MODEP2_GPIO, MODEP2_PIN,GPIO_PuPd_UP},
    {MODET2_GPIO, MODET2_PIN,GPIO_PuPd_DOWN},
    
    {PWRKEY3_GPIO, PWRKEY3_PIN,GPIO_PuPd_UP},
    {RTH3_GPIO, RTH3_PIN,GPIO_PuPd_DOWN}, 
    {EMG3_GPIO, EMG3_PIN,GPIO_PuPd_DOWN}, 
    {MODES3_GPIO, MODES3_PIN,GPIO_PuPd_DOWN}, 
    {MODEP3_GPIO, MODEP3_PIN,GPIO_PuPd_UP},
    {MODET3_GPIO, MODET3_PIN,GPIO_PuPd_DOWN},
    
    {PWRKEY4_GPIO, PWRKEY4_PIN,GPIO_PuPd_DOWN},
	{RTH4_GPIO, RTH4_PIN,GPIO_PuPd_DOWN}, 
    {EMG4_GPIO, EMG4_PIN,GPIO_PuPd_DOWN}, 
    {MODEP4_GPIO, MODEP4_PIN,GPIO_PuPd_UP}, 
    {MODEA4_GPIO, MODEA4_PIN,GPIO_PuPd_DOWN}, 
};

/*----------------------------------------------------------------------------*/
//global functions
void gpio_drv_init(void)
{
    GPIO_InitTypeDef  gpio_init;
    gpio_src_enum src;
    
    gpio_init.GPIO_Mode  = GPIO_Mode_OUT;
    gpio_init.GPIO_OType = GPIO_OType_PP;
    //gpio_init.GPIO_PuPd  = GPIO_PuPd_UP;
    gpio_init.GPIO_Speed = GPIO_Speed_50MHz;

    for(src = GPIO_SRC_START; src < GPIO_SRC_NUM; src++)
    {
        gpio_init.GPIO_Pin = gpio_config_array[src].pin;
		gpio_init.GPIO_PuPd = gpio_config_array[src].GPIO_PuPd;
        GPIO_Init(gpio_config_array[src].port, &gpio_init);
    }
	gpio_value_set(GPIO_SRC_PWRKEY_R1);
	gpio_value_set(GPIO_SRC_MODE1);
	gpio_value_set(GPIO_SRC_PWRKEY2);
	gpio_value_set(GPIO_SRC_MODEP_2);
	gpio_value_set(GPIO_SRC_PWRKEY3);
	gpio_value_set(GPIO_SRC_MODEP_3);
	gpio_value_set(GPIO_SRC_MODEP_4);
	
}


void gpio_value_set(u8 src)
{
    ASSERT_D(src < GPIO_SRC_NUM);
    GPIO_SetBits(gpio_config_array[src].port, gpio_config_array[src].pin);
}

void gpio_value_reset(u8 src)
{
    ASSERT_D(src < GPIO_SRC_NUM);
    GPIO_ResetBits(gpio_config_array[src].port, gpio_config_array[src].pin);
}

