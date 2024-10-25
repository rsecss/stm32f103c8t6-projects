#include "stm32f10x.h"

/**
 * @brief IWDG 初始化
 * @note 这里的重装值需要按照需求根据公式自行计算，公式：TIWDG = TLSI × PR预分频系数 × (RL + 1)，其中这里的 TIWDG = 5000ms，TLSI = 1/40KHz =0.025，PR预分频系数 = 16
 * @param None
 * @retval None
 */
void IWDG_Init()
{
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable); // 独立看门狗写使能
	IWDG_SetPrescaler(IWDG_Prescaler_16); // 设置预分频为16
	IWDG_SetReload(12499); // 设置重装值为 12499，独立看门狗的超时时间为 5000ms
	IWDG_ReloadCounter(); // 重装计数器，喂狗
	IWDG_Enable(); // 独立看门狗使能
}
