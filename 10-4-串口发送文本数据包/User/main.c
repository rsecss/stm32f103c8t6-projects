#include "stm32f10x.h"
#include "Delay.h"
#include "OLED.h"
#include "LED.h"
#include "Serial.h"
#include <string.h>

uint8_t KeyNumber;	// 定义用于接收按键键码的变量

int main()
{
	/* 模块初始化 */
	OLED_Init();
	LED_Init();
	Serial_Init();

	/* 显示静态字符 */
	OLED_ShowString(1, 1, "TxPacket");
	OLED_ShowString(3, 1, "RxPacket");

	while (1)
	{
		if (Serial_RxFlag == 1) 
		{
			OLED_ShowString(4, 1, "                ");
			OLED_ShowString(4, 1, Serial_RxPacket);	

			/* 将收到的数据包与预设的指令对比，以此决定将要执行的操作 */
			if (strcmp(Serial_RxPacket, "LED_ON") == 0)
			{
				LED1_ON(); // 开灯
				Serial_SendString("ON\r\n");
				OLED_ShowString(2, 1, "                ");
				OLED_ShowString(2, 1, "ON"); // OLED 清除指定位置，并显示 ON			
			}
			else if (strcmp(Serial_RxPacket, "LED_OFF") == 0)
			{
				LED1_OFF(); // 关灯
				Serial_SendString("OFF\r\n");
				OLED_ShowString(2, 1, "                ");
				OLED_ShowString(2, 1, "OFF"); // OLED 清除指定位置，并显示 OFF	
			}
			else
			{
				Serial_SendString("ERROR\r\n");
				OLED_ShowString(2, 1, "                ");
				OLED_ShowString(2, 1, "ERROR"); // OLED 清除指定位置，并显示ERROR
			}

			Serial_RxFlag = 0; // 处理完成后，需要将接收数据包标志位清零，否则将无法接收后续数据包
		}
	}
}
