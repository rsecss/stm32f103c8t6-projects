#include "stm32f10x.h"

/**
 * @brief 输入捕获初始化
 * 
 * @param 无
 * @retval 无
 */
void IC_Init()
{
    /* 开启时钟 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_APB2PeriphClockCmd(GPIOA, ENABLE);

    /* GPIO 初始化 */
    GPIO_InitTypeDef GPIO_InitStrucure;
    GPIO_InitStrucure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStrucure.GPIO_Pin = GPIO_Pin_6; 
    GPIO_InitStrucure.GPIO_Speed = GPIO_Speed_50MHz;
    
    /* 配置时钟源 */
    TIM_InternalClockConfig(TIM3); // 选择 TIM3 为内部时钟，若不调用此函数，TIM 默认也为内部时钟

    /* 时基单元初始化 */
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; // 时钟分频，不分频
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; // 计数器模式，选择向上计数
    TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1; // ARR
    TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1; // PSC
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0; // 重复计数器，高级定时
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure); // 将结构体变量交给 TIM_TimeBaseInit，配置 TIM3 的时基单元

    /* 输入捕获初始化 */
    TIM_ICInitTypeDef TIM_ICInitStructure;
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1; // 选择定时器通道 1
    TIM_ICInitStructure.TIM_ICFilter = 0xF; // 输入滤波器参数，可以过滤信号抖动
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising; // 极性，选择为上升沿触发捕获
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1; // 
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; // 输入信号交叉，选择直通，不交叉
    TIM_ICInit(TIM3, &TIM_TimeBaseInitStructure); // 将结构体变量交给 TIM_ICInit，配置 TIM3 的输入捕获通道

    /* 选择触发源及从模式 */
    TIM_SelectInputTrigger(TIM3, TIM_TS_ITR1); // 触发源选择 TI1FP1
    TIM_SelectSlaveMode(TIM3, TIM_SlaveMode_Reset); // 从模式选择复位，即 TI1 产生上升沿时，会触发 CNT 归零

    /* TIM 使能 */
    TIM_Cmd(TIM3, ENABLE); // 使能 TIM3，定时器开始运行
}

/**
 * @brief 获取输入捕获的频率 
 * 
 * @param 无
 * @return uint32_t 捕获得到得频率
 * @note 测周法得到频率 fx = fc / N
 */
uint32_t IC_GetFreq()
{
    return 1000000 / (TIM_GetCapture1(TIM3) + 1); // ，这里不执行 +1 的操作也可，fc = 72M / PSC = 1000000
}
