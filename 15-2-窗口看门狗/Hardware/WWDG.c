#include "stm32f10x.h"

/**
 * @brief WWDG 初始化
 * @note 这里的重装值需要按照需求根据公式自行计算，公式：
 * 1. 超时时间（最晚时间）：TWWDG = TPCLK1 × 4096 × WDGTB预分频系数 × (T[5:0] + 1)
 * 2. 窗口时间（最早时间）：TWIN = TPCLK1 × 4096 × WDGTB预分频系数 × (T[5:0] - W[5:0])
 * 这里设置超时时间 = 50ms，窗口时间 = 30ms
 * @param None
 * @retval None
 */
void WWDG_Init()
{
	/* 开启 WWDG 时钟 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);

	/* WWDG 初始化 */
	WWDG_SetPrescaler(WWDG_Prescaler_8); ///< 设置预分频为 8
	WWDG_SetWindowValue(0x40 | 21); ///< 设置窗口值，窗口时间为 30ms
	WWDG_Enable(0x40 | 54); // 使能并第一次喂狗，超时时间为 50ms
}
