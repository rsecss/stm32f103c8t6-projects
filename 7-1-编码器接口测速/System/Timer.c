#include "stm32f10x.h"

/**
 * @brief 初始化定时中断
 * 
 * @param 无
 * @retval 无
 */
void Timer_Init()
{
    /* 开启时钟 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); // 开启 TIM2 时钟

    /* 配置时钟源 */
    TIM_InternalClockConfig(TIM2); // 选择 TIM2 为内部时钟，若不调用此函数，TIM 默认也为内部时钟

    /* 初始化时基单元 */
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; // 时钟分频，选择不分频，此参数用于配置滤波器时钟，不影响时基单元功能
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period = 1000 - 1;     // ARR(计数周期)
    TIM_TimeBaseInitStructure.TIM_Prescaler = 7200 - 1;  // PSC(预分频器)
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0; // 重复计数器，高级定时器才会用
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);  // 将结构体变量交给 TIM_TimeBaseInit，配置 TIM2 的时基单元

    /* 中断输出配置 */
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);      // 清除定时器更新标志位（由于 TIM_TimeBaseInit 函数末尾，手动产生了更新事件，若不清除此标志位，则开启中断后，会立刻进入一次中断，如果不介意此问题，则不清除此标志位也可）
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); // 开启 TIM2 的更新中断

    /* NVIC 中断分组 */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 配置 NVIC 为分组 2，即抢占优先级范围：0~3，响应优先级范围：0~3

    /* NVIC 配置 */
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;           // 选择配置 NVIC 的 TIM2 线
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           // 指定 NVIC 线路使能
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; // 指定 NVIC 线路的抢占优先级为 2
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;        // 指定 NVIC 线路的响应优先级为 2
    NVIC_Init(&NVIC_InitStructure);                           // 将结构体变量交给 NVIC_Init，配置 NVIC 外设

    /* TIM 使能 */
    TIM_Cmd(TIM2, ENABLE); // 使能 TIM2，定时器开始运行
}

/**
 * @brief  TIM2 中断函数(可复制到其他地方)
 * @param 无
 * @retval 无
 * @attention 此函数为中断函数，无需调用，中断触发后自动执行；函数名为预留的指定名称，可以从启动文件复制；请确保函数名正确，不能有任何差异，否则中断函数将不能进入
 */
// void TIM2_IRQHandler()
// {
//     if (TIM_GetITStatus(TIM2,TIM_IT_Update) == SET)
//     {

//         TIM_ClearITPendingBit(TIM2, TIM_IT_Update); 	//清除TIM2 更新事件的中断标志位；中断标志位必须清除；否则中断将连续不断地触发，导致主程序卡死
//     }
// }
