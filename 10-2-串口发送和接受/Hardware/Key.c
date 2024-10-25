#include "stm32f10x.h"
#include "Delay.h"

/**
 * @brief 按键初始化程序
 * @param 无
 * @retval 无
 */
void Key_Init()
{
    /* 开启时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); // 开启 GPIOB 时钟

    /* GPIO 初始化 */
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure); // 将 PB1 和 PB11 引脚初始化为上拉输入
}

/**
 * @brief 按键获取键码
 * @param 无
 * @retval KeyNumber
 * @attention 此函数是阻塞式操作，当按键按住不放时，函数会卡住，直到按键松手
 */
uint8_t key_GetNumber()
{
    uint8_t KeyNumber = 0;

    if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0)
    {
        Delay_ms(20); // 延时消抖
        while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0); // 读 PB1 输入寄存器的状态，如果为 0，则代表按键 1 按下
        Delay_ms(20);  // 延时消抖
        KeyNumber = 1; // 置键码为 1
    }

    if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11) == 0)
    {
        Delay_ms(20); // 延时消抖
        while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11) == 0); // 读 PB1 输入寄存器的状态，如果为 0，则代表按键 2 按下
        Delay_ms(20);  // 延时消抖
        KeyNumber = 2; // 置键码为 2
    }

    return KeyNumber;
}
