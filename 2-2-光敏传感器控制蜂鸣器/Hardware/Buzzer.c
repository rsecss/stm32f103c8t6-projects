#include "stm32f10x.h"

/**
 * @brief 蜂鸣器初始化
 * @param 无
 * @retval 无
 */
void Buzzer_Init()
{
    /* 开启时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); // 开启 GPIOB 的时钟

    /* GPIO 初始化*/
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure); // 将 PB12 引脚初始化为推挽输出

    /* 设置 GPIO 初始化后的默认电平 */
    GPIO_SetBits(GPIOB, GPIO_Pin_12); // 设置 PB12 脚为高电平
}

/**
 * @brief 开启蜂鸣器
 * @param 无
 * @retval 无
 */

void Buzzer_ON()
{
    GPIO_ResetBits(GPIOB, GPIO_Pin_12); // 设置 PB12 引脚为低电平
}

/**
 * @brief 关闭蜂鸣器
 * @param 无
 * @retval 无
 */
void Buzzer_OFF()
{
    GPIO_SetBits(GPIOB, GPIO_Pin_12); // 设置 PB12 引脚为高电平
}

/**
 * @brief 翻转蜂鸣器
 * @param 无
 * @retval 无
 */
void Buzzer_Turn()
{
    if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12) == 0) // 获取输出寄存器的状态，如果当前引脚输出低电平
    {
        GPIO_SetBits(GPIOB, GPIO_Pin_12); // 设置 PB12 引脚为高电平
    }
    else
    {
        GPIO_ResetBits(GPIOB, GPIO_Pin_12); // 设置 PB12 引脚为低电平
    }
}
