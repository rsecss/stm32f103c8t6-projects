#include "stm32f10x.h"
#include "Delay.h"
#include "OLED.h"
#include "InfraredSensor.h"

int main()
{
	/* 模块初始化 */
	OLED_Init();
	InfraredSensor_Init();

	/* 开启 PWR 时钟 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE); // 停止模式和待机模式一定要记得开启


	/* 显示静态字符 */
	OLED_ShowString(1, 1, "Count:");

	while (1)
	{
		// OLED 不断刷新显示 CountSensor_Get 的返回值
		OLED_ShowNum(1, 7, InfraredSensor_Get(), 5);

		/* OLED 闪烁显示 Runing */
		OLED_ShowString(2, 1, "Running");
		Delay_ms(100);
		OLED_ShowString(2, 1, "       ");
		Delay_ms(100);

		PWR_EnterSTOPMode(PWR_Regulator_ON, PWR_STOPEntry_WFI); // STM32 进入停止模式，并等待中断唤醒
		SystemInit(); // 唤醒后，要重新配置时钟
	}
}
