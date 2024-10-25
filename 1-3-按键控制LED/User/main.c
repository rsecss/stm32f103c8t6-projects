#include "stm32f10x.h"
#include "Delay.h"
#include "LED.h"
#include "key.h"

uint8_t key_number; // 定义用于接收按键键码的变量
int main()
{
    /* 模块初始化 */
    LED_Init(); // LED 初始化
    Key_Init(); // 按键初始化

    while (1)
    {
        // LED1_ON();
        // Delay_ms(500);
        // LED1_OFF();
        // Delay_ms(500);

        key_number = key_GetNumber(); // 按键获取键码

        // 按键1按下
        if (key_number == 1)
        {
            LED1_Turn(); // 翻转 LED1
        }

        // 按键 2 按下
        if (key_number == 2)
        {
            LED2_Turn(); // 翻转 LED2
        }
    }
}
