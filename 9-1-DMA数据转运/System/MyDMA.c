#include "stm32f10x.h"

uint16_t MyDMA_Size; ///< 定义全局变量，用于记住 Init 函数的 Size，供 Transfer 函数使用

/**
 * @brief DMA 初始化
 * 
 * @param SourceAddr 源数组的首地址
 * @param DestinationAddr 目的数组的首地址
 * @param Size 转运的数据大小（转运次数）
 * @retval None
 */
void MyDMA_Init(uint32_t SourceAddr, uint32_t DestinationAddr, uint16_t Size) 
{
    MyDMA_Size = Size; ///< 将 Size 写入全局变量，并记住参数 Size

    /** 开启 DMA 时钟 */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    /** DMA 初始化 */
    DMA_InitTypeDef DMA_InitStructure;
    DMA_InitStructure.DMA_PeripheralBaseAddr = SourceAddr; ///< 外设基地址，给定形参 SourceAddr
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; ///< 外设数据宽度，选择字节
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable; ///< 外设地址自增，选择使能
    DMA_InitStructure.DMA_MemoryBaseAddr = DestinationAddr; ///< 存储器基地址，给定形参 DestinationAddr
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; ///< 存储器数据宽度，选择字节
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; ///< 存储器地址自增，选择使能
    DMA_InitStructure.DMA_BufferSize = Size; ///< 转运的数据大小（转运次数）
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; ///< 数据传输方向，选择由外设到存储器
    DMA_InitStructure.DMA_M2M = DMA_M2M_Enable; ///< 存储器到存储器，选择使能
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal; ///< 模式，选择正常模式
    DMA_InitStructure.DMA_Priority = DMA_Priority_High; ///< 优先级，选择高
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);

    /** DMA 使能 */
    DMA_Cmd(DMA1_Channel1, DISABLE); ///< 先不给使能，初始化后不会立刻工作，等后续调用 Transfer 后，再开始
}

/**
 * @brief 启动 DMA 数据转运
 * 
 * @param None
 * @retval None
 */
void MyDMA_Transfer()
{
    DMA_Cmd(DMA1_Channel1, DISABLE); ///< DMA 失能，在写入传输计数器之前，需要 DMA 暂停工作
    DMA_SetCurrDataCounter(DMA1_Channel1, MyDMA_Size); ///< 写入传输计数器，指定将要转运的次数
    DMA_Cmd(DMA1_Channel1, ENABLE); ///< DMA 使能，开始工作

    while (DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET); ///< 等待 DMA 工作完成
    DMA_ClearFlag(DMA1_FLAG_TC1); ///< 清除工作完成标志位
}
