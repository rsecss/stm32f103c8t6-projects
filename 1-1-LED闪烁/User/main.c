#include "stm32f10x.h" // Device header
#include "Delay.h"

int main()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	while (1)
	{
		/* 这两段代码的作用是相同的 */

		GPIO_ResetBits(GPIOA, GPIO_Pin_0); // 点亮 LED
		Delay_ms(500);
		GPIO_SetBits(GPIOA, GPIO_Pin_0); // 熄灭 LED
		Delay_ms(500);

//		GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_RESET); // 点亮 LED
//		Delay_ms(500);
//		GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_RESET); // 熄灭 LED
//		Delay_ms(500);
	}
}
