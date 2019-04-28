#include "common.h"
#include "stm32f4xx_gpio.h"
#include "gpio_drv.h"

/*----------------------------------------------------------------------------*/

typedef struct{
    GPIO_TypeDef *port;
    u16         pin;
}gpio_config_t;


STATIC gpio_config_t gpio_config_array[GPIO_SRC_NUM] = {
    {PWRKEY1_GPIO, PWRKEY1_PIN},
    {RTH1_GPIO, RTH1_PIN}, 
    {EMG1_GPIO, EMG1_PIN}, 
    {MODE1_GPIO, MODE1_PIN}, 
    
    {PWRKEY2_GPIO, PWRKEY2_PIN},
    {RTH2_GPIO, RTH2_PIN}, 
    {EMG2_GPIO, EMG2_PIN}, 
    {MODE2_GPIO, MODE2_PIN}, 
    
    {PWRKEY3_GPIO, PWRKEY3_PIN},
    {RTH3_GPIO, RTH3_PIN}, 
    {EMG3_GPIO, EMG3_PIN}, 
    {MODE3_GPIO, MODE3_PIN}, 
    
    {PWRKEY4_GPIO, PWRKEY4_PIN},
	{RTH4_GPIO, RTH4_PIN}, 
    {EMG4_GPIO, EMG4_PIN}, 
    {MODE4_GPIO, MODE4_PIN}, 

	{PWRCTL1_GPIO, PWRCTL1_PIN}, 
	{PWRCTL2_GPIO, PWRCTL2_PIN}, 
	{PWRCTL3_GPIO, PWRCTL3_PIN}, 
	{PWRCTL4_GPIO, PWRCTL4_PIN}, 
};

/*----------------------------------------------------------------------------*/
//global functions
void gpio_drv_init(void)
{
    GPIO_InitTypeDef  gpio_init;
    gpio_src_enum src;
    
    gpio_init.GPIO_Mode  = GPIO_Mode_OUT;
    gpio_init.GPIO_OType = GPIO_OType_PP;
    gpio_init.GPIO_PuPd  = GPIO_PuPd_UP;
    gpio_init.GPIO_Speed = GPIO_Speed_50MHz;

    for(src = GPIO_SRC_START; src < GPIO_SRC_NUM; src++)
    {
        gpio_init.GPIO_Pin = gpio_config_array[src].pin;
        GPIO_Init(gpio_config_array[src].port, &gpio_init);
    }

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

