#include "stm32f10x.h"
#include "Delay.h"
#include "OLED.h"
#include "AD.h"

uint16_t ADValue;			// 定义 AD 值变量
float Voltage;				// 定义电压变量 
int main()
{
	/* 模块初始化 */
	OLED_Init();
	AD_Init();

	/* 显示静态字符串 */
	OLED_ShowString(1, 1, "ADvalue:");
	OLED_ShowString(2, 1, "Voltage:");

	while (1)
	{
		ADValue = AD_GetValue(); // 获取 AD 转换的值
		Voltage = (float)ADValue / 4095 * 3.3; // 将 AD 值线性变换到 0~3.3 的范围，表示电压；2^12 = 4096

		OLED_ShowNum(1, 9, ADValue, 4);	// 显示 AD 值
		OLED_ShowNum(2, 9, Voltage, 1);	// 显示电压值的整数部分
		OLED_ShowNum(2, 11, (uint16_t)(Voltage * 100) % 100, 2); // 显示电压值的小数部分

		Delay_ms(100);
	}
}
