#include "stm32f10x.h"
#include "Delay.h"

/** 引脚配置层 */

/**
 * @brief I2C 写 SCL 引脚电平
 * @param BitValue: 协议层传入当前需要写入的 SCL 电平，范围 0~1
 * @retval None
 * @note 该函数需要用户实现内容，当 BitValue = 0 时，置 SCL 为低电平；
 *       当 BitValue = 1 时，置 SCL 为高电平
 */
void MyI2C_Write_SCL(uint8_t BitValue)
{
    GPIO_WriteBit(GPIOB, GPIO_Pin_10, (BitAction)BitValue); ///< 根据 BitValue，设置 SCL 引脚的电平

    Delay_us(10); ///< 延时 10us，防止时序频率超过要求
}

/**
 * @brief I2C 写 SDA 引脚电平
 * @param BitValue: BitValue 协议层传入的当前需要写入 SDA 的电平，范围 0~0xFF
 * @retval None
 * @note 该函数需要用户实现内容，当 BitValue = 0 时，置 SDA 为低电平；
 *       当 BitValue 非 0 时，置 SDA 为高电平
 */
void MyI2C_Write_SDA(uint8_t BitValue)
{
    GPIO_WriteBit(GPIOB, GPIO_Pin_11, (BitAction)BitValue); ///< 根据 BitValue，设置 SDA 引脚的电平

    Delay_us(10); ///< 延时 10us，防止时序频率超过要求
}

/**
 * @brief I2C 读 SDA 引脚电平
 * @param None
 * @reval BitValue: 协议层需要得到的当前 SDA 的电平，范围 0~1
 * @note 此函数需要用户实现内容，当前 SDA 为低电平时，返回0；
 *       当前SDA为高电平时，返回1
 */
uint8_t MyI2C_Read_SDA()
{
    uint8_t BitValue;
    BitValue = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11); ///< 读取低电平

    Delay_us(10); ///< 延时 10us，防止时序频率超过要求

    return BitValue;
}

/**
 * @brief I2C 初始化
 * @param None
 * @retval None
 * @note 此函数需要用户实现内容，实现 SCL 和 SDA 引脚的初始化
 */
void MyI2C_Init()
{
    /** 开启 GPIOB 时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    /** GPIO 初始化 */
    GPIO_InitTypeDef GPIO_InitStructure; ///< 初始化开漏输出
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 || GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /** 设置默认电平 */
    GPIO_SetBits(GPIOB, GPIO_Pin_10 || GPIO_Pin_11); ///< 设置 PB10 和 PB11 引脚初始化后默认为高电平（释放总线状态）
}

/** 协议层 */

/**
 * @brief I2C 起始
 * @param None
 * @retval None
 */
void MyI2C_Start()
{
    MyI2C_Write_SDA(1); ///< 释放 SDA，确保 SDA 为高电平
    MyI2C_Write_SCL(1); ///< 释放 SCL，确保 SCL 为高电平
    MyI2C_Write_SDA(0); ///< 在 SCL 高电平期间，拉低 SDA，产生起始信号
    MyI2C_Write_SCL(0); ///< 起始后把 SCL 也拉低，即为了占用总线，也为了方便总线时序的拼接
}

/**
 * @brief I2C 终止
 * @param None
 * @retval None
 */
void MyI2C_Stop()
{
    MyI2C_Write_SDA(0);
    MyI2C_Write_SCL(1);
    MyI2C_Write_SDA(1);
}

/**
 * @brief I2C 发送一个字节
 * @param Byte: 要发送的一个字节数据，范围：0X00~0XFF
 * @retval None
 */
void MyI2C_SendByte(uint8_t Byte) 
{
    uint8_t i = 0;
    while (i < 8) 
    {
        MyI2C_Write_SDA(Byte & (0x80 >> i)); ///< 使用掩码的方式取出 Byte 的指定一位数据并写入到 SDA 线
        MyI2C_Write_SCL(1); ///< 释放 SCL，从机在 SCL 高电平期间读取 SDA
        MyI2C_Write_SCL(0); ///< 拉低 SCL，主机开始发送下一位数据
        i++;
    }
}

/**
 * @brief I2C 接受一个字节
 * @param None
 * @retval Byte: 接收到的一个字节数据，范围：0x00~0xFF
 */
uint8_t MyI2C_ReceiveByte() 
{
    uint8_t Byte = 0x00;
    uint8_t i = 0;

    while (i < 8) 
    {
        MyI2C_Write_SCL(1); ///< 释放 SCL，主机机在SCL高电平期间读取 SDA
        if (MyI2C_Read_SDA() == 1)
        {
            Byte |= (0x80 >> i); ///< 读取 SDA 数据，并存储到 Byte 变量；当 SDA 为 1 时，置变量指定位为 1，当 SDA 为 0 时，不做处理，指定位为默认的初值 0
        }
        MyI2C_Write_SCL(0);
        i++;
    }
    return Byte;
}

/**
 * @brief I2C 发送应答位
 * @param Byte: 要发送的应答位，范围：0~1，0 表示应答，1 表示非应答
 * @retval None
 */
void MyI2C_SendAck(uint8_t AckBit)
{
    MyI2C_Write_SDA(AckBit); ///< 主机把应答位数据放到 SDA 线
    MyI2C_Write_SCL(1); ///< 释放 SCL，从机在 SCL 高电平期间，读取应答位
    MyI2C_Write_SCL(0); ///< 拉低 SCL，开始下一个时序模块
}

/**
 * @brief I2C 接受应答位
 * @param None
 * @retval AckBit: 接收到的应答位，范围：0~1，0 表示应答，1 表示非应答
 */
uint8_t MyI2C_ReceiveAck()
{
    uint8_t AckBit;

    MyI2C_Write_SDA(1); ///< 接收前，主机先确保释放 SDA，避免干扰从机的数据发送
    MyI2C_Write_SCL(1); ///< 释放 SCL，主机机在 SCL 高电平期间读取 SDA
    AckBit = MyI2C_Read_SDA(); ///< 将应答位存储到变量里
    MyI2C_Write_SCL(0); ///< 拉低 SCL，开始下一个时序模块

    return AckBit;
}
