#include "stm32f10x.h"
#include "Delay.h"
#include "OLED.h"
#include "PWM.h"

int main()
{
	/* 模块初始化 */
	OLED_Init();
	PWM_Init();

	while (1)
	{
		uint8_t i = 0;
		for (i = 0; i < 100; i++)
		{
			PWM_SetComapare1(i); // 依次将定时器的CCR寄存器设置为 0~100，PWM 占空比逐渐增大，LED 逐渐变亮
			Delay_ms(10);
		}
		for (i = 100; i > 0; i--)
		{
			PWM_SetComapare1(i); // 依次将定时器的 CCR 寄存器设置为 100~0，PWM 占空比逐渐减小，LED 逐渐变暗
			Delay_ms(10);
		}
	}
}
