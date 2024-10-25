#include "stm32f10x.h"
#include "Delay.h"
#include "OLED.h"
#include "key.h"
#include "Store.h"

uint8_t KeyNumber;
int main()
{
	/* 模块初始化 */
	OLED_Init();
	Key_Init();
	Store_Init();

	/* 显示静态字符串 */
	OLED_ShowString(1, 1, "Flag:");
	OLED_ShowString(2, 1, "Data:");

	while (1)
	{
		KeyNumber = key_GetNumber(); // 获取按键键码
		if (KeyNumber == 1)
		{
			/* 测试 */
			Store_Data[1] ++;
			Store_Data[2] += 2;
			Store_Data[3] += 3;
			Store_Data[4] += 4;

			/* 将 Store_Data 的数据备份保存到闪存，实现掉电不丢失 */
			Store_Save();
		}
		if (KeyNumber == 2)
		{
			Store_Clear(); // Store_Data 的数据全部清 0
		}

		/* 显示 Store_Data 的第一位标志位 */
		OLED_ShowHexNum(1, 6, Store_Data[0], 4); 

		/* 显示 Store_Data 的有效存储数据 */
		OLED_ShowHexNum(3, 1, Store_Data[1], 4);
		OLED_ShowHexNum(3, 6, Store_Data[2], 4);
		OLED_ShowHexNum(4, 1, Store_Data[3], 4);
		OLED_ShowHexNum(4, 6, Store_Data[4], 4);
	}
}
