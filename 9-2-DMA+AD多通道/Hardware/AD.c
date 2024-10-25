#include "stm32f10x.h"

uint16_t AD_Value[4]; ///< 定义用于存放 AD 转换结果的全局数组

/**
 * @brief AD 初始化
 * 
 * @param None
 * @retval None
 */
void AD_Init()
{
    /** 开启时钟 ADC1、GPIOA、DMA1 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    /** 设置 ADC 时钟 */
    RCC_ADCCLKConfig(RCC_PCLK2_Div6); ///< 选择时钟 6 分频，ADCCLK = 72MHz / 6 = 12MHz 

    /** GPIO 初始化：将 PA0、PA1、PA2 和 PA3 引脚初始化为模拟输入 */
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

	/** 规则组通道配置 */
    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5); ///< 规则组序列 1 的位置，配置为通道 0
    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 2, ADC_SampleTime_55Cycles5); ///< 规则组序列 1 的位置，配置为通道 1
    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 3, ADC_SampleTime_55Cycles5); ///< 规则组序列 1 的位置，配置为通道 2
    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 4, ADC_SampleTime_55Cycles5); ///< 规则组序列 1 的位置，配置为通道 3


    /** ADC 初始化 */
    ADC_InitTypeDef ADC_InitStructure;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; ///< 连续转换，使能，每转换一次规则组序列后立刻开始下一次转换
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; ///< 数据对齐，选择右对齐
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; ///< 部触发，使用软件触发，不需要外部触发
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; ///< 模式，选择独立模式，即单独使用 ADC1
    ADC_InitStructure.ADC_NbrOfChannel = 4; ///< 通道数，为 4，扫描规则组的前4个通道
    ADC_InitStructure.ADC_ScanConvMode = ENABLE; ///< 扫描模式，使能，扫描规则组的序列，扫描数量由 ADC_NbrOfChannel 确定
    ADC_Init(ADC1, &ADC_InitStructure);

    /** DMA 初始化 */
    DMA_InitTypeDef DMA_InitStructure;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR; ///< 外设基地址，给定形参 SourceAddr
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; ///< 外设数据宽度，选择字节
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; ///< 外设地址自增，选择失能，始终以 ADC 数据寄存器为源
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)AD_Value; ///< 存储器基地址，给定存放 AD 转换结果的全局数组 AD_Value
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; ///< 存储器数据宽度，选择字节
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; ///< 存储器地址自增，选择使能
    DMA_InitStructure.DMA_BufferSize = 4; ///< 转运的数据大小（转运次数），与 ADC 通道数一致
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; ///< 数据传输方向，选择由外设到存储器，ADC 数据寄存器转到数组
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; ///< 存储器到存储器，选择失能，数据由 ADC 外设触发转运到存储器
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; ///< 模式，选择循环模式
    DMA_InitStructure.DMA_Priority = DMA_Priority_High; ///< 优先级，选择高
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);

	/** DMA 和 ADC 使能 */
	DMA_Cmd(DMA1_Channel1, ENABLE); ///< DMA1 的通道 1 使能
	ADC_DMACmd(ADC1, ENABLE); ///< ADC1 触发 DMA1 的信号使能
	ADC_Cmd(ADC1, ENABLE); ///< ADC1 使能

    /** ADC 校准（固定流程，内部有电路自动校准）*/
    ADC_ResetCalibration(ADC1); 
    while (ADC_GetResetCalibrationStatus(ADC1) == SET);
    ADC_StartCalibration(ADC1);
    while (ADC_GetCalibrationStatus(ADC1) == SET);

	/* ADC 触发 */
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);	///< 软件触发 ADC 开始工作，由于 ADC 处于连续转换模式，故触发一次后 ADC 就可以一直连续不断地工作
}
