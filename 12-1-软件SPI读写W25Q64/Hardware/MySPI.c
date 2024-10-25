#include "stm32f10x.h"

/** 引脚配置层 */

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
 * @brief SPI 写 SCK 引脚电平
 * @note 此函数需要用户实现
 * @param BitValue: 协议层传入的当前需要写入 SCK 的电平，范围 0~1
 *  该参数可以是以下值之一：
 *      @arg 0: 需要置 SCK 为低电平
 *      @arg 1: 需要置 SCK 为高电平
 * @retval None
 */ 
void MySPI_Write_SCK(uint8_t BitValue)
{
    GPIO_WriteBit(GPIOA, GPIO_Pin_5, (BitAction)BitValue); ///< 根据 BitValue，设置 SCK 引脚的电平
}

/**
 * @brief SPI 写 MOSI 引脚电平
 * @note 此函数需要用户实现
 * @param BitValue: 协议层传入的当前需要写入 MOSI 的电平，范围 0~0xFF
 *  该参数可以是以下值之一：
 *      @arg 0: 需要置 MOSI 为低电平
 *      @arg 1: 需要置 MOSI 为高电平
 * @retval None
 */ 
void MySPI_Write_MOSI(uint8_t BitValue)
{
    GPIO_WriteBit(GPIOA, GPIO_Pin_7, (BitAction)BitValue); ///< 根据 BitValue，设置 MOSI 引脚的电平
}

/**
 * @brief SPI 读 MISO 引脚电平
 * @note 此函数需要用户实现
 * @param BitValue: 协议层传入的当前需要读取 MISO 的电平，范围 0 ~ 0xFF
 * @retval None
 * @attention 当前 MISO 为低电平时，返回 0；当前 MISO 为高电平时，返回 1
 */
uint8_t MySPI_Read_MISO()
{
    return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6); ///< 读取 MISO 电平并返回
}

/**
 * @brief SPI 初始化
 * @note 此函数需要用户实现，实现包括 SS、SCK、MOSI 和 MISO 引脚初始化
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

    /** 设置默认电平 */
    MySPI_Write_SS(1);  ///< SS 默认为高电平
    MySPI_Write_SCK(0); ///< SCK 默认为低电平
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
    uint8_t i = 0;
    uint8_t ReceiveByte = 0x00; ///< 定义接收的数据，并赋初值 0x00，此处必须赋初值 0x00，后面会用到

    while (i < 8)
    {
        MySPI_Write_MOSI(SendByte & (0x80 >> i)); ///< 使用掩码的方式取出 ByteSend 的指定一位数据并写入到 MOSI 线
        MySPI_Write_SCK(1); ///< 拉高 SCK，上升沿移出数据
        if (MySPI_Read_MISO() == 1)
        {
            ReceiveByte |= (0x80 >> i); ///< 读取 MISO 数据，并存储到变量；当 MISO 为 1 时，置变量指定位为 1，当 MISO 为 0 时，不做处理，指定位为默认的初值 0
        }
        MySPI_Write_SCK(0); ///< 拉低 SCK，下降沿移入数据
        i++;
    }
    return ReceiveByte;
}
