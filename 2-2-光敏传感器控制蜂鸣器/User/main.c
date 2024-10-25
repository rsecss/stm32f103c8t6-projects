#include "stm32f10x.h"
#include "Delay.h"
#include "Buzzer.h"
#include "LightSensor.h"

uint8_t key_number; // 定义用于接收按键键码的变量
int main()
{
    /* 模块初始化 */
    Buzzer_Init(); // 蜂鸣器初始化
    LightSensor_Init(); // 光敏传感器初始化
    
    while (1)
    {
        if (LightSensor_Get() == 1) // 光敏输出为1
        {
            Buzzer_ON(); // 开启蜂鸣器
        }
        else
        {
            Buzzer_OFF(); // 关闭蜂鸣器
        }
    }
}
