#include "stm32f10x.h"
#include "Delay.h"
#include "OLED.h"
#include "Timer.h"

uint16_t Number; // 定义在定时器中断里自增的变量
int main()
{
	/* 模块初始化 */
	OLED_Init();
	Timer_Init();

	OLED_ShowString(1, 3, "Number:");
	while (1)
	{
		OLED_ShowNum(1, 5, Number, 5);
	}
}

/**
 * @brief  TIM2 中断函数（可复制到其他地方）
 * @param 无
 * @retval 无
 * @warning 此函数为中断函数，无需调用，中断触发后自动执行；函数名为预留的指定名称，可以从启动文件复制；请确保函数名正确，不能有任何差异，否则中断函数将不能进入
 */
void TIM2_IRQHandler()
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		Number++; // Number 变量自增，用于测试定时中断
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}
