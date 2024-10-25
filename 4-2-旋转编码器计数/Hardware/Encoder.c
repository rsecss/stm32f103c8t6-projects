#include "stm32f10x.h"

int16_t Encoder_Count = 0; // 计数旋转编码器的增量值

/**
 * @brief 初始化旋转编码器
 * @param 无
 * @retval 无
 */
void Encoder_Init()
{
    /* 开启时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); // 开启 PIOB 的时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);  // 开启 AFIO 的时钟，外部中断必须开启 AFIO 的时钟

    /* GPIO初始化 */
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure); // 将 PB0 和 PB1 引脚初始化为上拉输入

    /* AFIO 选择中断引脚 */
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource0); // 将外部中断的 0 号线映射到 GPIOB，即选择 PB0 为外部中断引脚
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource1); // 将外部中断的 1号线映射到 GPIOB，即选择 PB1 为外部中断引脚

    /* EXTI 初始化 */
    EXTI_InitTypeDef EXTI_InitStructure;
    EXTI_InitStructure.EXTI_Line = EXTI_Line0 | EXTI_Line1; // 选择配置外部中断的 0 号线和 1 号线
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;               // 指定外部中断线使能
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;     // 指定外部中断线为中断模式
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; // 指定外部中断线为下降沿触发
    EXTI_Init(&EXTI_InitStructure);

    /* NVIC 中断分组 */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 配置 NVIC 为分组 2

    /*NVIC配置*/
    NVIC_InitTypeDef NVIC_InitStructure;                      // 定义结构体变量
    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;          // 选择配置 NVIC 的 EXTI0 线
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           // 指定 NVIC 线路使能
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; // 指定 NVIC 线路的抢占优先级为 1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;        // 指定 NVIC 线路的响应优先级为 1
    NVIC_Init(&NVIC_InitStructure);                           // 将结构体变量交给 NVIC_Init，配置 NVIC 外设
}

/**
 * @brief 旋转编码器计数
 * @param 无
 * @retval 自上次调用此函数后，旋转编码器的增量值
 * @attention 使用 Temp 变量作为中继，目的是返回Encoder_Count 后将其清零;在这里，也可以直接返 Encoder_Count 但这样就不是获取增量值的操作方法了，也可以实现功能，只是思路不一样
 */
int16_t Encoder_Get()
{
    int16_t temp = 0;
    temp = Encoder_Count;
    Encoder_Count = 0;

    return temp;
}

/**
 * @brief EXTI0 外部中断函数
 * @param 无
 * @retval 无
 * @attention 此函数为中断函数，无需调用，中断触发后自动执行；函数名为预留的指定名称，可以从启动文件复制；请确保函数名正确，不能有任何差异，否则中断函数将不能进入
 * @attention 设置中断标志位后，后面必须清除，否则中断将连续不断地触发，导致主程序卡死
 */
void EXTI0_IRQHandler()
{
    if (EXTI_GetITStatus(EXTI_Line0) == SET) // 判断是否是外部中断 1 号线触发的中断
    {
        if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 0) // 如果出现数据乱跳的现象，可再次判断引脚电平，以避免抖动
        {
            if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0) // PB0 的下降沿触发中断，此时检测另一相 PB1 的电平，目的是判断旋转方向
            {
                Encoder_Count--; // 此方向定义为反转，计数变量自减
            }
        }
        EXTI_ClearITPendingBit(EXTI_Line0); // 清除外部中断0 号线的中断标志位
    }
}

/**
 * @brief EXTI1 外部中断函数
 * @param 无
 * @retval 无
 * @attention 此函数为中断函数，无需调用，中断触发后自动执行；函数名为预留的指定名称，可以从启动文件复制；请确保函数名正确，不能有任何差异，否则中断函数将不能进入
 * @attention 设置中断标志位后，后面必须清除，否则中断将连续不断地触发，导致主程序卡死
 */
void EXTI1_IRQHandler()
{
    if (EXTI_GetITStatus(EXTI_Line1) == SET) // 判断是否是外部中断 1 号线触发的中断
    {
        if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0) // 如果出现数据乱跳的现象，可再次判断引脚电平，以避免抖动
        {
            if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 0) // PB1 的下降沿触发中断，此时检测另一相 PB0 的电平，目的是判断旋转方向
            {
                Encoder_Count++; // 此方向定义为正转，计数变量自增
            }
        }
        EXTI_ClearITPendingBit(EXTI_Line1); // 清除外部中断 0 号线的中断标志位
    }
}
