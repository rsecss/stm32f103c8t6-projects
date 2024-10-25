#include "stm32f10x.h"
#include "Delay.h"
#include "OLED.h"
#include "Servos.h"
#include "Key.h"

uint8_t KeyNum = 0; // 定义用于接收键码的变量
double Angle = 0.0; // 定义角度变量

int main()
{
	/* 模块初始化 */
	OLED_Init();
	Servos_Init();
	Key_Init();

	/* 显示静态字符串 */
	OLED_ShowString(1, 1, "Angle:"); // 1 行 1 列显示字符串 Angle:

	while (1)
	{
		KeyNum = Key_GetNumber(); // 获取按键键码
		if (KeyNum == 1)
		{
			Angle += 15;	 // 角度变量自增 15
			if (Angle > 180) // 最大为 180 度
			{
				Angle = 0;
			}
		}
		Servos_SetAngle(Angle);		  // 设置舵机的角度为角度变量
		OLED_ShowNum(1, 7, Angle, 3); // OLED 显示角度变量
	}
}
