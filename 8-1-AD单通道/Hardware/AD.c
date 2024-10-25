#include "stm32f10x.h"

/**
 * @brief AD 函数初始化
 * 
 * @param 无
 * @retval 无
 */
void AD_Init()
{
    /* 开启时钟 ADC1 和 GPIOA */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    /* 设置 ADC 时钟 */
    RCC_ADCCLKConfig(RCC_PCLK2_Div6); ///< 选择时钟 6 分频，ADCCLK = 72MHz / 6 = 12MHz 

    /* GPIO 初始化 */
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* 规则组通道配置 */
    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5); ///< 规则组序列 1 的位置，配置为通道 0 

    /* ADC 初始化 */
    ADC_InitTypeDef ADC_InitStructure;
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; ///< 连续转换，失能，每转换一次规则组序列后停止
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; ///< 数据对齐，选择右对齐
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; ///< 部触发，使用软件触发，不需要外部触发
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; ///< 模式，选择独立模式，即单独使用 ADC1
    ADC_InitStructure.ADC_NbrOfChannel = 1; ///< 通道数，为 1，仅在扫描模式下，才需要指定大于 1 的数，在非扫描模式下，只能是 1
    ADC_InitStructure.ADC_ScanConvMode = DISABLE; ///< 扫描模式，失能，只转换规则组的序列 1 这一个位置 
    ADC_Init(ADC1, &ADC_InitStructure);

    /* ADC 使能 */
    ADC_Cmd(ADC1, ENABLE);

    /* ADC 校准（固定流程，内部有电路自动校准）*/
    ADC_ResetCalibration(ADC1); 
    while (ADC_GetResetCalibrationStatus(ADC1) == SET);
    ADC_StartCalibration(ADC1);
    while (ADC_GetCalibrationStatus(ADC1) == SET);

}

/**
 * @brief 获取 AD 转换的值
 * 
 * @param 无
 * @return uint16_t 读数据寄存器，得到 AD 转换的结果
 */
uint16_t AD_GetValue()
{
    ADC_SoftwareStartConvCmd(ADC1, ENABLE); ///< 软件触发 AD 转换一次
    while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET); ///< 等待 EOC 标志位，即等待 AD 转换结束

    return ADC_GetConversionValue(ADC1); 
}
