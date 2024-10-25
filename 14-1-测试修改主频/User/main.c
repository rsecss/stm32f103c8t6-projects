#include "stm32f10x.h"
#include "Delay.h"
#include "OLED.h"

int main()
{
	OLED_Init();

	/* 显示静态字符串 */
	OLED_ShowString(1, 1, "SYSCLK:");

	/* 显示 SystemCoreClock 变量，SystemCoreClock 的值表示当前的系统主频频率 */
	OLED_ShowNum(1, 8, SystemCoreClock, 8);

	while (1)
	{
		/* 闪烁 Running，指示当前主循环运行的快慢 */
		OLED_ShowString(2, 1, "Running");
		Delay_ms(500);
		OLED_ShowString(2, 1, "       ");
		Delay_ms(500);
	}
}
