#include "stm32f10x.h"
#include "PWM.h"

/**
 * @brief 舵机初始化
 * @param 无
 * @retval 无
 */
void Servos_Init()
{
    PWM_Init(); // 初始化舵机的底层PWM
}

/**
 * @brief 舵机设置角度
 * @param Angle 要设置的舵机角度，范围：0~180，这里需要进行缩放，角度：0~180，CCR2：500~2500，即 Angle = Angle / 180 * (2500 - 500) + 偏移量（500）
 * @retval 无
 */
void Servos_SetAngle(double Angle)
{
    PWM_SetCompare2(Angle / 180 * 2000 + 500); // 设置占空比（将角度线性变换，对应到舵机要求的占空比范围上）
}
