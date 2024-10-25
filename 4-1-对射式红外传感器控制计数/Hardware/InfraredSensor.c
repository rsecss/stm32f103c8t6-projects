#include "stm32f10x.h"

uint16_t count = 0; // 用于计数

/**
 * @brief 红外传感器初始化,这里的外部中断的配置步骤：开启时钟->配置 GPIO->选择 AFIO 中断引脚->EXTI 初始化->选择 NVIC 中断分组->配置 NVIC->完成
 * @param 无
 * @retval 无
 * @attention  NVIC 中断分组配置，如果没有其他的中断，可以随意选择中断分组，其中中断分组 2 比较均匀，并且该分组在整个工程中仅需调用一次；如果有多个中断，可以把 NVIC 配置这部分代码放在 main 函数内， while 函数之前；如果调用多次配置分组的代码，则后执行的配置会覆盖先执行的配置
 */
void InfraredSensor_Init()
{
    /* 开启时钟，这里不需要打开 EXTI and NVIC 时钟（内核外设时钟是默认打开） */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); // 开启 GPIOB 的时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);  // 开启 AFIO 的时钟，外部中断必须开启 AFIO 的时钟

    /* 将 PB14 引脚初始化为上拉输入 */
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* AFIO 选择中断引脚 */
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource14); // 将外部中断的 14 号线映射到 GPIOB，即选择 PB14 为外部中断引脚

    /* EXTI 初始化 */
    EXTI_InitTypeDef EXTI_InitStructure;
    EXTI_InitStructure.EXTI_Line = EXTI_Line14;            // 选择配置外部中断的 14 号线
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;              // 外部中断线使能
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;    // 外部中断线为中断模式
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; // 外部中断线为上升沿触发
    EXTI_Init(&EXTI_InitStructure);                        // 将结构体变量交给 EXTI_Init，配置 EXTI 外设

    /* NVIC 中断分组 */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 配置 NVIC 为分组 2，两位抢占，两位响应

    /* NVIC 配置 */
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;      // 选择配置 NVIC 的 EXTI15_10 线
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           // 指定 NVIC 线路使能
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; // 指定 NVIC 线路的抢占优先级为 1，在 0~3 中任选
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;        // 指定 NVIC 线路的响应优先级为 1，在 0~3 中任选
    NVIC_Init(&NVIC_InitStructure);                           // 将结构体变量交给 NVIC_Init，配置 NVIC 外设
}

/**
 * @brief 获取计数传感器的计数值
 * @param 无
 * @retval count，范围: 0~65535
 */
uint16_t InfraredSensor_Get()
{
    return count;
}
/**
 * @brief EXTI15_10 外部中断函数
 * @param 无
 * @retval 无
 * @attention 此函数为中断函数，无需调用，中断触发后自动执行；函数名为预留的指定名称，可以从启动文件复制；请确保函数名正确，不能有任何差异，否则中断函数将不能进入
 * @attention 设置中断标志位后，后面必须清除，否则中断将连续不断地触发，导致主程序卡死
 */
void EXTI15_10_IRQHandler()
{
    if (EXTI_GetITStatus(EXTI_Line14) == SET) // 判断是否是外部中断 14 号线触发的中断
    {
        count++;
        EXTI_ClearITPendingBit(EXTI_Line14); // 清除外部中断 14 号线的中断标志位
    }
}
