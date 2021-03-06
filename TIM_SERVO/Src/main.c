
#include "main.h"


GPIO_InitTypeDef GPIO_InitStruct;
TIM_HandleTypeDef TIM_HandleStruct;
TIM_OC_InitTypeDef TIM_OC_InitStruct;
static void SystemClock_Config(void);

uint32_t uhPrescalerValue = 0;

#define  PERIOD_VALUE       (uint32_t)(22000)  /* Period Value, because we have need about 20 ms Pause between Pulses (20ms+2ms)in ns = 22000ns  */
uint32_t CMP_VAL = 0;

int main(void)
{
    /* -1- Init System*/
    HAL_Init();
    SystemClock_Config();
    /* -2- Calculate Prescaler */
    //We want the Timer to run with 1MHz, so we have a 1ns Timescale
    uhPrescalerValue = (uint32_t) (SystemCoreClock / 1000000) - 1;

    /* -3- Enable Clocks*/
    //GPIO
    __GPIOA_CLK_ENABLE();
    //TIMER
    __TIM1_CLK_ENABLE();

    /* -4- Configure GPIO Pin*/
    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF2_TIM1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* -5- Configure Timer*/
    TIM_HandleStruct.Instance = TIM1;
    TIM_HandleStruct.Init.Prescaler = uhPrescalerValue;
    TIM_HandleStruct.Init.Period = PERIOD_VALUE;
    TIM_HandleStruct.Init.ClockDivision = 0;
    TIM_HandleStruct.Init.CounterMode = TIM_COUNTERMODE_UP;
    TIM_HandleStruct.Init.RepetitionCounter = 0;
    HAL_TIM_PWM_Init(&TIM_HandleStruct);

    /* -6- Configure Timer Output*/
    TIM_OC_InitStruct.OCMode = TIM_OCMODE_PWM1;
    TIM_OC_InitStruct.OCPolarity = TIM_OCPOLARITY_HIGH;
    TIM_OC_InitStruct.OCFastMode = TIM_OCFAST_DISABLE;
    TIM_OC_InitStruct.OCNPolarity = TIM_OCNPOLARITY_HIGH;
    TIM_OC_InitStruct.OCIdleState = TIM_OCIDLESTATE_RESET;
    TIM_OC_InitStruct.OCNIdleState = TIM_OCNIDLESTATE_RESET;
    TIM_OC_InitStruct.Pulse = CMP_VAL;
    HAL_TIM_PWM_ConfigChannel(&TIM_HandleStruct, &TIM_OC_InitStruct, TIM_CHANNEL_3);

    /* -7- Enable Timer Output*/
    HAL_TIM_PWM_Start(&TIM_HandleStruct, TIM_CHANNEL_3);

    while (1) {
        /* -8- From lest to right*/
        for (CMP_VAL = 1000; CMP_VAL != 2000; CMP_VAL++) {
            TIM_OC_InitStruct.Pulse = CMP_VAL;
            HAL_TIM_PWM_ConfigChannel(&TIM_HandleStruct, &TIM_OC_InitStruct, TIM_CHANNEL_3);
            HAL_TIM_PWM_Start(&TIM_HandleStruct, TIM_CHANNEL_3);
            HAL_Delay(2);
        }
        /* -9- From right to left*/
        for (CMP_VAL = 2000; CMP_VAL != 1000; CMP_VAL--) {
            TIM_OC_InitStruct.Pulse = CMP_VAL;
            HAL_TIM_PWM_ConfigChannel(&TIM_HandleStruct, &TIM_OC_InitStruct, TIM_CHANNEL_3);
            HAL_TIM_PWM_Start(&TIM_HandleStruct, TIM_CHANNEL_3);
            HAL_Delay(2);
        }
    }
}

void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct;
    RCC_ClkInitTypeDef RCC_ClkInitStruct;

    // Internal Oscillator Directly (it's fairly accurate, at least better than the AVR one
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = 16;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
    HAL_RCC_OscConfig(&RCC_OscInitStruct);

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0);

}
