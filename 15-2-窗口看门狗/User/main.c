#include "stm32f10x.h"
#include "Delay.h"
#include "OLED.h"
#include "key.h"
#include "WWDG.h"

int main()
{
	/* 模块初始化 */
	OLED_Init();
	Key_Init();

	/* 显示静态字符串 */
	OLED_ShowString(1, 1, "WWDG_TEST");

	/* 判断复位信号的来源 */
	if (RCC_GetFlagStatus(RCC_FLAG_WWDGRST) ==  SET)
	{
		/* OLED 闪烁 WWDGRST 字符串 */
		OLED_ShowString(2, 1, "WWDGRST"); 
		Delay_ms(500);
		OLED_ShowString(2, 1, "       ");
		Delay_ms(100);

		RCC_ClearFlag(); // 清除标志位
	}
	else
	{
		/* 闪烁 RST 字符串 */
		OLED_ShowString(3, 1, "RST");
		Delay_ms(500);
		OLED_ShowString(3, 1, "   ");
		Delay_ms(100);
	}

	/* IWDG 初始化 */
	WWDG_Init();

	while (1)
	{ 
		key_GetNumber(); // 调用阻塞式的按键扫描函数，模拟主循环卡死

		/* OLED 闪烁 FEED 字符串 */
		OLED_ShowString(4, 1, "FEED");
		Delay_ms(20); // 喂狗间隔为 20+20=40ms < 50ms
		OLED_ShowString(4, 1, "    ");
		Delay_ms(20);
		WWDG_SetCounter(0x40 | 54);	// 重装计数器，喂狗
	}
}
