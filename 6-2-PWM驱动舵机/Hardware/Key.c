#include "stm32f10x.h"
#include "Delay.h"

/**
 * @brief 按键初始化
 * @param 无
 * @retval 无
 */
void Key_Init()
{
    /* 开启时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); // 开启GPIOB的时钟

    /* GPIO 初始化 */
    GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);						// 将 PB1 引脚初始化为上拉输入
}

/**
 * @brief 按键获取键码
 * @param 无
 * @retval 按下按键的键码值，范围：0~2，返回 0 代表没有按键按下
 * @attention 此函数是阻塞式操作，当按键按住不放时，函数会卡住，直到按键松手
 */
uint8_t Key_GetNumber()
{
    uint8_t KeyNumber = 0;		// 定义变量，默认键码值为 0
    if (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1) == 0) // 读 PB1 输入寄存器的状态，如果为 0，则代表按键 1 按下
    {
        Delay_ms(20); // 延时消抖
        while (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1) == 0); // 等待按键松手
        Delay_ms(20); // 延时消抖
        
        KeyNumber = 1; // 置键码为 1
    }
    return KeyNumber; // 返回键码值，如果没有按键按下，所有 if 都不成立，则键码为默认值 0
}
