#include "stm32f10x.h"

/**
 * @brief 编码器初始化
 * 
 * @param 无
 * @retval 无
 */
void Encoder_Init()
{
    /* 开启 TIM3、GPIOA 时钟 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    /* GPIO 初始化：将 PA6 和 PA7 引脚初始化为上拉输入 */
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* 时基单元初始化：时钟选择不分频、计数器模式选择向上计数、ARR 和 PSC 自定义，不使用重复定时器 */
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1; /*!< ARR */
    TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1; /*!< PSC */
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);

    /* 初始化输入捕获：配置定时器通道 1、配置输入滤波参数；配置定时器通道 2、配置输入滤波参数 */
    TIM_ICInitTypeDef TIM_ICInitStructure;
    TIM_ICStructInit(&TIM_ICInitStructure); /*!< 结构体初始化，若结构体没有完整赋值，最好执行此函数，给结构体所有成员都赋一个默认值，避免结构体初值不确定的问题 */
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
    TIM_ICInitStructure.TIM_ICFilter = 0xF;
    TIM_ICInit(TIM3, &TIM_ICInitStructure);
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
    TIM_ICInitStructure.TIM_ICFilter = 0xF;
    TIM_ICInit(TIM3, &TIM_ICInitStructure);

    /* 配置编码接口 */
    TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising); /*!< 配置编码器模式以及两个输入通道是否反相；注意此时参数的 Rising 和 Falling 已经不代表上升沿和下降沿了，而是代表是否反相；此函数必须在输入捕获初始化之后进行，否则输入捕获的配置会覆盖此函数的部分配置 */

    /* TIM 使能 */
    TIM_Cmd(TIM3, ENABLE);
}

/**
 * @brief 获取编码器的增量值
 * 
 * @param 无
 * @return int16_t 自上此调用此函数后，编码器的增量值
 */
int16_t Encoder_Get() 
{
	/* 使用 Temp 变量作为中继，目的是返回 CNT 后将其清零 */
    int16_t Temp;
    Temp = TIM_GetCounter(TIM3);
    TIM_SetCounter(TIM3, 0);

    return Temp;
}
