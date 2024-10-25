#include "stm32f10x.h"
#include "Delay.h"
#include "OLED.h"

int main()
{
	OLED_Init();

	OLED_ShowChar(1, 1, 'A');
	OLED_ShowString(1, 3, "Hello");
	OLED_ShowNum(2, 1, 1234, 4);
	OLED_ShowSignedNum(2, 6, 12345, 5);
	OLED_ShowHexNum(3, 1, 0XAA55, 4);
	OLED_ShowBinNum(4, 1, 0XAA55, 4);

	OLED_Clear();

	while (1)
	{
		
	}
}
