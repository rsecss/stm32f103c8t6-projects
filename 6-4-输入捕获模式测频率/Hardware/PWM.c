#include "stm32f10x.h"

/**
 * @brief PWM 初始化
 * 
 * @details 1KHz，占空比为 0，分辨率为 1% 的 PWM 波形
 * @param 无
 * @retval 无
 * @note
 * PWM 频率：Freqency = CK_PSC / (PSC + 1) / (ARR + 1)
 * PWM 占空比：Duty = CCR / (ARR + 1)
 * PWM 分辨率：Reso = 1 / (ARR + 1)
 */
void PWM_Init()
{
    /* 开启时钟 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);  // 开启 TIM2 时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // 开启 GPIOA 时钟

    // /* GPIO 重映射 */
    // RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); // 开启 AFIO 时钟，重映射必须先开启 AFIO 时钟
    // RCC_PinRemapConfig(GPIO_PartialRemap1_TIM2, ENABLE); // 将 TIM2 引脚部分重映射，具体的映射方案需要查看参考手册
    // GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); // 将 JTAG 引脚失能，作为普通 GPIO 引脚使用

    /* GPIO 初始化 */
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; // GPIO_Pin_15
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure); // 将 PA0 引脚初始化为复用推免输出（受外设控制的引脚，均需要配置为复用模式）

    /* 配置时钟源 */
    TIM_InternalClockConfig(TIM2); // 选择 TIM2 为内部时钟，若不调用此函数，TIM 默认也为内部时钟

    /* 初始化时基单元 */
    TIM_TimeBaseInitTypeDef TIM_TimBaseInitStructure;
    TIM_TimBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; // 时钟分频，选择不分频，此参数用于配置滤波器时钟，不影响时基单元功能
    TIM_TimBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; // 计数器模式，选择向上计数
    TIM_TimBaseInitStructure.TIM_Period = 100 - 1; // 计数周期，即 ARR 的值
    TIM_TimBaseInitStructure.TIM_Prescaler = 720 - 1; // 预分频器，即 PSC 的值
    TIM_TimBaseInitStructure.TIM_RepetitionCounter = 0; // 重复计数器，高级定时器才会使用
    TIM_TimeBaseInit(TIM2, &TIM_TimBaseInitStructure); // 将结构体变量交给 TIM_TimeBaseInit，配置 TIM2 的时基单元

    /* 输出比较初始化 */
    TIM_OCInitTypeDef TIM_OCInitStructure; // 定义结构体变量
    TIM_OCStructInit(&TIM_OCInitStructure); // 结构体初始化，若结构体没有完整赋值；则最好执行此函数，给结构体所有成员都赋一个默认值；避免结构体初值不确定的问题
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; // 输出比较模式，选择 PWM 模式 1
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; // 输出极性，选择为高，若选择极性为低，则输出高低电平取反
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; // 输出使能
    TIM_OCInitStructure.TIM_Pulse = 0; // 初始的 CCR 值
    TIM_OC1Init(TIM2, &TIM_OCInitStructure); // 将结构体变量交给 TIM_OC1Init，配置 TIM2 的输出比较通道 1

    /* TIM 使能 */
    TIM_Cmd(TIM2, ENABLE); // 使能 TIM2，定时器开始运行
}

/**
 * @brief PWM 设置 CCR
 * 
 * @param Compare 要写入的 CCR 的值，范围：0~100
 * @retval 无
 * @note CCR 和 ARR 共同决定占空比，此函数仅设置 CCR 的值，并不直接是占空比——占空比 Duty = CCR / (ARR + 1)
 */
void PWM_SetComapare1(uint16_t Compare)
{
    TIM_SetCompare1(TIM2, Compare); // 设置 CCR1 的值
}

/**
 * @brief 调节 PWM 频率
 * 
 * @param Prescaler 
 * @retval 无
 */
void PWM_SetPrescaler(uint16_t Prescaler)
{
    TIM_PrescalerConfig(TIM2, Prescaler, TIM_PSCReloadMode_Immediate);
}
