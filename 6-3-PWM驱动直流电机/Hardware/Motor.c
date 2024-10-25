#include "stm32f10x.h"
#include "PWM.h"

/**
 * @brief 直流电机初始化
 * @param 无
 * @retval 无
 */
void Motor_Init()
{
    /* 开启时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // 开启 GPIOA 的时钟

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure); // 将 PA4 和 PA5 引脚初始化为推挽输出

    PWM_Init(); // 初始化直流电机的底层 PWM
}

/**
 * @brief 直流电机设置速度
 * @param Speed 要设置的速度，范围：-100~100
 * @retval 无
 */
void Motor_SetSpeed(int8_t Speed)
{
    if (Speed >= 0) // 如果设置正转的速度值
    {
        /* 假设设置方向为正转 */
        GPIO_SetBits(GPIOA, GPIO_Pin_4);   // PA4 置高
        GPIO_ResetBits(GPIOA, GPIO_Pin_5); // PA5 置低
        
		PWM_SetCompare3(Speed); // 设置速度
    }
    else // 反转
    {
        GPIO_ResetBits(GPIOA, GPIO_Pin_4); // PA4 置低
        GPIO_SetBits(GPIOA, GPIO_Pin_5);   // PA5 置高，设置方向为反转

        PWM_SetCompare3(-Speed); // 由于此时速度值为负数，而 PWM 只能给正数，所以 PWM 设置为负的速度值，
    }
}
