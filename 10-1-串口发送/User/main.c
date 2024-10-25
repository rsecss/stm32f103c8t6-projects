#include "stm32f10x.h"
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"

int main()
{
	/* 模块初始化 */
	OLED_Init();
	Serial_Init();

	/* 串口发送一个字节 */
	Serial_SendByte(0x41);

	/* 串口发送一个数组 */
	uint8_t testArray[] = {0x42, 0x43, 0x44, 0x45};
	Serial_SendArray(testArray, 4);

	/* 串口发送字符串 */
	Serial_SendString("\r\nNumber1=");

	/* 串口发送数字 */
	Serial_SendNumber(1234, 4);


	/*下述 3 种方法可实现 printf 的效果 */

	/*First：直接重定向 printf，但 printf 函数只有一个，此方法不能在多处使用 */
	printf("\r\nNumber2=%d", 222); // 串口发送 printf 打印的格式化字符串（需要重定向 fputc 函数，并在工程选项里勾选 Use MicroLIB）
	
	/* Second：使用 sprintf 打印到字符数组，再用串口发送字符数组，此方法打印到字符数组，之后想怎么处理都可以，可在多处使用 */
	uint8_t String[100];
	sprintf(String, "\r\nNumber3=%d", 555); // 使用 sprintf，把格式化字符串打印到字符数组
	Serial_SendString(String); // 串口发送字符数组（字符串）

	/* Third：将 sprintf 函数封装起来，实现专用的 printf，此方法就是把方法 2 封装起来，更加简洁实用，可在多处使用 */
	Serial_Printf("\r\nNumber4=%d", 999); // 串口打印字符串，使用自己封装的函数实现 printf 的效果
	Serial_Printf("\r\n");
	while (1)
	{
		
	}
}
