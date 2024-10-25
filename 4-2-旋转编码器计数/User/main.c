#include "stm32f10x.h"
#include "Delay.h"
#include "OLED.h"
#include "Encoder.h"

int16_t Number = 0; // 定义待被旋转编码器调节的变量
int main()
{
	/* 模块初始化 */
	OLED_Init();
	Encoder_Init();

	/* 显示静态字符 */
	OLED_ShowString(1, 1, "Number:");

	while (1)
	{
		Number += Encoder_Get();
		// 获取自上此调用此函数后，旋转编码器的增量值，并将增量值加到 Number 上
		OLED_ShowSignedNum(1, 8, Number, 5); // 显示 Number
	}
}
