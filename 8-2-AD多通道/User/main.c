#include "stm32f10x.h"
#include "Delay.h"
#include "OLED.h"
#include "AD.h"

uint16_t AD0, AD1, AD2, AD3; // 定义 AD 变量
int main()
{
	/* 模块初始化 */
	OLED_Init();
	AD_Init();

	/* 显示静态字符串 */
	OLED_ShowString(1, 1, "AD0:");
	OLED_ShowString(2, 1, "AD1:");
	OLED_ShowString(3, 1, "AD2:");
	OLED_ShowString(4, 1, "AD3:");

	while (1)
	{
		AD0 = AD_GetValue(ADC_Channel_0); // 单次启动 ADC，转换通道 0
		AD0 = AD_GetValue(ADC_Channel_1); // 单次启动 ADC，转换通道 1
		AD0 = AD_GetValue(ADC_Channel_2); // 单次启动 ADC，转换通道 2
		AD0 = AD_GetValue(ADC_Channel_3); // 单次启动 ADC，转换通道 3

		OLED_ShowNum(1, 5, AD0, 4);	//显示通道 0的转换结果 AD0
		OLED_ShowNum(2, 5, AD1, 4);	//显示通道 1 的转换结果 AD1
		OLED_ShowNum(3, 5, AD2, 4);	//显示通道 2 的转换结果 AD2
		OLED_ShowNum(4, 5, AD3, 4);	//显示通道 3 的转换结果 AD3

		Delay_ms(100);
	}
}
