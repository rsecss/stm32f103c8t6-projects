#include "stm32f10x.h"
#include "Delay.h"
#include "OLED.h"
#include "Timer.h"
#include "Encoder.h"

/* 定义速度变量 */
int16_t Speed = 0; 
int main()
{
	/* 模块初始化 */
	OLED_Init();
	Timer_Init();
	Encoder_Init();

	OLED_ShowString(1, 1, "Speed:");
	while (1)
	{
		OLED_ShowSignedNum(1, 7, Speed, 5);
	}
}

/**
 * @brief  TIM2 中断函数（可复制到其他地方）
 * @param 无
 * @retval 无
 * @warning 
 *  此函数为中断函数，无需调用，中断触发后自动执行；函数名为预留的指定名称，
 * 	可以从启动文件复制；请确保函数名正确，不能有任何差异，否则中断函数将不能进入
 */
void TIM2_IRQHandler()
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET) /*!< 判断是否是 TIM2 的更新事件触发 */
	{
		Speed = Encoder_Get(); /*!< 每隔固定时间段读取一次编码器计数增量值，即为速度值 */
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}
