#include "stm32f10x.h"
#include "Delay.h"
#include "OLED.h"
#include "InfraredSensor.h"

int main()
{
	/* 模块初始化 */
	OLED_Init();
	InfraredSensor_Init();

	/* 显示静态字符 */
	OLED_ShowString(1, 1, "Count");

	while (1)
	{
		// OLED 不断刷新显示 CountSensor_Get 的返回值
		OLED_ShowNum(1, 7, InfraredSensor_Get(), 5);
	}
}
