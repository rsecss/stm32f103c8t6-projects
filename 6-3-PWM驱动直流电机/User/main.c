#include "stm32f10x.h"
#include "Delay.h"
#include "OLED.h"
#include "Motor.h"
#include "Key.h"

uint8_t KeyNum = 0; // 定义用于接收键码的变量
int8_t Speed = 0; // 定义速度变量

int main()
{
	/* 模块初始化 */
	OLED_Init();
	Motor_Init();
	Key_Init();

	/* 显示静态字符串 */
	OLED_ShowString(1, 1, "Speed:"); // 1 行 1 列显示字符串 Speed:

	while (1)
	{
		KeyNum = Key_GetNumber(); // 获取按键键码
		if (KeyNum == 1)
		{
			Speed += 20;
			if (Speed > 100) // 最大为 100 度
			{
				Speed = -100; // 此操作会让电机旋转方向突然改变，可能会因供电不足而导致单片机复位；若出现了此现象，则应避免使用这样的操作
			}
		}
		Motor_SetSpeed(Speed);		  // 设置直流电机的速度为速度变量
		OLED_ShowSignedNum(1, 7, Speed, 3);	// OLED 显示速度变量
	}
}
