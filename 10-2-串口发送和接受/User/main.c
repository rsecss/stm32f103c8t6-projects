#include "stm32f10x.h"
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"

uint8_t RxData;	// 定义用于接收串口数据的变量

int main()
{
	/* 模块初始化 */
	OLED_Init();
	Serial_Init();

	/* 显示静态字符 */
	OLED_ShowString(1, 1, "RXData:");

	while (1)
	{
		if (Serial_GetRxFlag() == 1)
		{
			RxData = Serial_GetRxData(); // 获取串口接收的数据
			Serial_SendByte(RxData); // 串口将收到的数据回传回去，用于测试
			OLED_ShowHexNum(1, 8, RxData, 2); // 显示串口接收的数据
		}
	}
}
