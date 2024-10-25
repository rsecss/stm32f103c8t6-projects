#include "stm32f10x.h"

/** 硬件配置层 */
/**
 * @brief SPI 写 SS 引脚电平
 * @note 此函数需要用户实现
 * @param BitValue: 协议层传入的当前需要写入 SS 的电平，范围 0~1
 *  该参数可以是以下值之一：
 *      @arg 0: 需要置 SS 为低电平
 *      @arg 1: 需要置 SS 为高电平
 * @retval None
 */ 
void MySPI_Write_SS(uint8_t BitValue)
{
    GPIO_WriteBit(GPIOA, GPIO_Pin_4, (BitAction)BitValue); ///< 根据 BitValue，设置 SS 引脚的电平
}


/**
 * @brief SPI 初始化
 * @param None
 * @retval None
 */
void MySPI_Init()
{
    /** 开启 GPIOA 的时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    /** GPIO 初始化 */
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure); ///< 将 PA4、PA5 和 PA7 引脚初始为推免输出

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure); ///< 将 PA6 引脚初始化为上拉输入

    /** SPI 初始化 */
    SPI_InitTypeDef SPI_InitStructure;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256; ///< 波特率分频，选择 128 分频
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge; ///< SPI 相位，选择第一个时钟边沿采样，极性和相位决定选择 SPI 模式 0
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low; ///< SPI 极性，选择低极性
    SPI_InitStructure.SPI_CRCPolynomial = 7; ///< CRC 多项式，暂时用不到，给默认值 7
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; ///< 数据宽度，选择为 8 位
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; ///< 方向，选择 2 线全双工
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; ///< 先行位，选择高位先行
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master; ///< 模式，选择为 SPI 主模式
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft; ///< NSS，选择由软件控制
    SPI_Init(SPI1, &SPI_InitStructure);

    /** SPI 使能 */
    SPI_Cmd(SPI1, ENABLE);

    /** 设置默认电平 */
    MySPI_Write_SS(1);  ///< SS 默认为高电平
}

/** 协议层 */

/**
 * @brief SPI 起始
 * @param None
 * @retval None
 */
void MySPI_Start()
{
    MySPI_Write_SS(0); ///< 拉低 SS，开始时序
}

/**
 * @brief SPI 终止
 * @param None
 * @retval None
 */
void MySPI_Stop()
{
    MySPI_Write_SS(1); ///< 拉高 SS，终止时序
}

/**
 * @brief SPI 交换传输一个字节，使用 SPI 模式 0
 * @attention 
 * 
 * @param SendByte: 发送一个字节
 * @retval ByteReceive: 接受一个字节
 */
uint8_t MySPI_SwapByte(uint8_t SendByte)
{
    while (SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE) != SET); ///< 等待发送数据寄存器空
    SPI_I2S_SendData(SPI1, SendByte); ///< 写入数据到发送数据寄存器，开始产生时序
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) != SET); ///< 等待接收数据寄存器非空

    return SPI_I2S_ReceiveData(SPI1); ///< 读取接收到的数据并返回
}
