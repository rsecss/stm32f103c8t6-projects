#include "stm32f10x.h"

/**
 * @brief 初始化光敏传感器
 * @param 无
 * @retval 无
 */
void LightSensor_Init()
{
	/* 开启时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); // 开启 GPIOB 的时钟

	/* GPIO 初始化 */
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure); // 将 PB13 引脚初始化为上拉输入
}

/**
 * @brief 获取光敏传感器输出的高低电平
 * @param 无
 * @retval 光敏传感器的高低电平，范围 0/1
 */
uint8_t LightSensor_Get()
{
	return GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13);
}
