#include "stm32f10x.h" // Device header
#include "Delay.h"

int main()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	while (1)
	{
		GPIO_Write(GPIOA, ~0X0001); // 0000 0000 0000 0001
		Delay_ms(500);
		GPIO_Write(GPIOA, ~0X0002); // 0000 0000 0000 0010
		Delay_ms(500);
		GPIO_Write(GPIOA, ~0X0004); // 0000 0000 0000 0100
		Delay_ms(500);
		GPIO_Write(GPIOA, ~0X0008); // 0000 0000 0000 1000
		Delay_ms(500);
		GPIO_Write(GPIOA, ~0X0010); // 0000 0000 0001 0000
		Delay_ms(500);
		GPIO_Write(GPIOA, ~0X0020); // 0000 0000 0010 0000
		Delay_ms(500);
		GPIO_Write(GPIOA, ~0X0040); // 0000 0000 0100 0000
		Delay_ms(500);
		GPIO_Write(GPIOA, ~0X0080); // 0000 0000 1000 0000
		Delay_ms(500);
	}
}
