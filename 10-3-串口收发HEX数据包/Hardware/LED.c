#include "stm32f10x.h"

/**
 * @brief LED初始化程序
 * @param 无
 * @retval 无
 */
void LED_Init()
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // 开启时钟

    /* GPIO 初始化 */
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure); // 将 PA1 和 PA2 引脚初始化为推挽输出

    /* 设置 GPIO 初始化后的默认电平 */
    GPIO_SetBits(GPIOA, GPIO_Pin_1 | GPIO_Pin_2);
}

/**
 * @brief 开启 LED1
 * @param 无
 * @retval 无
 */
void LED1_ON()
{
    GPIO_ResetBits(GPIOA, GPIO_Pin_1);
}

/**
 * @brief 关闭 LED1
 * @param 无
 * @retval 无
 */
void LED1_OFF()
{
    GPIO_SetBits(GPIOA, GPIO_Pin_1);
}

/**
 * @brief 开启 LED2
 * @param 无
 * @retval 无
 */
void LED2_ON()
{
    GPIO_ResetBits(GPIOA, GPIO_Pin_2);
}

/**
 * @brief 关闭 LED2
 * @param 无
 * @retval 无
 */
void LED2_OFF()
{
    GPIO_SetBits(GPIOA, GPIO_Pin_2); // 设置 PA2 引脚为高电平
}

/**
 * @brief 翻转 LED1
 * @param 无
 * @retval 无
 */
void LED1_Turn()
{
    if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) == 0)
    {
        GPIO_SetBits(GPIOA, GPIO_Pin_1); // 设置 PA2 引脚为高电平
    }
    else
    {
        GPIO_ResetBits(GPIOA, GPIO_Pin_1); // 设置 PA2 引脚为低电平
    }
}

/**
 * @brief 翻转 LED2
 * @param 无
 * @retval 无
 */
void LED2_Turn()
{
    if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2) == 0)
    {
        GPIO_SetBits(GPIOA, GPIO_Pin_2); // 设置 PA2 引脚为高电平
    }
    else
    {
        GPIO_ResetBits(GPIOA, GPIO_Pin_2); // 设置 PA2 引脚为低电平
    }
}
